#include "Editor.h"
#include "Renderer.h"
#include "Helper.h"

#include <string>
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include "ExclWriter.h"

Editor::Editor(Renderer* _renderer)
{
    testing = false;
    testing_timer = 1;
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // io.NavFlags |= ImGuiNavFlags_EnableKeyboard;  // Enable Keyboard Controls
    ImGui_ImplDX12_Init(GetActiveWindow(), FRAME_COUNT, g.device, DXGI_FORMAT_R8G8B8A8_UNORM, g.font_heap->GetCPUDescriptorHandleForHeapStart(), g.font_heap->GetGPUDescriptorHandleForHeapStart());

    // Setup style
    ImGui::StyleColorsDark();

    renderer = _renderer;
    wnd_flags = 0;
    name = "Hejpappahadetbrasesp.txt";

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

    if (begin != 0 && begin + (testing_timer * 1000.f) < clock())
    {
        begin = 0;
        ExclWriter::writeToFile(name, doubles);
        doubles.clear();
    }
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

        std::vector<double> line;
        line.push_back(renderer->timer->GetDeltaTime(RB_TIMER));
        line.push_back(renderer->timer->GetDeltaTime(CB_TIMER));
        line.push_back(renderer->timer->GetDeltaTime(TB_TIMER));
        doubles.push_back(line);
    }
    ImGui::End();
}

void Editor::update_settings_window()
{
    if (ImGui::Begin("Editor"))
    {
        ImGui::Text("Settings");
        ImGui::DragInt("Vertex Count", (int*)&renderer->num_vertices, 1000.f, 10000, 1000000);
        
        if (ImGui::DragInt("Buffer Count", (int*)&renderer->num_buffers, 1.f, 1, 32)) 
            renderer->restart();
        
        if (ImGui::Button("Pause"))  
            renderer->stop();
        
        if (ImGui::Button("Resume"))   
            renderer->resume();

        ImGui::SliderInt("Testing duration (seconds)", &testing_timer, 1, 10);
        if (ImGui::Button("Run Test & Print To File"))
            TOGGLE_FLAG(wnd_flags, _WINDOW_FLAG::SAVE_AS_WINDOW);
    }
    ImGui::End();
}

void Editor::update_popup_save_as()
{
    if (ImGui::Begin("Run Test"))
    {
        name.resize(100);
        ImGui::InputText("filename", &name[0], 100, ImGuiInputTextFlags_CharsNoBlank);

        if (ImGui::Button("Run & Save"))
        {
            begin = clock();
            TOGGLE_FLAG(wnd_flags, _WINDOW_FLAG::SAVE_AS_WINDOW);
            doubles.clear();
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