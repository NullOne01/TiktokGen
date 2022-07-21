#ifndef TIKTOKGEN_TEXTFUNCTIONS_H
#define TIKTOKGEN_TEXTFUNCTIONS_H

#include <opencv2/imgproc.hpp>
#include <vector>
#include <opencv2/freetype.hpp>
#include "../../view_model/utils/StringFunctions.h"

struct TextFunctions {
    static const int height_offset = 5;

    static void putTextMultiline(cv::InputOutputArray img, const cv::String &text, cv::Point org,
                                 int fontFace, double fontScale, cv::Scalar color,
                                 double prefWidth,
                                 int thickness = 1, int lineType = cv::LINE_8) {
        int baseline = 0;
        cv::Size text_size = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);
        double width_per_char = text_size.width / StringFunctions::utf8_length(text);
        int char_per_line = prefWidth / width_per_char;

        std::vector<std::string> lines = StringFunctions::getLinesFixedWidth(text, char_per_line);
        putTextMultiline(img, lines, org, fontFace, fontScale, color, thickness, lineType);
    }

    static void putTextMultiline(cv::Ptr<cv::freetype::FreeType2> ft2, cv::InputOutputArray img, const cv::String &text,
                                 cv::Point org,
                                 int fontHeight, cv::Scalar color,
                                 double prefWidth,
                                 int thickness = 1, int lineType = cv::LINE_8) {
        int baseline = 0;
        cv::Size text_size = ft2->getTextSize(text, fontHeight, thickness, &baseline);
        double width_per_char = text_size.width / StringFunctions::utf8_length(text);
        int char_per_line = prefWidth / width_per_char;

        std::vector<std::string> lines = StringFunctions::getLinesFixedWidth(text, char_per_line);
        putTextMultiline(ft2, img, lines, org, fontHeight, color, thickness, lineType);
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
            org += cv::Point(0, text_size.height + height_offset);
        }
    }

    static void putTextMultiline(cv::Ptr<cv::freetype::FreeType2> ft2, cv::InputOutputArray img,
                                 const std::vector<cv::String> &lines, cv::Point org,
                                 int fontHeight, cv::Scalar color,
                                 int thickness = 1, int lineType = cv::LINE_8) {
        for (const auto &line: lines) {
            int baseline = 0;
            cv::Size text_size = ft2->getTextSize(line, fontHeight, thickness, &baseline);
            ft2->putText(img, //target image
                         line,
                         org,
                         fontHeight,
                         color,
                         thickness,
                         lineType,
                         false);
            org += cv::Point(0, text_size.height + height_offset);
        }
    }

    static cv::Size getSizeOfLines(const std::vector<cv::String> &lines,
                                   int fontFace, double fontScale, cv::Scalar color,
                                   int thickness = 1, int lineType = cv::LINE_8) {
        int baseline = 0;
        int max_width = cv::getTextSize(lines[0], fontFace, fontScale, thickness, &baseline).width;
        int max_height = 0;
        for (const auto &line: lines) {
            cv::Size text_size = cv::getTextSize(line, fontFace, fontScale, thickness, &baseline);
            max_height += text_size.height;
            max_width = std::max(max_width, text_size.width);
        }

        return {max_width, max_height};
    }

    static cv::Size getSizeOfLines(cv::Ptr<cv::freetype::FreeType2> ft2, const std::vector<cv::String> &lines,
                                   int fontHeight, cv::Scalar color,
                                   int thickness = 1, int lineType = cv::LINE_8) {
        int baseline = 0;
        int max_width = ft2->getTextSize(lines[0], fontHeight, thickness, &baseline).width;
        int max_height = 0;
        for (const auto &line: lines) {
            cv::Size text_size = ft2->getTextSize(line, fontHeight, thickness, &baseline);
            max_height += text_size.height + height_offset;
            max_width = std::max(max_width, text_size.width);
        }

        return {max_width, max_height};
    }

    static cv::Size getSizeOfLines(const cv::String &big_line, double prefWidth,
                                   int fontFace, double fontScale, cv::Scalar color,
                                   int thickness = 1, int lineType = cv::LINE_8) {
        int baseline = 0;
        cv::Size text_size = cv::getTextSize(big_line, fontFace, fontScale, thickness, &baseline);
        double width_per_char = text_size.width / StringFunctions::utf8_length(big_line);
        int char_per_line = prefWidth / width_per_char;

        std::vector<std::string> lines = StringFunctions::getLinesFixedWidth(big_line, char_per_line);
        return getSizeOfLines(lines, fontFace, fontScale, color, thickness, lineType);
    }

    static cv::Size getSizeOfLines(cv::Ptr<cv::freetype::FreeType2> ft2, const cv::String &big_line, double prefWidth,
                                   int fontHeight, cv::Scalar color,
                                   int thickness = 1, int lineType = cv::LINE_8) {
        int baseline = 0;
        cv::Size text_size = ft2->getTextSize(big_line, fontHeight, thickness, &baseline);
        double width_per_char = text_size.width / StringFunctions::utf8_length(big_line);
        int char_per_line = prefWidth / width_per_char;

        std::vector<std::string> lines = StringFunctions::getLinesFixedWidth(big_line, char_per_line);
        return getSizeOfLines(ft2, lines, fontHeight, color, thickness, lineType);
    }

    static int getBestFontHeight(cv::Ptr<cv::freetype::FreeType2> ft2, cv::InputOutputArray img, const cv::String &text,
                                 double prefHeight, double prefWidth, int thickness = 1) {
        int font_size;
        for (font_size = 60; font_size > 1; font_size--) {
            cv::Size multiline_size = TextFunctions::getSizeOfLines(ft2, text, prefWidth, font_size,
                                                                    cv::Scalar(255, 255, 255, 255),
                                                                    thickness);
            if (multiline_size.height <= prefHeight) {
                break;
            }
        }

        return font_size;
    }

    static void putTextMultiline(cv::Ptr<cv::freetype::FreeType2> ft2, cv::InputOutputArray img, const cv::String &text,
                                 cv::Point org, cv::Scalar color,
                                 double prefHeight, double prefWidth,
                                 int thickness = 1, int lineType = cv::LINE_8) {
        int font_size = getBestFontHeight(ft2, img, text, prefHeight, prefWidth, thickness);
        putTextMultiline(ft2, img, text, org, font_size, color, prefWidth, thickness, lineType);
    }
};

#endif //TIKTOKGEN_TEXTFUNCTIONS_H
