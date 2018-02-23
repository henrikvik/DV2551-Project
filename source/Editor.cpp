#include "Editor.h"
#include "Renderer.h"
#include "Helper.h"

#include <string>
#include "ImGui\imgui.h"

Editor::Editor(Renderer* _renderer)
{
    renderer = _renderer;
    wnd_flags = 0;
    TOGGLE_FLAG(wnd_flags, _WINDOW_FLAG::MAIN_WINDOW);
}

Editor::~Editor() { }

void Editor::update()
{
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