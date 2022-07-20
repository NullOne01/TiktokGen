#ifndef TIKTOKGEN_TELEGRAMLISTENERTHREAD_H
#define TIKTOKGEN_TELEGRAMLISTENERTHREAD_H


#include <functional>
#include <thread>
#include <map>
#include <td/telegram/Client.h>
#include "ThreadWrapper.h"
#include "TelegramClient.h"
#include <iostream>


namespace detail {
    template<class... Fs>
    struct overload;

    template<class F>
    struct overload<F> : public F {
        explicit overload(F f) : F(f) {
        }
    };

    template<class F, class... Fs>
    struct overload<F, Fs...>
            : public overload<F>, public overload<Fs...> {
        overload(F f, Fs... fs) : overload<F>(f), overload<Fs...>(fs...) {
        }

        using overload<F>::operator();
        using overload<Fs...>::operator();
    };
}  // namespace detail

template<class... F>
auto overloaded(F... f) {
    return detail::overload<F...>(f...);
}

class TelegramListenerThread : public ThreadWrapper {
public:
    std::function<void()> func_ui;

    std::function<void()> func_error_ui;

    explicit TelegramListenerThread(TelegramClient *telegram_client);

private:
    using Object = td::td_api::object_ptr<td::td_api::Object>;

    TelegramClient *telegram_client_;

    // auth fields
    std::string phone, enc_key, auth_password, first_name, last_name, code;

    void process_response(td::ClientManager::Response response);

    void process_update(td::td_api::object_ptr<td::td_api::Object> update);

    void on_authorization_state_update();

    void check_authentication_error(Object object);

    std::function<void(td::tl::unique_ptr<td::td_api::Object>)> create_authentication_query_handler();

    void run() override;
};


#endif //TIKTOKGEN_TELEGRAMLISTENERTHREAD_H
