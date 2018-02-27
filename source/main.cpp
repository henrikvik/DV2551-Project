#include "Window.h"
#include "Renderer.h"
#include "ConstantBuffer.h"

int main(int argc, char* argv[])
{
    Window w("Hi", 500, 500);
    Renderer r;
	// ConstantBuffer test(10, 10);

    while (1)
    {
        r.update();
        r.render();
    }

	return 0;
}