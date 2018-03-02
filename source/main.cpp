#include "Window.h"
#include "Renderer.h"
#include "ConstantBuffer.h"

#define __(v) while (1) { v
#define _ }

int main(int argc, char* argv[])
{
	// Shader s(32);

    Window w("Hi", 500, 500);
    Renderer r(&w);
	ConstantBuffer test(1, 10, 10);

	__(r.update();)        
        r.render();_

	return 0;
}