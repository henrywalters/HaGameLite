//
// Created by henry on 2/13/23.
//

#include "../../include/hagame/utils/config.h"
#include "../../include/hagame/utils/file.h"
#include "../../include/hagame/utils/string.h"

using namespace hg::utils;

bool Config::hasSection(std::string section) const {
    return m_data.find(section) != m_data.end();
}

bool Config::has(std::string section, std::string key) const {
    return hasSection(section) && m_data.at(section).find(key) != m_data.at(section).end();
}

std::vector<std::string> Config::sections() {
    std::vector<std::string> sections;
    for (auto& [section, data] : m_data) {
        sections.push_back(section);
    }
    return sections;
}

void Config::addSection(std::string section) {
    if (hasSection(section)) return;
    m_data.insert(std::make_pair(section, std::unordered_map<std::string, std::string>()));
}

Config Config::Parse(std::vector<std::string> lines) {
    hg::utils::ConfigData parsed;
    std::string sectionName;
    for (auto line : lines) {
        if (line[0] == '[' && line[line.size() - 1] == ']') {
            sectionName = line.substr(1, line.size() - 2);
            if (parsed.find(sectionName) == parsed.end()) {
                parsed.insert(std::make_pair(sectionName, std::unordered_map<std::string, std::string>()));
            }
        }
        else if (line != "") {

            auto parts = s_split(line, '=');

            std::string rawValue;

            if (parts.size() == 1) {
                rawValue = "";
            } else if (parts.size() == 2) {
                rawValue = parts[1];
            } else {
                std::cout << "Invalid config line: " << line << "\n";
                throw std::runtime_error("Invalid configuration file");
            }

            if (parsed[sectionName].find(parts[0]) != parsed[sectionName].end()) {
                std::cout << "Duplicate key found in section: " << sectionName << ", " << parts[0] << "\n";
                throw std::runtime_error("INvalid configuration file");
            }

            parsed[sectionName].insert(std::make_pair(parts[0], rawValue));
        }
    }
    return Config(parsed);
}

std::vector<std::string> Config::keys(std::string section) {
    std::vector<std::string> keys;
    for (const auto &[key, value] : m_data[section]) {
        keys.push_back(key);
    }
    return keys;
}

std::string Config::toString() const {
    std::vector<std::string> out;

    for (const auto &[sectionName, section] : m_data) {
        out.push_back("[" + sectionName + "]");
        for (const auto& [key, value] : section) {
            out.push_back(key + "=" + value);
        }
        out.push_back("\n");
    }

    return s_join(out, "\n");
}

void Config::concat(Config config) {
    for (const auto& section : config.sections()) {
        addSection(section);

        for (const auto& [key, data] : config.m_data[section]) {
            config.setRaw(section, key, data);
        }
    }
}

void Config::clear() {
    m_data.clear();
}

std::string MultiConfig::toString() const {
    auto lines = std::vector<std::string>();
    for (auto [name, page] : m_configs) {
        lines.push_back("~" + name + "~\n");
        lines.push_back(page.toString());
    }

    return s_join(lines, "\n");
}

MultiConfig MultiConfig::Parse(std::string file) {

    if (!f_exists(file)) {
        throw std::runtime_error("File: " + file + " does not exist");
    }

    hg::utils::MultiConfig multiConfig;
    std::unordered_map<std::string, std::vector<std::string>> pages;

    std::string pageName = "";
    std::vector<std::string> page;

    for (auto line : f_readLines(file)) {
        line = s_trim(line);
        if (line[0] == '~' && line[line.size() - 1] == '~') {
            if (pageName != "") {
                pages.insert(std::make_pair(pageName, page));
            }
            page.clear();
            pageName = line.substr(1, line.size() - 2);
            if (pageName == "") {
                throw std::runtime_error("Page name must be non-empty");
            }

        }
        else if (line != "") {
            if (pageName == "") {
                throw std::runtime_error("Encountered section before page was declared");
            }
            page.push_back(line);
        }
    }

    if (pageName != "") {
        pages.insert(std::make_pair(pageName, page));
    }

    for (auto& [name, page] : pages) {
        multiConfig.addPage(name, Config::Parse(page));
    }

    return multiConfig;
}

void MultiConfig::clear() {
    m_configs.clear();
}
