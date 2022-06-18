#ifndef TIKTOKGEN_LOGFUNCTIONS_H
#define TIKTOKGEN_LOGFUNCTIONS_H

#include <string>
#include <fstream>
#include <sstream>
#include "plog/Log.h"

struct LogFunctions {
    static std::string getNewLogs() {
        std::ifstream t("DebugLog.txt");
        std::stringstream buffer;
        buffer << t.rdbuf();

        return buffer.str();
    }
};

#endif //TIKTOKGEN_LOGFUNCTIONS_H
