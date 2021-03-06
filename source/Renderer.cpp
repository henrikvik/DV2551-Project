#include "Renderer.h"
#include "Helper.h"
#include "Editor.h"
#include "Window.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "D3D12Timer.hpp"
#include "ConstantBuffer.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

static const FLOAT clearColor[4] = { 0, 1, 1, 1 };

Renderer::Renderer(Window* w)
{
    sign_root_buffer    = nullptr;
    sign_table_buffer   = nullptr;
    sign_root_constant  = nullptr;
    pipe_root_buffer    = nullptr;
    pipe_table_buffer   = nullptr;
    pipe_root_constant  = nullptr;
    running             = true;
    window              = w;
    num_vertices        = 1000;
    num_buffers         = 32;

#ifdef _DEBUG
    setupDebug();
#endif

	auto adapter = findAdapter();
	D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&g.device));
	SafeRelease(adapter);
    BreakOnFail(g.device->GetDeviceRemovedReason());

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = 1;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    BreakOnFail(g.device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g.font_heap)));

    build_command_resourses();
    g.swap_chain = static_cast<IDXGISwapChain3*>(createSwapChain(*w, createFactory(), g.command_queue));
    build_fence();
    build_rs();
    createRenderTagets();

    editor = new Editor(this);
    timer = new D3D12Timer(g.device, 3);
    BreakOnFail(g.device->GetDeviceRemovedReason());

	ConstantBuffer::CreateDescHeap(g.cbdHeap, num_buffers);

    // Creating the pipeline states and root signs
    restart();
}

Renderer::~Renderer()
{
	SafeRelease(g.device);
    SafeDelete(editor);
    SafeDelete(timer);

#ifdef _DEBUG
	SafeRelease(debug);
#endif
}

void Renderer::update()
{
    editor->update();
}

void Renderer::render()
{
    frame();
    BreakOnFail(g.device->GetDeviceRemovedReason());

    ID3D12CommandList* temp[] = { g.command_list };
    g.command_queue->ExecuteCommandLists(_countof(temp), temp);
    BreakOnFail(g.device->GetDeviceRemovedReason());
    BreakOnFail(g.swap_chain->Present(0, 0)); // Present(1, 0); with vsync

	// double the wait, triple the fun todO TODO TODO TODO TODO TODO
    next_frame();
    wait_for_gpu();

	UINT64 frequency = 0;
	BreakOnFail(g.command_queue->GetTimestampFrequency(&frequency));
	timer->CalculateTime(frequency);
}

void Renderer::frame()
{
    BreakOnFail(g.command_allocator->Reset());
    BreakOnFail(g.command_list->Reset(g.command_allocator, nullptr));

    g.command_list->RSSetViewports(1, &g.view_port);
    g.command_list->RSSetScissorRects(1, &g.scissor_rect);
    g.command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(g.render_target[g.frame_index], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle(g.render_target_heap->GetCPUDescriptorHandleForHeapStart(), g.frame_index, g.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
    g.command_list->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, nullptr);
    g.command_list->ClearRenderTargetView(renderTargetViewHandle, clearColor, 0, nullptr);
    g.command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
    BreakOnFail(g.device->GetDeviceRemovedReason());

    static ConstantBuffer buffers[32];

    float f = 1.f; // test

    auto set_timer = [&](PipelineState& pipe, UINT index)
    {
        g.command_list->SetGraphicsRootSignature(pipe.getRootSignature()->get_ptr());
        g.command_list->SetPipelineState(pipe);

        switch (pipe.getRootSignature()->get_type())
        {
            case RootSignature::Type::RootConstantBuffer:
            {
                for (size_t i = 0; i < pipe.getRootSignature()->get_num(); i++)
                {
                    g.command_list->SetGraphicsRootConstantBufferView(i, buffers[i].getResource()->GetGPUVirtualAddress());
                }
            }
            break;
            case RootSignature::Type::RootConstant:
                for (size_t j = 0; j < pipe.getRootSignature()->get_num(); j++) {
                    g.command_list->SetGraphicsRoot32BitConstants(j, 1, &f, 0);
                }
            break;
            case RootSignature::Type::TableConstantBuffer:
                static ID3D12DescriptorHeap* heaps[] = { g.cbdHeap };
                g.command_list->SetDescriptorHeaps(ARRAYSIZE(heaps), heaps);
                g.command_list->SetGraphicsRootDescriptorTable(0, g.cbdHeap->GetGPUDescriptorHandleForHeapStart());
            break;
        }

        timer->Start(g.command_list, index);
        g.command_list->DrawInstanced(num_vertices, 1, 0, 0);
        timer->Stop(g.command_list, index);
    };

	BreakOnFail(g.device->GetDeviceRemovedReason());

    if (running)
    {
        set_timer(*pipe_root_buffer, RB_TIMER);
        set_timer(*pipe_table_buffer, TB_TIMER);
        set_timer(*pipe_root_constant, RC_TIMER);
    }

	timer->ResolveQuery(g.command_list);

    editor->render();

    g.command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(g.render_target[g.frame_index], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
    BreakOnFail(g.command_list->Close());
}

void Renderer::build_command_resourses()
{
    // building command queue
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	BreakOnFail(g.device->GetDeviceRemovedReason());
    BreakOnFail(g.device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&g.command_queue)));

    // building allocator
    BreakOnFail(g.device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g.command_allocator)));

    // building command list
    BreakOnFail(g.device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g.command_allocator, nullptr, IID_PPV_ARGS(&g.command_list)));
    BreakOnFail(g.command_list->Close());
}

