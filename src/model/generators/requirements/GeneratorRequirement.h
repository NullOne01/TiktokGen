#ifndef TIKTOKGEN_GENERATORREQUIREMENT_H
#define TIKTOKGEN_GENERATORREQUIREMENT_H

#include <string>
#include "IGeneratorRequirement.h"

template<typename T>
class GeneratorRequirement : public IGeneratorRequirement {
public:
    std::string name = "NONE";

    explicit GeneratorRequirement(std::string name) : name(std::move(name)) {}

    virtual void showView() {}

    virtual T getValue() {
        return T();
    }
};


#endif //TIKTOKGEN_GENERATORREQUIREMENT_H
