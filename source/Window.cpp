#include "Window.h"
#include <stdio.h>
#include <sdl\include\SDL_syswm.h>

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

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Window::update()
{
    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_KEYDOWN:
                printf("Nice key m8\n");
                break;
            
            case SDL_WINDOWEVENT:
            {
                switch (event.window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                    exit(0);
                    break;
                }
                break;
            }

            case SDL_SYSWMEVENT:
            {
                SDL_SysWMmsg *message = event.syswm.msg;
                if (message->subsystem == SDL_SYSWM_WINDOWS)
                {
                    ImGui_ImplWin32_WndProcHandler(
                        message->msg.win.hwnd, 
                        message->msg.win.msg, 
                        message->msg.win.wParam,
                        message->msg.win.lParam);
                }
                break;
            }
        }
    }
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
