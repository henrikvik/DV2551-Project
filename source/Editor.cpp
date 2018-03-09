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
    TOGGLE_FLAG(wnd_flags, _WINDOW_FLAG::ROOT_WINDOW);
    TOGGLE_FLAG(wnd_flags, _WINDOW_FLAG::PIPE_WINDOW);
}

Editor::~Editor() 
{
    ImGui_ImplDX12_Shutdown();
    ImGui::DestroyContext();
}

void Editor::update()
{
    ImGui_ImplDX12_NewFrame(g.command_list);

    if (CHECK_FLAG(wnd_flags, _WINDOW_FLAG::MAIN_WINDOW)) update_main_window();
    if (CHECK_FLAG(wnd_flags, _WINDOW_FLAG::ROOT_WINDOW)) update_root_window();
    if (CHECK_FLAG(wnd_flags, _WINDOW_FLAG::PIPE_WINDOW)) update_pipe_window();
}

void Editor::update_main_window()
{
    if (ImGui::Begin("Editor - Main"))
    {
        ImGui::Columns(3, "cols", true);
        ImGui::Text("Pipeline State");

        for (int i = 0; i < 5; i++)
        {
            std::string str = "PS_" + std::to_string(i);
            ImGui::Text(str.c_str());
            ImGui::SameLine();
            ImGui::Button("Edit");
        }

        for (int i = 0; i < 5; i++)
        {
            std::string str = "RS_" + std::to_string(i);
            ImGui::Text(str.c_str());
            ImGui::SameLine();
            ImGui::Button("Edit");
        }

        ImGui::NextColumn();
        ImGui::Text("Experiment");
        ImGui::Button("Validate");
        ImGui::Button("Run");
        ImGui::Button("Run & Save As");
    }
    ImGui::End();
}

void Editor::update_root_window()
{
    if (ImGui::Begin("Editor - Sub"))
    {

    }
    ImGui::End();
}

void Editor::update_pipe_window()
{
    if (ImGui::Begin("Editor - Pipeline State"))
    {

    }
    ImGui::End();
}

void Editor::render()
{
    static float clearColor[4] = { 1, 1, 1, 1 };

    // Rendering ImGui (Taken from the ImGui Sample)
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = g.render_target[g.frame_index];
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    g.command_list->Reset(g.command_allocator, NULL);
    g.command_list->ResourceBarrier(1, &barrier);

    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle(g.render_target_heap->GetCPUDescriptorHandleForHeapStart(), g.frame_index, g.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
    g.command_list->ClearRenderTargetView(renderTargetViewHandle, (float*)&clearColor, 0, nullptr);
    g.command_list->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, nullptr);
    g.command_list->SetDescriptorHeaps(1, &g.font_heap);
    
    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData());
    
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    g.command_list->ResourceBarrier(1, &barrier);
}