//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_FILE_H
#define HAGAME2_FILE_H

#include <string>
#include <vector>

namespace hg::utils {

    struct FileBuffer {
        size_t size;
        unsigned char* data;
    };

    struct FileParts {
        std::string path;
        std::string fullName;
        std::string name;
        std::string extension;
        bool isDir;
    };

    bool f_exists(std::string file);
    std::string f_read(std::string file);
    std::vector<std::string> f_readLines(std::string file);
    FileBuffer f_readToBuffer(std::string file);
    void f_write(std::string file, std::string content);
    void f_writeLines(std::string file, std::vector<std::string> lines);
    void f_append(std::string file, std::string content);
    void f_appendLines(std::string file, std::vector<std::string> lines);
    FileParts f_getParts(std::string file);

    std::vector<std::string> d_listFiles(std::string path, bool recursive = false);
    std::vector<std::string> d_listDirs(std::string path);


}

#endif //HAGAME2_FILE_H
