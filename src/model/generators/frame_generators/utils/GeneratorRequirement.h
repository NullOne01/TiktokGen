#ifndef TIKTOKGEN_GENERATORREQUIREMENT_H
#define TIKTOKGEN_GENERATORREQUIREMENT_H

#include <string>
#include <utility>
#include "GeneratorRequirementType.h"

struct GeneratorRequirement {
    std::string name = "NONE";
    GeneratorRequirementType type = NONE;

    GeneratorRequirement(std::string name, GeneratorRequirementType type) : name(std::move(name)), type(type) {}
};

#endif //TIKTOKGEN_GENERATORREQUIREMENT_H
