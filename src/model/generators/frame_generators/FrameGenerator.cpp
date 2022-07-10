#include "FrameGenerator.h"
#include "NoneGenerator.h"
#include "TextVideoGenerator.h"

FrameGenerator::~FrameGenerator() = default;

FrameGenerator *FrameGenerator::createGenerator(int index) {
    switch (index) {
        case 0:
            return new NoneGenerator();
        case 1:
            return new TextVideoGenerator();
    }

    throw std::invalid_argument("Unknown generator");
}

void FrameGenerator::viewRequirements() {
    for (auto &requirement: requirements) {
        requirement->showView();
    }
}
