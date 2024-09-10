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

    //if (tmp.length() > 0) {
        parts.push_back(tmp);
    //}

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

std::vector<std::string> hg::utils::s_partition(std::string str, char start, char end) {
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

std::string hg::utils::s_replace(std::string str, std::string replace, std::string with) {
    std::string out;

    size_t i = 0;
    while (i < str.size()) {
        // Find the next occurrence of the pattern
        size_t found = str.find(replace, i);

        if (found != std::string::npos) {
            // Append the characters between the current position and the match
            out += str.substr(i, found - i);

            // Append the replacement
            out += with;

            // Move the current position past the matched pattern
            i = found + replace.size();
        } else {
            // No more occurrences, append the remaining characters
            out += str.substr(i);
            break;
        }
    }

    return out;
}
