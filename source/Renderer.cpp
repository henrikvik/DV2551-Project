#include "Renderer.h"
#include "Helper.h"
#include "Editor.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

Renderer::Renderer()
{
    editor = new Editor(this);

	auto adapter = findAdapter();
	D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&g.device));
	SafeRelease(adapter);
}

Renderer::~Renderer()
{
	SafeRelease(g.device);
    SafeDelete(editor);
}

void Renderer::update() 
{
    editor->update();
}

void Renderer::render()
{
    editor->render();
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

IDXGISwapChain1* Renderer::createSwapChain(Window const &window, IDXGIFactory5 *factory, ID3D12CommandQueue *queue)
{
	IDXGISwapChain1 *swapChain;

	DXGI_SWAP_CHAIN_DESC1 swapDesc = {};
	swapDesc.Width = window.getHeight();
	swapDesc.Height = window.getWidth();
	swapDesc.Scaling = DXGI_SCALING_STRETCH;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.BufferCount = ARRAYSIZE(renderTargets);
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	factory->CreateSwapChainForHwnd(queue, window.getWindowHandle(), &swapDesc, nullptr, nullptr, &swapChain);
	return swapChain;
}

void Renderer::createRenderTagets(ID3D12CommandQueue * queue)
{
}
