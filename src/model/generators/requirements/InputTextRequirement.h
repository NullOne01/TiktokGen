#ifndef TIKTOKGEN_INPUTTEXTREQUIREMENT_H
#define TIKTOKGEN_INPUTTEXTREQUIREMENT_H

#include "GeneratorRequirement.h"

class InputTextRequirement : public GeneratorRequirement<std::string> {
public:
    void showView() override;

    std::string getValue() override;

    InputTextRequirement(const std::string &name);

private:
    std::string text_;
};


#endif //TIKTOKGEN_INPUTTEXTREQUIREMENT_H
