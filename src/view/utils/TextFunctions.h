#ifndef TIKTOKGEN_TEXTFUNCTIONS_H
#define TIKTOKGEN_TEXTFUNCTIONS_H

#include <opencv2/imgproc.hpp>
#include <vector>
#include "../../view_model/utils/StringFunctions.h"

struct TextFunctions {
    static void putTextMultiline(cv::InputOutputArray img, const cv::String &text, cv::Point org,
                                 int fontFace, double fontScale, cv::Scalar color,
                                 double prefWidth,
                                 int thickness = 1, int lineType = cv::LINE_8) {
        int baseline = 0;
        cv::Size text_size = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);
        double width_per_char = text_size.width / text.size();
        int char_per_line = prefWidth / width_per_char;

        std::vector<std::string> lines = StringFunctions::getLinesFixedWidth(text, char_per_line);
        putTextMultiline(img, lines, org, fontFace, fontScale, color, thickness, lineType);
    }

    static void putTextMultiline(cv::InputOutputArray img, const std::vector<cv::String> &lines, cv::Point org,
                                 int fontFace, double fontScale, cv::Scalar color,
                                 int thickness = 1, int lineType = cv::LINE_8) {
        for (const auto &line: lines) {
            int baseline = 0;
            cv::Size text_size = cv::getTextSize(line, fontFace, fontScale, thickness, &baseline);
            cv::putText(img, //target image
                        line, //text
                        org, //top-left position
                        fontFace,
                        fontScale,
                        color, //font color
                        thickness, lineType);
            org += cv::Point(0, text_size.height);
        }
    }
};

#endif //TIKTOKGEN_TEXTFUNCTIONS_H
