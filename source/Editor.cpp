#include "Editor.h"
#include "Renderer.h"
#include "Helper.h"

#include <string>
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>

Editor::Editor(Renderer* _renderer)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // io.NavFlags |= ImGuiNavFlags_EnableKeyboard;  // Enable Keyboard Controls
    ImGui_ImplDX12_Init(GetActiveWindow(), FRAME_COUNT, g.device, DXGI_FORMAT_R8G8B8A8_UNORM, g.font_heap->GetCPUDescriptorHandleForHeapStart(), g.font_heap->GetGPUDescriptorHandleForHeapStart());

    // Setup style
    ImGui::StyleColorsDark();

    renderer = _renderer;
    wnd_flags = 0;
    TOGGLE_FLAG(wnd_flags, _WINDOW_FLAG::MAIN_WINDOW);
    TOGGLE_FLAG(wnd_flags, _WINDOW_FLAG::SETTINGS_WINDOW);
}

Editor::~Editor() 
{
    ImGui_ImplDX12_Shutdown();
    ImGui::DestroyContext();
}

void Editor::update()
{
    ImGui_ImplDX12_NewFrame(g.command_list);

    if (CHECK_FLAG(wnd_flags, _WINDOW_FLAG::MAIN_WINDOW))       update_main_window();
    if (CHECK_FLAG(wnd_flags, _WINDOW_FLAG::SETTINGS_WINDOW))   update_settings_window();
}

void Editor::update_main_window()
{
    if (ImGui::Begin("Results"))
    {
        ImGui::Columns(2, "cols", true);

        // Column 1
        ImGui::Text("Root Constants");
        ImGui::Text("Root Constant Buffer");
        ImGui::Text("Descriptor Table");

        // Column 2
        ImGui::NextColumn();
        ImGui::Text("%lld ms", renderer->timer->GetDeltaTime(RB_TIMER));
        ImGui::Text("%lld ms", renderer->timer->GetDeltaTime(CB_TIMER));
        ImGui::Text("%lld ms", renderer->timer->GetDeltaTime(TB_TIMER));
    }
    ImGui::End();
}

void Editor::update_settings_window()
{
    if (ImGui::Begin("Editor"))
    {
        ImGui::Text("Settings");
        ImGui::SliderInt("Number of Vertices", (int*)&renderer->num_vertices, 0, 100000);
        ImGui::SliderInt("Number of Buffers", (int*)&renderer->num_buffers, 0, 64);
        if (ImGui::Button("Run")) printf("Not implemented.\n");
        if (ImGui::Button("Run & Save As")) printf("Not implemented.\n");
    }
    ImGui::End();
}

void Editor::render()
{
    static float clearColor[4] = { 1, 1, 1, 1 };

    g.command_list->SetDescriptorHeaps(1, &g.font_heap);
    
    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData());
}