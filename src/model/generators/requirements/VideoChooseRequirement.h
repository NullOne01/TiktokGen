#ifndef TIKTOKGEN_VIDEOCHOOSEREQUIREMENT_H
#define TIKTOKGEN_VIDEOCHOOSEREQUIREMENT_H

#include <string>
#include "GeneratorRequirement.h"
#include "imgui.h"
#include "imfilebrowser.h"

class VideoChooseRequirement : public GeneratorRequirement<std::string> {
public:
    VideoChooseRequirement(const std::string &name);

    void showView() override;

    std::string getValue() override;

private:
    std::string path_;

    ImGui::FileBrowser choose_video_dialog_;
};


#endif //TIKTOKGEN_VIDEOCHOOSEREQUIREMENT_H
