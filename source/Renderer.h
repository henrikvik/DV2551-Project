#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>

class Renderer
{
public:
    friend class Editor;

	Renderer();
	virtual ~Renderer();

private:
	IDXGIAdapter1 * findAdapter();
	IDXGIFactory5 * createFactory();

};