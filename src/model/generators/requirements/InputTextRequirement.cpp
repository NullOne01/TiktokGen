#include "InputTextRequirement.h"
#include "imgui.h"
#include "imgui_impl_opengl3_loader.h"
#include <imgui_stdlib.h>

void InputTextRequirement::showView() {
    std::string window_name = "Text editor. Parameter: " + name;
    ImGui::Begin(window_name.c_str());
    ImGui::InputText("Text", &text_);
    ImGui::End();
}

std::string InputTextRequirement::getValue() {
    return text_;
}

InputTextRequirement::InputTextRequirement(const std::string &name) : GeneratorRequirement(name) {}
