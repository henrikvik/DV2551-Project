#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include "D3D12Timer.hpp"

#define RB_TIMER 0
#define TB_TIMER 1
#define CB_TIMER 2

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

    IDXGISwapChain1* createSwapChain(Window const &window, IDXGIFactory4 *factory, ID3D12CommandQueue *queue);
private:

    void build_command_resourses();
    void build_fence();
    void build_rs();
    void createRenderTagets();

    void wait_for_gpu();
    void next_frame();

    void stop();
    void resume();
    void restart();

    bool running;
    Editor* editor;
    ID3D12Debug* debug;
    Window* window;
    D3D12Timer* timer;
    UINT num_vertices;
    UINT num_buffers;
    IDXGIAdapter1* findAdapter();
    IDXGIFactory4* createFactory();
    void setupDebug();

    RootSignature* sign_root_buffer;
    RootSignature* sign_table_buffer;
    RootSignature* sign_root_constant;
    PipelineState* pipe_root_buffer;
    PipelineState* pipe_table_buffer;
    PipelineState* pipe_root_constant;
};