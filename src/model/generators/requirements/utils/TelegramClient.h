#ifndef TIKTOKGEN_TELEGRAMCLIENT_H
#define TIKTOKGEN_TELEGRAMCLIENT_H


#include <memory>
#include <map>
#include <td/telegram/td_api.h>
#include <td/telegram/Client.h>

class TelegramListenerThread;

class TelegramClient {
private:
    using Object = td::td_api::object_ptr<td::td_api::Object>;

    friend class TelegramListenerThread;


public:
    TelegramClient();

    std::unique_ptr<TelegramListenerThread> listener_thread_;

    std::atomic<bool> is_authorized_ = false;

    void send_query(td::td_api::object_ptr<td::td_api::Function> f, std::function<void(Object)> handler);

    void query_last_message(const std::string &chat_name, std::string *last_message);

    void start();


private:
    std::unique_ptr<td::ClientManager> client_manager_;

    td::td_api::object_ptr<td::td_api::AuthorizationState> authorization_state_;
    int authentication_query_id_ = 0;
    std::map<std::uint64_t, std::function<void(Object)>> handlers_;

    int client_id_ = 0;
    int current_query_id_ = 0;

    uint64_t next_query_id();
};


#endif //TIKTOKGEN_TELEGRAMCLIENT_H