void Renderer::build_fence()
{
    // building the fence
    BreakOnFail(g.device->CreateFence(g.fence_value, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g.fence)));
    g.fence_value = 1;

    // building the fence event
    g.fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (g.fence_event == nullptr)
        BreakOnFail(0x80004003);
}

void Renderer::build_rs()
{
    g.view_port = CD3DX12_VIEWPORT(0.0f, 0.0f, window->getWidth(), window->getWidth());
    g.scissor_rect = CD3DX12_RECT(0, 0, LONG(window->getWidth()), LONG(window->getWidth()));
}

void Renderer::wait_for_gpu()
{
    BreakOnFail(g.command_queue->Signal(g.fence, g.fence_value));

    //wait for the event completion
    BreakOnFail(g.fence->SetEventOnCompletion(g.fence_value, g.fence_event));
    WaitForSingleObjectEx(g.fence_event, INFINITE, FALSE);
    g.fence_value++;
}

void Renderer::next_frame()
{
    // signaling the gpu
    const UINT64 currentFenceValue = g.fence_value;
    BreakOnFail(g.command_queue->Signal(g.fence, currentFenceValue));

    g.frame_index = g.swap_chain->GetCurrentBackBufferIndex();

    // sit and wait for the gpu to yell "GO!"
    if (g.fence->GetCompletedValue() < g.fence_value)
    {
        BreakOnFail(g.fence->SetEventOnCompletion(g.fence_value, g.fence_event));
        WaitForSingleObjectEx(g.fence_event, INFINITE, FALSE);
    }
    g.fence_value = currentFenceValue + 1;
}

void Renderer::stop()
{
    running = false;
}

void Renderer::resume()
{
    running = true;
}

void Renderer::restart()
{
    SafeDelete(sign_root_buffer);
    SafeDelete(sign_table_buffer);
    SafeDelete(sign_root_constant);
    SafeDelete(pipe_root_buffer);
    SafeDelete(pipe_table_buffer);
    SafeDelete(pipe_root_constant);

    sign_root_buffer    = new RootSignature(RootSignature::Type::RootConstantBuffer, num_buffers, RootSignature::Visiblity::All);
    sign_table_buffer   = new RootSignature(RootSignature::Type::TableConstantBuffer, num_buffers, RootSignature::Visiblity::All);
    sign_root_constant  = new RootSignature(RootSignature::Type::RootConstant, num_buffers, RootSignature::Visiblity::All);
    pipe_root_buffer    = new PipelineState(sign_root_buffer);
    pipe_table_buffer   = new PipelineState(sign_table_buffer);
    pipe_root_constant  = new PipelineState(sign_root_constant);
}

IDXGIAdapter1* Renderer::findAdapter()
{
	auto factory = createFactory();
	IDXGIAdapter1 * adapter = nullptr;

	UINT adapterIndex = 0;
	while (!adapter && SUCCEEDED(factory->EnumAdapters1(adapterIndex++, &adapter)))
		if FAILED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device), nullptr)) 
			SafeRelease(adapter);

	SafeRelease(factory);
	return adapter;
}

IDXGIFactory4 * Renderer::createFactory()
{
    IDXGIFactory4* factory = nullptr;
#if _DEBUG
    BreakOnFail(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory)));
#else
    BreakOnFail(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory)));
#endif

	return factory;
}

void Renderer::setupDebug()
{
    // Enable the debug layer
    ID3D12Debug* debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
    }
}

IDXGISwapChain1* Renderer::createSwapChain(Window const &window, IDXGIFactory4 *factory, ID3D12CommandQueue *queue)
{
	IDXGISwapChain1 *swapChain;

	DXGI_SWAP_CHAIN_DESC1 swapDesc = {};
	swapDesc.Width = window.getHeight();
	swapDesc.Height = window.getWidth();
	swapDesc.Scaling = DXGI_SCALING_STRETCH;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.BufferCount = ARRAYSIZE(g.render_target);
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	factory->CreateSwapChainForHwnd(queue, window.getWindowHandle(), &swapDesc, nullptr, nullptr, &swapChain);
	return swapChain;
}

void Renderer::createRenderTagets()
{
    // creating the heap for the render targets
    D3D12_DESCRIPTOR_HEAP_DESC renderTargetViewHeapDesc = {};
    renderTargetViewHeapDesc.NumDescriptors = ARRAYSIZE(g.render_target);
    renderTargetViewHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    renderTargetViewHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    BreakOnFail(g.device->CreateDescriptorHeap(&renderTargetViewHeapDesc, IID_PPV_ARGS(&g.render_target_heap)));

    // actually creating the render targets (back & front buffers)
    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle(g.render_target_heap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < ARRAYSIZE(g.render_target); i++)
    {
        BreakOnFail(g.swap_chain->GetBuffer(i, IID_PPV_ARGS(&g.render_target[i])));
        g.device->CreateRenderTargetView(g.render_target[i], nullptr, renderTargetViewHandle);
        renderTargetViewHandle.Offset(1, g.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
    }
}