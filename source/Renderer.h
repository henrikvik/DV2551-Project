#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#pragma comment(lib, "d3d12.lib")

class Renderer
{
public:
	Renderer();
	virtual ~Renderer();

private:
	IDXGIAdapter1 * findAdapter();
	IDXGIFactory5 * createFactory();

};