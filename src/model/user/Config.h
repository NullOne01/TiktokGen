#ifndef TIKTOKGEN_CONFIG_H
#define TIKTOKGEN_CONFIG_H


#include <memory>
#include "ConfigRead.h"

class Config {
public:
    static void init(const std::string &path);

    static ConfigRead *instance();

private:
    inline static std::unique_ptr<ConfigRead> instance_;
};


#endif //TIKTOKGEN_CONFIG_H
