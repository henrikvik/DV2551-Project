#include "Window.h"

Window::Window(const char *title, UINT width, UINT height)
{
	this->width = width;
	this->height = height;

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

void Window::update()
{
}

UINT Window::getWidth() const
{
	return width;
}

UINT Window::getHeight() const
{
	return height;
}

HWND Window::getWindowHandle() const
{
	return GetActiveWindow();
}
