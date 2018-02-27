#include "Renderer.h"
#include "Helper.h"
#include "Editor.h"
#include "Window.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

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
    editor->render();
    // execute command list
    // swapchain present
    g.swap_chain->Present(0, 1);
    next_frame();
    wait_for_gpu();
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

    g.fence_index = g.swap_chain->GetCurrentBackBufferIndex();

    // sit and wait for the gpu to yell "GO!"
    if (g.fence->GetCompletedValue() < g.fence_value)
    {
        BreakOnFail(g.fence->SetEventOnCompletion(g.fence_value, g.fence_event));
        WaitForSingleObjectEx(g.fence_event, INFINITE, FALSE);
    }
    g.fence_value = currentFenceValue + 1;
}

IDXGIAdapter1 * Renderer::findAdapter()
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
	swapDesc.BufferCount = ARRAYSIZE(g.renderTargets);
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	factory->CreateSwapChainForHwnd(queue, window.getWindowHandle(), &swapDesc, nullptr, nullptr, &swapChain);
	return swapChain;
}

void Renderer::createRenderTagets(ID3D12CommandQueue * queue)
{
    // Tjena Notch, Har du lust å snacka?
}