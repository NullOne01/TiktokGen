#include "FrameGenerator.h"
#include "NoneGenerator.h"
#include "TextVideoGenerator.h"
#include "TelegramRandomGenerator.h"

FrameGenerator::~FrameGenerator() = default;

FrameGenerator *FrameGenerator::createGenerator(int index) {
    switch (index) {
        case 0:
            return new NoneGenerator();
        case 1:
            return new TextVideoGenerator();
        case 2:
            return new TelegramRandomGenerator();
    }

    throw std::invalid_argument("Unknown generator");
}

void FrameGenerator::viewRequirements() {
    for (auto &requirement: requirements) {
        requirement->showView();
    }
}
