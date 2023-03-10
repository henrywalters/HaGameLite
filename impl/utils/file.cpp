//
// Created by henry on 12/18/22.
//

#include <fstream>
#include <streambuf>

#include "../../include/hagame/utils/file.h"
#include "../../include/hagame/utils/string.h"


std::string hg::utils::f_read(std::string file) {
    std::ifstream stream(file);
    auto str = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
    stream.close();
    return str;
}

std::vector<std::string> hg::utils::f_readLines(std::string file) {
    return s_split(f_read(file), '\n');
}

hg::utils::FileBuffer hg::utils::f_readToBuffer(std::string file) {
    FileBuffer buffer;

    FILE* f = fopen(file.c_str(), "rb");
    fseek(f, 0, SEEK_END);
    buffer.size = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer.data = (unsigned char*) malloc(buffer.size);

    fread(buffer.data, buffer.size, 1, f);
    fclose(f);

    return buffer;
}

void hg::utils::f_write(std::string file, std::string content) {
    std::ofstream stream(file, std::ios::trunc);
    stream << content;
    stream.close();
}

void hg::utils::f_writeLines(std::string file, std::vector<std::string> lines) {
    f_write(file, s_join(lines, "\n"));
}

void hg::utils::f_append(std::string file, std::string content) {
    std::ofstream stream(file, std::ios::app);
    stream << content;
    stream.close();
}

void hg::utils::f_appendLines(std::string file, std::vector<std::string> lines) {
    f_append(file, s_join(lines, "\n"));
}