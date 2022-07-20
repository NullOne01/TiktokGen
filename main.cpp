#include <iostream>
#include <SDL.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>
#include <imgui_impl_sdl.h>
#include "imgui.h"
#include "src/view/Renderer.h"
#include "plog/Severity.h"
#include "plog/Initializers/RollingFileInitializer.h"
#include "src/view_model/MainViewModel.h"
#include "plog/Log.h"
#include "src/model/user/Config.h"

int main() {
    plog::init(plog::debug, "DebugLog.txt");
    // Here should be your config file.
    Config::init("resources/config.cfg");

    try {
        Renderer renderer;
        MainView mainView(renderer);
        MainViewModel mainViewModel(mainView);

        bool show_demo_window = true;

        while (renderer.isInLoop()) {
            renderer.preFrame();

            if (show_demo_window) {
                ImGui::ShowDemoWindow(&show_demo_window);
            }
            mainViewModel.onFrame();

            renderer.postFrame();
        }
    } catch (const std::exception& e) {
        PLOG_ERROR << e.what();
    }

    return 0;
}
