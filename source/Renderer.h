#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include "Window.h"

#define FRAME_COUNT 2

class Renderer
{
public:
    friend class Editor;

	Renderer();
	virtual ~Renderer();
    void update();
    void render();

	IDXGISwapChain1* createSwapChain(Window const &window, IDXGIFactory5 *factory, ID3D12CommandQueue *queue);
	void createRenderTagets(ID3D12CommandQueue *queue);
private:

    void build_command_resourses();

    Editor* editor;
	ID3D12Debug* debug;
	ID3D12Resource* renderTargets[FRAME_COUNT];

	IDXGIAdapter1 * findAdapter();
	IDXGIFactory5 * createFactory();
	void setupDebug();
};