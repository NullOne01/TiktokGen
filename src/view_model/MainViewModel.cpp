#include "MainViewModel.h"
#include "plog/Log.h"
#include "utils/LogFunctions.h"

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
    for (const auto &requirement_vec: user_state_->generator_requirements) {
        for (int i = 0; i < requirement_vec.size(); ++i) {
            bindRequirement(requirement_vec[i], i);
        }
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
}