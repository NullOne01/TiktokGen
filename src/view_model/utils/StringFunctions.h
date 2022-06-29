#ifndef TIKTOKGEN_STRINGFUNCTIONS_H
#define TIKTOKGEN_STRINGFUNCTIONS_H

#include <string>
#include <vector>

struct StringFunctions {
    static std::string getTextFixedWidth(const std::string &base_text, int max_char_line) {
        std::vector<std::string> lines = getLinesFixedWidth(base_text, max_char_line);
        std::string res;
        for (auto &line: lines) {
            res += line + '\n';
        }

        return res;
    }

    static std::vector<std::string> getLinesFixedWidth(const std::string &base_text, int max_char_line) {
        std::vector<std::string> words = split(base_text, " ");
        std::vector<std::string> res;
        std::string new_word;
        for (auto &word: words) {
            if (new_word.size() + word.size() > max_char_line) {
                res.push_back(new_word);
                new_word = "";
            }

            new_word += word + " ";
        }
        res.push_back(new_word);

        return res;
    }

    /**
     * Simple string split implementation. Should be replaced with boost.
     * @param base_str
     * @param delimiter
     * @return
     */
    static std::vector<std::string> split(const std::string &base_str, const std::string &delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = base_str.find(delimiter, pos_start)) != std::string::npos) {
            token = base_str.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back(base_str.substr(pos_start));
        return res;
    }
};

#endif //TIKTOKGEN_STRINGFUNCTIONS_H