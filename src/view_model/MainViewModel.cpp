#include "MainViewModel.h"
#include "plog/Log.h"
#include "utils/LogFunctions.h"
#include "../model/generators/video_generators/VideoGenerator.h"

MainViewModel::MainViewModel(MainView &view) : view_(view) {}

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

    view_.chooseGeneratorWindow(&user_state_->chosen_generator, user_state_->generator_names,
                                generator_lambda);
}

void MainViewModel::showRequirementWindows() {
    auto &needed_requirements = user_state_->generator_requirements[user_state_->chosen_generator];

    for (int i = 0; i < needed_requirements.size(); ++i) {
        bindRequirement(needed_requirements[i], i);
    }
}

void MainViewModel::bindRequirement(const GeneratorRequirement &requirement, int data_index) {
    switch (requirement.type) {
        case NEED_TEXT :
            view_.textInputWindow(requirement.name, &user_state_->data_[data_index]);
            break;
        case NEED_VIDEO:
            view_.videoChooseWindow(requirement.name, &user_state_->data_[data_index]);
            break;
        case NONE:
            break;
    }
}

void MainViewModel::onGenerateButton() {
    LOGD << "User clicked generate button " + std::string(*user_state_);

    std::unique_ptr<FrameGenerator> frame_generator(user_state_->createGenerator());
    std::unique_ptr<VideoGenerator> video_generator = std::make_unique<VideoGenerator>(*frame_generator);
    video_generator->generate();

    LOGD << "User generated video";
}