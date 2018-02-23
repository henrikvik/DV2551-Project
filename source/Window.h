#pragma once

#pragma comment (lib, "sdl/SDL2.lib")
#pragma comment (lib, "sdl/SDL2main.lib")

#include <Windows.h>
#include <sdl\include\SDL.h>

class Window {
private:
	SDL_Window *sdlWindow;
public:
	Window(const char *title, UINT width, UINT height);
	~Window();

	HWND getWindowHandle();
};