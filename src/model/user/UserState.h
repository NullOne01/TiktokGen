#ifndef TIKTOKGEN_USERSTATE_H
#define TIKTOKGEN_USERSTATE_H

#include <string>
#include <vector>
#include "../generators/frame_generators/FrameGenerator.h"
#include "../generators/frame_generators/TextVideoGenerator.h"
#include "../generators/frame_generators/NoneGenerator.h"
#include "../generators/frame_generators/TelegramRandomGenerator.h"
#include "ConfigRead.h"

class UserState {
public:
    int chosen_generator = 0;

    std::unique_ptr<FrameGenerator> generator;

    void createGenerator() {
        generator = std::unique_ptr<FrameGenerator>(FrameGenerator::createGenerator(chosen_generator));
    }

    const std::vector<std::string> generator_names = {
            NoneGenerator::name,
            TextVideoGenerator::name,
            TelegramRandomGenerator::name
    };

    explicit operator std::string() const {
        return "UserState(GeneratorName: " + generator_names[chosen_generator] + ")";
    }
};


#endif //TIKTOKGEN_USERSTATE_H
