#pragma once

#ifdef _WIN64
#pragma comment (lib, "sdl/x64/SDL2.lib")
#pragma comment (lib, "sdl/x64/SDL2main.lib")
#elif _WIN32
#pragma comment (lib, "sdl/x86/SDL2.lib")
#pragma comment (lib, "sdl/x86/SDL2main.lib")
#endif

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