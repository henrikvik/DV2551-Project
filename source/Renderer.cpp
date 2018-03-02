#include "Renderer.h"
#include "Helper.h"
#include "Editor.h"
#include "Window.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

static const FLOAT clearColor[4] = { 1, 1, 1, 1 };

Renderer::Renderer(Window* w)
{
    editor = new Editor(this);

	auto adapter = findAdapter();
	D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&g.device));
	SafeRelease(adapter);
	SafeRelease(debug);

#ifdef _DEBUG
	setupDebug();
#endif

    build_command_resourses();
    g.swap_chain = static_cast<IDXGISwapChain3*>(createSwapChain(*w, createFactory(), g.command_queue));
    build_fence();
    createRenderTagets();
}

Renderer::~Renderer()
{
	SafeRelease(g.device);
    SafeDelete(editor);

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
    editor->render();
    
    ID3D12CommandList* temp[] = { g.command_list };
    g.command_queue->ExecuteCommandLists(_countof(temp), temp);
    g.swap_chain->Present(1, 0);

    next_frame();
    wait_for_gpu();
}

void Renderer::frame()
{
    // Starting the command list & allocator
    BreakOnFail(g.command_allocator->Reset());
    BreakOnFail(g.command_list->Reset(g.command_allocator, nullptr));

    //// Setting current testing setup
    //g.command_list->SetPipelineState(pipelineState);
    //g.command_list->SetGraphicsRootSignature(rootSignature);

    //// Setting RS
    //g.command_list->RSSetViewports(1, &viewPort);
    //g.command_list->RSSetScissorRects(1, &scissorRect);

    // Setting the Render Target Desc
    ID3D12DescriptorHeap* ppHeaps[] = { g.render_target_heap };
    g.command_list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    // Switching front & back buffers
    g.command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(g.render_target[g.frame_index], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle(g.render_target_heap->GetCPUDescriptorHandleForHeapStart(), g.frame_index, g.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
    g.command_list->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, nullptr);
    g.command_list->ClearRenderTargetView(renderTargetViewHandle, clearColor, 0, nullptr);
    g.command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(g.render_target[g.frame_index], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    // Closing the command list
    BreakOnFail(g.command_list->Close());
}

void Renderer::build_command_resourses()
{
    // building command queue
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
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
        BreakOnFail(0x80004003, __FILE__, __LINE__);
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

IDXGIFactory5 * Renderer::createFactory()
{
	IDXGIFactory5 * factory = nullptr;
	CreateDXGIFactory(IID_PPV_ARGS(&factory));
	return factory;
}

void Renderer::setupDebug()
{
	debug = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
	}
}

IDXGISwapChain1* Renderer::createSwapChain(Window const &window, IDXGIFactory5 *factory, ID3D12CommandQueue *queue)
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