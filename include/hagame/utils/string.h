//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_STRING_H
#define HAGAME2_STRING_H

#include <vector>
#include <string>

namespace hg::utils {
    std::vector<std::string> s_split(std::string str, char delim);
    std::vector<std::string> s_partition(std::string str, char start, char end);
    std::string s_join(std::vector<std::string> strs, std::string delim);
    std::string s_trim(std::string str);
    std::string s_replace(std::string str, std::string replace, std::string with);
    bool s_startsWith(std::string str, std::string check);
    bool s_endsWith(std::string str, std::string check);
}

#endif //HAGAME2_STRING_H
