#include "Renderer.h"
#include "Helper.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

Renderer::Renderer()
{
	auto adapter = findAdapter();
	D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&g.device));
	SafeRelease(adapter);
}

Renderer::~Renderer()
{
	SafeRelease(g.device);
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
