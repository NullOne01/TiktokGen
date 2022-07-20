#include "MainViewModel.h"
#include "plog/Log.h"
#include "utils/LogFunctions.h"
#include "../model/generators/video_generators/VideoGenerator.h"

MainViewModel::MainViewModel(MainView &view) : view_(view) {
}

void MainViewModel::onFrame() {
    showMainWindow();
    showLogWindow();
    showRequirementWindows();
}

void MainViewModel::showLogWindow() {
    // Read whole log file each tick? Sounds great!
    last_read_log_ = LogFunctions::getNewLogs();
    view_.logWindow(last_read_log_);
}

void MainViewModel::showMainWindow() {
    auto generator_lambda = [this]() {
        onGenerateButton();
    };

    auto select_generator_lambda = [this]() {
        onSelectGenerator();
    };

    view_.chooseGeneratorWindow(&user_state_->chosen_generator, user_state_->generator_names,
                                generator_lambda, select_generator_lambda);
}

void MainViewModel::showRequirementWindows() {
    if (user_state_->generator == nullptr) {
        return;
    }

    user_state_->generator->viewRequirements();
}

void MainViewModel::onGenerateButton() {
    LOGD << "User clicked generate button " + std::string(*user_state_);

    user_state_->generator->loadData();
    std::unique_ptr<VideoGenerator> video_generator = std::make_unique<VideoGenerator>(*user_state_->generator);
    video_generator->generate();

    LOGD << "User generated video";
}

void MainViewModel::onSelectGenerator() {
    user_state_->createGenerator();
}
