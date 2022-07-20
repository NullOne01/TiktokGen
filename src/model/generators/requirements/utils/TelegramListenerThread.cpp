#include "TelegramListenerThread.h"
#include "imgui.h"
#include "../../../user/Config.h"
#include "plog/Log.h"
#include <imgui_stdlib.h>
#include <iostream>

TelegramListenerThread::TelegramListenerThread(TelegramClient *telegram_client) : telegram_client_(telegram_client) {}

void TelegramListenerThread::run() {
    try {
        ThreadWrapper::run();

        while (true) {
            process_response(telegram_client_->client_manager_->receive(10));
        }
    } catch (const std::exception &e) {
        PLOG_ERROR << e.what();
    }
}

void TelegramListenerThread::process_response(td::ClientManager::Response response) {
    if (!response.object) {
        return;
    }

    //std::cout << response.request_id << " " << to_string(response.object) << std::endl;
    if (response.request_id == 0) {
        return process_update(std::move(response.object));
    }
    auto it = telegram_client_->handlers_.find(response.request_id);
    if (it != telegram_client_->handlers_.end()) {
        it->second(std::move(response.object));
        telegram_client_->handlers_.erase(it);
    }
}

void TelegramListenerThread::process_update(td::td_api::object_ptr<td::td_api::Object> update) {
    td::td_api::downcast_call(
            *update, overloaded(
                    [this](td::td_api::updateAuthorizationState &update_authorization_state) {
                        telegram_client_->authorization_state_ = std::move(
                                update_authorization_state.authorization_state_);
                        on_authorization_state_update();
                    },
                    [](auto &update) {}));
}

void TelegramListenerThread::on_authorization_state_update() {
    telegram_client_->authentication_query_id_++;
    td::td_api::downcast_call(
            *telegram_client_->authorization_state_,
            overloaded(
                    [this](td::td_api::authorizationStateReady &) {
                        telegram_client_->is_authorized_ = true;
                        func_ui = []() {
                            ImGui::Text("Got authorization");
                        };
                    },
                    [this](td::td_api::authorizationStateLoggingOut &) {
                        telegram_client_->is_authorized_ = false;
                        func_ui = []() {
                            ImGui::Text("Logging out");
                        };
                    },
                    [this](td::td_api::authorizationStateClosing &) {
                        func_ui = []() {
                            ImGui::Text("Closing");
                        };
                    },
                    [this](td::td_api::authorizationStateClosed &) {
                        telegram_client_->is_authorized_ = false;
                        func_ui = []() {
                            ImGui::Text("Terminated");
                        };
                    },
                    [this](td::td_api::authorizationStateWaitCode &) {
                        func_ui = [this]() {
                            ImGui::Text("Enter auth code:");
                            ImGui::InputText("Code", &code);
                            if (ImGui::Button("Enter")) {
                                telegram_client_->send_query(
                                        td::td_api::make_object<td::td_api::checkAuthenticationCode>(code),
                                        create_authentication_query_handler());
                            }
                        };
                    },
                    [this](td::td_api::authorizationStateWaitRegistration &) {
                        func_ui = [this]() {
                            ImGui::Text("Enter your first name: ");
                            ImGui::InputText("First name", &first_name);
                            ImGui::Text("Enter your last name: ");
                            ImGui::InputText("Last name", &last_name);
                            if (ImGui::Button("Enter")) {
                                telegram_client_->send_query(
                                        td::td_api::make_object<td::td_api::registerUser>(first_name, last_name),
                                        create_authentication_query_handler());
                            }
                        };
                    },
                    [this](td::td_api::authorizationStateWaitPassword &) {
                        func_ui = [this]() {
                            ImGui::Text("Enter authentication password:");
                            ImGui::InputText("Auth password", &auth_password);
                            if (ImGui::Button("Enter")) {
                                telegram_client_->send_query(
                                        td::td_api::make_object<td::td_api::checkAuthenticationPassword>(auth_password),
                                        create_authentication_query_handler());
                            }
                        };
                    },
                    [this](td::td_api::authorizationStateWaitOtherDeviceConfirmation &state) {
                        func_ui = [&state]() {
                            ImGui::Text("Confirm this login link on another device: %s", state.link_.c_str());
                        };
                    },
                    [this](td::td_api::authorizationStateWaitPhoneNumber &) {
                        func_ui = [this]() {
                            ImGui::Text("Enter phone number:");
                            ImGui::InputText("Text", &phone);
                            if (ImGui::Button("Enter")) {
                                telegram_client_->send_query(
                                        td::td_api::make_object<td::td_api::setAuthenticationPhoneNumber>(phone,
                                                                                                          nullptr),
                                        create_authentication_query_handler());
                            }
                        };
                    },
                    [this](td::td_api::authorizationStateWaitEncryptionKey &) {
                        func_ui = [this]() {
                            ImGui::Text("Enter encryption key or DESTROY:");
                            ImGui::InputText("Key (Or DESTROY)", &enc_key);
                            if (ImGui::Button("Enter")) {
                                if (enc_key == "DESTROY") {
                                    telegram_client_->send_query(td::td_api::make_object<td::td_api::destroy>(),
                                                                 create_authentication_query_handler());
                                } else {
                                    telegram_client_->send_query(
                                            td::td_api::make_object<td::td_api::checkDatabaseEncryptionKey>(
                                                    std::move(enc_key)),
                                            create_authentication_query_handler());
                                }
                            }
                        };
                    },
                    [this](td::td_api::authorizationStateWaitTdlibParameters &) {
                        auto parameters = td::td_api::make_object<td::td_api::tdlibParameters>();
                        parameters->database_directory_ = "tdlib";
                        parameters->use_message_database_ = true;
                        parameters->use_secret_chats_ = true;
                        parameters->api_id_ = std::stoi(Config::instance()->getValue("api_id"));
                        parameters->api_hash_ = Config::instance()->getValue("api_hash");
                        parameters->system_language_code_ = "en";
                        parameters->device_model_ = "Desktop";
                        parameters->application_version_ = "1.0";
                        parameters->enable_storage_optimizer_ = true;
                        telegram_client_->send_query(
                                td::td_api::make_object<td::td_api::setTdlibParameters>(std::move(parameters)),
                                create_authentication_query_handler());
                    }));
}

void TelegramListenerThread::check_authentication_error(TelegramListenerThread::Object object) {
    if (object->get_id() == td::td_api::error::ID) {
        auto error = td::move_tl_object_as<td::td_api::error>(object);
        std::string error_msg = to_string(error);
        func_error_ui = [error_msg]() {
            ImGui::Text("Error: %s", error_msg.c_str());
        };

        on_authorization_state_update();
    }
}

std::function<void(td::tl::unique_ptr<td::td_api::Object>)>
TelegramListenerThread::create_authentication_query_handler() {
    return [this, id = telegram_client_->authentication_query_id_](Object object) {
        if (id == telegram_client_->authentication_query_id_) {
            check_authentication_error(std::move(object));
        }
    };
}
