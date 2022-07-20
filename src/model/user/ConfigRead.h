#ifndef TIKTOKGEN_CONFIGREAD_H
#define TIKTOKGEN_CONFIGREAD_H

#include <string>
#include <map>

class ConfigRead {
public:
    ConfigRead(const std::string &path);

    ConfigRead();

    void parseFile(const std::string &path);

    std::string getValue(const std::string &key);

private:
    std::map<std::string, std::string> values_;
};


#endif //TIKTOKGEN_CONFIGREAD_H
