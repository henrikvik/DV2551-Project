#include "Window.h"

Window::Window(const char *title, UINT width, UINT height)
{
	sdlWindow = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0
	);
}

Window::~Window()
{
	SDL_DestroyWindow(sdlWindow);
}

HWND Window::getWindowHandle()
{
	return GetActiveWindow();
}
