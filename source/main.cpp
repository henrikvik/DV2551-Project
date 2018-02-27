#include "Window.h"
#include "Renderer.h"
#include "Shader.h"

#define __(v) while (1) { v
#define _ }

int main(int argc, char* argv[])
{
	Shader s(32);

    Window w("Hi", 500, 500);
    Renderer r;

	__(r.update();)        
        r.render();_

	return 0;
}