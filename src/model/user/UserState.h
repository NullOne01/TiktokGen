#ifndef TIKTOKGEN_USERSTATE_H
#define TIKTOKGEN_USERSTATE_H

#include <string>
#include <vector>
#include "../generators/frame_generators/utils/GeneratorRequirementType.h"
#include "../generators/frame_generators/FrameGenerator.h"
#include "../generators/frame_generators/TextVideoGenerator.h"
#include "../generators/frame_generators/NoneGenerator.h"

class UserState {
public:
    int chosen_generator = 0;

    std::vector<std::string> data_ = std::vector<std::string>(5);

    FrameGenerator *createGenerator() {
        switch (chosen_generator) {
            case 0:
                return new NoneGenerator();
            case 1:
                return new TextVideoGenerator(data_[0], data_[1]);
        }

        throw std::invalid_argument("Unknown generator");
    }

    const std::vector<std::vector<GeneratorRequirement>> generator_requirements = {
            NoneGenerator::requirements,
            TextVideoGenerator::requirements
    };

    const std::vector<std::string> generator_names = {
            NoneGenerator::name,
            TextVideoGenerator::name
    };

    explicit operator std::string() const {
        return "UserState(GeneratorName: " + generator_names[chosen_generator] + ")";
    }
};


#endif //TIKTOKGEN_USERSTATE_H
