#include "MainView.h"
#include "imgui.h"
#include "imgui_impl_opengl3_loader.h"
#include <imgui_stdlib.h>

#include <imfilebrowser.h>
#include <utility>

MainView::MainView(const Renderer &renderer) : renderer_(renderer) {
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

void MainView::chooseGeneratorWindow(int *generator_num, const std::vector<std::string> &items,
                                     const std::function<void(void)> &generate_button_callback,
                                     const std::function<void(void)> &generator_select_callback) const {
    ImGui::Begin("Main window");

    if (ImGui::BeginCombo("##combo", items[*generator_num].c_str())) {
        for (int i = 0; i < items.size(); i++) {
            bool is_selected = (*generator_num == i);
            if (ImGui::Selectable(items[i].c_str(), is_selected)) {
                *generator_num = i;
                generator_select_callback();
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
