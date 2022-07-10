#ifndef TIKTOKGEN_MAINVIEWMODEL_H
#define TIKTOKGEN_MAINVIEWMODEL_H

#include <memory>
#include "../view/MainView.h"
#include "../model/user/UserState.h"

class MainViewModel {
public:
    explicit MainViewModel(MainView &view);

    void onFrame();

private:
    MainView &view_;

    std::unique_ptr<UserState> user_state_ = std::make_unique<UserState>();

    std::string last_read_log_ = "";

    void showMainWindow();

    void showLogWindow();

    void showRequirementWindows();

    void onGenerateButton();

    void onSelectGenerator();
};


#endif //TIKTOKGEN_MAINVIEWMODEL_H
