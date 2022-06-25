#include "MainView.h"
#include "imgui.h"
#include "imgui_impl_opengl3_loader.h"
#include <imgui_stdlib.h>

#include <imfilebrowser.h>
#include <utility>

MainView::MainView(const Renderer &renderer) : renderer_(renderer) {
    choose_video_dialog_.SetTitle("Choose video");
    choose_video_dialog_.SetTypeFilters({".avi", ".mp4"});
}

void MainView::logWindow(const std::string &log) const {
    ImGui::Begin("Log window");

    ImGui::TextWrapped("%s", log.c_str());

    ImGui::End();
}

void MainView::matWindow(const cv::Mat &image) const {
    ImGui::Begin("Mat window");

    // Image should be RGBA
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
    ImGui::Image(reinterpret_cast<void *>( static_cast<intptr_t>( texture )), ImVec2(image.cols, image.rows));

    ImGui::End();
}

void MainView::textInputWindow(const std::string &name, std::string *text) const {
    std::string window_name = "Text editor. Parameter: " + name;
    ImGui::Begin(window_name.c_str());
    ImGui::InputText("Text", text);
    ImGui::End();
}

void MainView::videoChooseWindow(const std::string &name, std::string *path) {
    std::string window_name = "Choose video. Parameter: " + name;
    ImGui::Begin(window_name.c_str());
    if (ImGui::Button("Open file dialog")) {
        choose_video_dialog_.Open();
    }
    ImGui::Text("You've chosen: %s", (*path).c_str());
    ImGui::End();

    choose_video_dialog_.Display();

    if (choose_video_dialog_.HasSelected()) {
        *path = choose_video_dialog_.GetSelected().string();
        choose_video_dialog_.ClearSelected();
    }
}

void MainView::chooseGeneratorWindow(int *generator_num, const std::vector<std::string> &items,
                                     const std::function<void(void)> &generate_button_callback) const {
    ImGui::Begin("Main window");

    if (ImGui::BeginCombo("##combo", items[*generator_num].c_str())) {
        for (int i = 0; i < items.size(); i++) {
            bool is_selected = (*generator_num == i);
            if (ImGui::Selectable(items[i].c_str(), is_selected)) {
                *generator_num = i;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    if (ImGui::Button("Generate!")) {
        generate_button_callback();
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

    ImGui::End();
}
