#include "TelegramClient.h"
#include "TelegramListenerThread.h"


void TelegramClient::send_query(td::td_api::object_ptr<td::td_api::Function> f, std::function<void(Object)> handler) {
    auto query_id = next_query_id();
    if (handler) {
        handlers_.emplace(query_id, std::move(handler));
    }
    client_manager_->send(client_id_, query_id, std::move(f));
}

std::uint64_t TelegramClient::next_query_id() {
    return ++current_query_id_;
}

TelegramClient::TelegramClient() {
    td::ClientManager::execute(td::td_api::make_object<td::td_api::setLogVerbosityLevel>(1));
    client_manager_ = std::make_unique<td::ClientManager>();
    client_id_ = client_manager_->create_client_id();
}

void TelegramClient::start() {
    send_query(td::td_api::make_object<td::td_api::getOption>("version"), {});

    listener_thread_ = std::make_unique<TelegramListenerThread>(this);
    listener_thread_->start();
}

void TelegramClient::query_last_message(const std::string &chat_name, std::string *last_message) {
    send_query(td::td_api::make_object<td::td_api::searchPublicChat>(chat_name), [this, last_message](Object object) {
        auto chat_found = td::move_tl_object_as<td::td_api::chat>(object);
        int64_t chat_id = chat_found->id_;

        int message_pool_size = 99;
        send_query(
                td::td_api::make_object<td::td_api::getChatHistory>(chat_id, 0, 0, message_pool_size,
                                                                    false),
                [last_message, message_pool_size](Object object) {
                    auto messages = td::move_tl_object_as<td::td_api::messages>(object);
                    // TODO: this line can show errors, if message is not message text.
                    for (int i = 0; i < messages->total_count_; ++i) {
                        if (messages->messages_[i]->content_->get_id() != td::td_api::messageText::ID) {
                            continue;
                        }

                        auto message_text = td::move_tl_object_as<td::td_api::messageText>(
                                messages->messages_[i]->content_);
                        *last_message = message_text->text_->text_;
                        break;
                    }
                }
        );
    });
}
