#include "TelegramRandomRequirement.h"
#include "../../user/Config.h"
#include "imgui.h"
#include <iostream>

TelegramRandomRequirement::TelegramRandomRequirement(const std::string &name)
        : GeneratorRequirement(name) {
    telegram_client_ = std::make_unique<TelegramClient>();
}

void TelegramRandomRequirement::showView() {
    std::string window_name = "Telegram random post. Parameter: " + name;
    ImGui::Begin(window_name.c_str());
    if (ImGui::Button("Start")) {
        start();
    }

    if (telegram_client_->listener_thread_ != nullptr) {
        if (telegram_client_->listener_thread_->func_ui) {
            telegram_client_->listener_thread_->func_ui();
        }

        if (telegram_client_->listener_thread_->func_error_ui) {
            telegram_client_->listener_thread_->func_error_ui();
        }
    }

    ImGui::Text("Your auth: %s", telegram_client_->is_authorized_ ? "yes" : "no");

    if (telegram_client_->is_authorized_) {
        if (ImGui::Button("Get post")) {
            getPost();
        }

        ImGui::Text("Your post: %s", post_text_.c_str());
    }
    ImGui::End();

    uiLoop();
}

void TelegramRandomRequirement::uiLoop() {

}

void TelegramRandomRequirement::start() {
    telegram_client_->start();
}

void TelegramRandomRequirement::getPost() {
    // example chat
    telegram_client_->query_last_message("myfavoritejumoreski", &post_text_);
}

std::string TelegramRandomRequirement::getValue() {
    return post_text_;
}
