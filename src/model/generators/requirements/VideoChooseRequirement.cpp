#include "VideoChooseRequirement.h"

#include <utility>
#include "imgui.h"

VideoChooseRequirement::VideoChooseRequirement(const std::string &name) : GeneratorRequirement(name) {
    choose_video_dialog_.SetTitle("Choose video");
    choose_video_dialog_.SetTypeFilters({".avi", ".mp4"});
}

void VideoChooseRequirement::showView() {
    std::string window_name = "Choose video. Parameter: " + name;
    ImGui::Begin(window_name.c_str());
    if (ImGui::Button("Open file dialog")) {
        choose_video_dialog_.Open();
    }
    ImGui::Text("You've chosen: %s", path_.c_str());
    ImGui::End();

    choose_video_dialog_.Display();

    if (choose_video_dialog_.HasSelected()) {
        path_ = choose_video_dialog_.GetSelected().string();
        choose_video_dialog_.ClearSelected();
    }
}

std::string VideoChooseRequirement::getValue() {
    return path_;
}
