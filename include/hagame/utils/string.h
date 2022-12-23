//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_STRING_H
#define HAGAME2_STRING_H

#include <vector>
#include <string>

namespace hg::utils {
    std::vector<std::string> s_split(std::string str, char delim);
    std::string s_join(std::vector<std::string> strs, std::string delim);

}

#endif //HAGAME2_STRING_H
