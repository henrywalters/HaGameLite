//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_FILE_H
#define HAGAME2_FILE_H

#include <string>
#include <vector>

namespace hg::utils {

    std::string f_read(std::string file);
    std::vector<std::string> f_readLines(std::string file);
    void f_write(std::string file, std::string content);
    void f_writeLines(std::string file, std::vector<std::string> lines);
    void f_append(std::string file, std::string content);
    void f_appendLines(std::string file, std::vector<std::string> lines);


}

#endif //HAGAME2_FILE_H
