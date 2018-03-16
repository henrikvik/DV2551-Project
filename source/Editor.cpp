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
    test_timer_sec = 2.f;
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
    if (CHECK_FLAG(wnd_flags, _WINDOW_FLAG::SAVE_AS_WINDOW))    update_popup_save_as();
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
        ImGui::Text("%f ms", renderer->timer->GetDeltaTime(RB_TIMER));
        ImGui::Text("%f ms", renderer->timer->GetDeltaTime(CB_TIMER));
        ImGui::Text("%f ms", renderer->timer->GetDeltaTime(TB_TIMER));
    }
    ImGui::End();
}

void Editor::update_settings_window()
{
    if (ImGui::Begin("Editor"))
    {
        ImGui::Text("Settings");
        ImGui::DragInt("Number of Vertices", (int*)&renderer->num_vertices, 1000.f, 10000, 1000000);
        
        if (ImGui::DragInt("Number of Buffers", (int*)&renderer->num_buffers, 1.f, 1, 32)) 
            renderer->restart();
        
        if (ImGui::Button("Stop"))  
            renderer->stop();
        
        if (ImGui::Button("Run"))   
            renderer->resume();

        ImGui::SliderInt("Testing duration (seconds)", (int*)&test_timer_sec, 1.f, 10.f);
        if (ImGui::Button("Run Test & Print To File"))
            TOGGLE_FLAG(wnd_flags, _WINDOW_FLAG::SAVE_AS_WINDOW);
    }
    ImGui::End();
}

void Editor::update_popup_save_as()
{
    if (ImGui::Begin("Run Test"))
    {
        static std::string buffer;
        buffer.resize(100);
        ImGui::InputText("filename", &buffer[0], 100, ImGuiInputTextFlags_CharsNoBlank);

        if (ImGui::Button("Run & Save"))
        {
            // renderer->run_test_print(buffer, test_timer_sec * 1000.f);
            TOGGLE_FLAG(wnd_flags, _WINDOW_FLAG::SAVE_AS_WINDOW);
        }

        if (ImGui::Button("Go Back")) 
            TOGGLE_FLAG(wnd_flags, _WINDOW_FLAG::SAVE_AS_WINDOW);
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