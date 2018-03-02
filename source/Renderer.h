#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>

class PipelineState;
class RootSignature;
class Window;
class Renderer
{
public:
    friend class Editor;

	Renderer(Window* w);
	virtual ~Renderer();
    void update();
    void render();
    void frame();

	IDXGISwapChain1* createSwapChain(Window const &window, IDXGIFactory5 *factory, ID3D12CommandQueue *queue);
private:

    void build_command_resourses();
    void build_fence();
    void build_rs();
    void createRenderTagets();

    void wait_for_gpu();
    void next_frame();

    Editor* editor;
	ID3D12Debug* debug;
    Window* window;

	IDXGIAdapter1* findAdapter();
	IDXGIFactory5* createFactory();
	void setupDebug();

    // test 
    PipelineState* pipelineState;
    RootSignature* rootSignature;
};