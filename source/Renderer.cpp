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

    build_command_resourses();
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

void Renderer::build_command_resourses()
{
    // building command queue
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    BreakOnFail(g.device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&g.commandQueue)));

    // building allocator
    BreakOnFail(g.device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g.commandAllocator)));

    // building command list
    BreakOnFail(g.device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g.commandAllocator, nullptr, IID_PPV_ARGS(&g.commandList)));
    BreakOnFail(g.commandList->Close());
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

