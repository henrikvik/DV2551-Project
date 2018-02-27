#include "Window.h"
#include "Renderer.h"

int main(int argc, char* argv[])
{
    Window w("Hi", 500, 500);
    Renderer r;

    while (1)
    {
        r.update();
        r.render();
    }

	return 0;
}