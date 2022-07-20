#ifndef TIKTOKGEN_TELEGRAMRANDOMREQUIREMENT_H
#define TIKTOKGEN_TELEGRAMRANDOMREQUIREMENT_H

#include <string>
#include <map>
#include "GeneratorRequirement.h"
#include "td/telegram/Client.h"
#include "td/telegram/td_api.h"
#include "td/telegram/td_api.hpp"
#include "utils/TelegramListenerThread.h"
#include <iostream>

class TelegramRandomRequirement : public GeneratorRequirement<std::string> {
public:
    explicit TelegramRandomRequirement(const std::string &name);

    void showView() override;

    std::string getValue() override;


private:
    std::unique_ptr<TelegramClient> telegram_client_;

    void start();

    void getPost();

    std::string post_text_;

    void uiLoop();
};


#endif //TIKTOKGEN_TELEGRAMRANDOMREQUIREMENT_H
