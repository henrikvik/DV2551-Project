#include "Window.h"
#include "Renderer.h"
#include "ConstantBuffer.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx12.h"

#define D_O_ while (1) { 
#define S_H_I_T_ w.update(); r.update(); r.render();
#define F_O_R_E_V_E_R_ } return 0;

int main(int argc, char* argv[])
{
    Window w("Hi", 500, 500);
    Renderer r(&w);

	D_O_ S_H_I_T_ F_O_R_E_V_E_R_
}