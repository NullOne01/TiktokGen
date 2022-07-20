#ifndef TIKTOKGEN_TELEGRAMRANDOMGENERATOR_H
#define TIKTOKGEN_TELEGRAMRANDOMGENERATOR_H

#include "FrameGenerator.h"

class TelegramRandomGenerator : public FrameGenerator {
public:
    TelegramRandomGenerator();

    ~TelegramRandomGenerator() override;

    Generator<cv::Mat> frameGenerator() override;

    void loadData() override;

    inline const static std::string name = "TelegramRandomGenerator";

private:
    std::string path_to_video_;
    std::string telegram_text_;
};


#endif //TIKTOKGEN_TELEGRAMRANDOMGENERATOR_H
