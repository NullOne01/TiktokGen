#include "ConfigRead.h"
#include <fstream>

void ConfigRead::parseFile(const std::string &path) {
    std::ifstream my_file(path);

    if (!my_file.is_open()) {
        std::throw_with_nested(std::runtime_error("Can't open config file: " + path));
    }

    std::string key, value;
    while (my_file >> key >> value) {
        values_[key] = value;
    }

    my_file.close();
}

std::string ConfigRead::getValue(const std::string &key) {
    if (!values_.contains(key)) {
        return "NULL";
    }

    return values_[key];
}

ConfigRead::ConfigRead(const std::string &path) {
    parseFile(path);
}

ConfigRead::ConfigRead() {
}
