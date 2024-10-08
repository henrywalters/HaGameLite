//
// Created by henry on 12/18/22.
//

#include <fstream>
#include <streambuf>
#include <filesystem>

#include "../../include/hagame/utils/file.h"
#include "../../include/hagame/utils/string.h"

bool hg::utils::f_exists(std::string file) {
    return std::filesystem::exists(file);
}

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

hg::utils::FileParts hg::utils::f_getParts(std::string file) {
    FileParts parts;
    parts.isDir = std::filesystem::is_directory(file);

    auto pathParts = utils::s_split(file, '/');
    parts.fullName = pathParts[pathParts.size() - 1];
    parts.path = "";

    for (int i = 0; i < pathParts.size() - 1; i++) {
        parts.path += pathParts[i] + "/";
    }

    if (parts.isDir) {
        parts.extension = "";
        parts.name = parts.fullName;
    } else {
        auto lastDot = parts.fullName.find_last_of('.');
        if (lastDot == std::string::npos) {
            parts.extension = "";
            parts.name = parts.fullName;
        } else {
            parts.name = parts.fullName.substr(0, lastDot);
            parts.extension = parts.fullName.substr(lastDot + 1, parts.fullName.size());
        }
    }

    return parts;
}

std::vector<std::string> hg::utils::d_listDirs(std::string path) {
    std::vector<std::string> out;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            out.push_back(entry.path().generic_string());
        }
    }
    return out;
}

std::vector<std::string> hg::utils::d_listFiles(std::string path, bool recursive) {
    std::vector<std::string> out;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        auto path = entry.path().generic_string();
        if (!entry.is_directory()) {
            out.push_back(path);
        } else if (recursive) {
            auto dirFiles = d_listFiles(path, true);
            out.insert(out.end(), dirFiles.begin(), dirFiles.end());
        }
    }
    return out;
}