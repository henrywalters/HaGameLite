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

std::string hg::utils::s_trim(std::string str) {
    std::string out = "";
    bool foundNonWhiteSpace = false;
    int lastWhiteSpace = 0;
    int lastNonWhiteSpace = 0;

    for (int i = 0; i < str.size(); i++) {
        if (!foundNonWhiteSpace) {
            if (str[i] != ' ') {
                foundNonWhiteSpace = true;
                lastWhiteSpace = i;
                out += str[i];
            }
        }
        else {
            if (str[i] != ' ') {
                lastNonWhiteSpace = i;
            }
        }
    }

    for (int i = lastWhiteSpace + 1; i <= lastNonWhiteSpace; i++) {
        out += str[i];
    }

    return out;
}

std::vector<std::string> s_partition(std::string str, char start, char end) {
    std::vector<std::string> partitions;
    std::string part = "";
    bool inPart = false;

    for (int i = 0; i < str.size(); i++) {
        auto c = str[i];
        if (c == start) {
            part = "";
            inPart = true;
        }
        else if (c == end && inPart) {
            partitions.push_back(part);
            inPart = false;
        }
        else {
            part += c;
        }
    }

    return partitions;
}