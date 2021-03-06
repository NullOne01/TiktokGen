#ifndef TIKTOKGEN_MAINVIEW_H
#define TIKTOKGEN_MAINVIEW_H

#include <string>
#include <vector>
#include "Renderer.h"
#include "imfilebrowser.h"
#include "opencv2/core/mat.hpp"

class MainView {
public:
    explicit MainView(const Renderer &renderer);

    void logWindow(const std::string &log) const;

    void matWindow(const cv::Mat &image) const;

    void chooseGeneratorWindow(int *generator_num, const std::vector<std::string> &items,
                               const std::function<void(void)> &generate_button_callback,
                               const std::function<void(void)> &generator_select_callback) const;

private:
    const Renderer &renderer_;
};


#endif //TIKTOKGEN_MAINVIEW_H
