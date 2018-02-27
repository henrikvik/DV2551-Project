#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>

class Renderer
{
public:
    friend class Editor;

	Renderer();
	virtual ~Renderer();
    void update();
    void render();

private:

    void build_command_resourses();
    void build_fence();

    void wait_for_gpu();
    void next_frame();

    Editor* editor;
	IDXGIAdapter1 * findAdapter();
	IDXGIFactory5 * createFactory();

};