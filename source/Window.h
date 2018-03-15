#pragma once

//#ifdef _WIN64
// Vi kör bara x64
#pragma comment (lib, "sdl/SDL2.lib")
#pragma comment (lib, "sdl/SDL2main.lib")
//#elif _WIN32
//#pragma comment (lib, "sdl/x86/SDL2.lib")
//#pragma comment (lib, "sdl/x86/SDL2main.lib")
//#endif

#include <Windows.h>
#include <sdl\include\SDL.h>

class Window {
private:
	SDL_Window *sdlWindow;
	UINT width, height;
public:
	Window(const char *title, UINT width, UINT height);
	~Window();
    
    void update();

	HWND getWindowHandle() const;
	UINT getWidth() const;
	UINT getHeight() const;
};