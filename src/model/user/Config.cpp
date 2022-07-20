#include "Config.h"

void Config::init(const std::string &path) {
    instance_ = std::make_unique<ConfigRead>(path);
}

ConfigRead *Config::instance() {
    return instance_.get();
}


