//
// Created by henry on 12/18/22.
//

#include "../../include/hagame/utils/string.h"

std::vector<std::string> hg::utils::s_split(std::string str, char delim) {
    std::vector<std::string> parts = {};
    std::string tmp = "";

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == delim) {
            parts.push_back(tmp);
            tmp = "";
        }
        else {
            tmp += str[i];
        }
    }

    if (tmp.length() > 0) {
        parts.push_back(tmp);
    }

    return parts;
}

std::string hg::utils::s_join(std::vector<std::string> strs, std::string delim) {
    std::string out = "";
    int size = strs.size();
    for (int i = 0; i < size; i++) {
        out += strs[i];
        if (i < size - 1) {
            out += delim;
        }
    }
    return out;
}