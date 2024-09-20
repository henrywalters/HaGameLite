//
// Created by henry on 9/9/24.
//
#include "../../../include/hagame/graphics/material.h"
#include "../../../include/hagame/utils/file.h"
#include "../../../include/hagame/utils/string.h"

using namespace hg::graphics;

Color getColor(std::string line) {
    auto parts = hg::utils::s_split(line, ' ');
    return Color(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3]), 1.0f);
}

Materials::Materials(std::string mtlPath) {

    std::string currentMtl = "";

    for (const auto& line : hg::utils::f_readLines(mtlPath)) {
        auto parts = hg::utils::s_split(line, ' ');
        auto command = parts[0];

        if (command == "newMtl") {
            m_materials.insert(std::make_pair(parts[1], Material{}));
            currentMtl = parts[1];
        }

        if (currentMtl == "") {
            continue;
        }

        if (command == "Ns") {
            m_materials[currentMtl].shininess = std::stof(parts[1]);
        } else if (command == "Ka") {
            m_materials[currentMtl].albedo = getColor(line);
        } else if (command == "Kd") {
            m_materials[currentMtl].diffuse = getColor(line);
        } else if (command == "Ke") {
            m_materials[currentMtl].specular = getColor(line);
        } else if (command == "Ni") {
            m_materials[currentMtl].opticalDensity = std::stof(parts[1]);
        } else if (command == "d") {
            m_materials[currentMtl].dissolve = std::stof(parts[1]);
        } else if (command == "illum") {
            m_materials[currentMtl].illumModel = std::stoi(parts[1]);
        } else if (command == "map_Kd") {
            m_materials[currentMtl].mapTexture = parts[1];
        }

    }
}

Material Materials::get(std::string name) {
    return m_materials.at(name);
}

std::unordered_map<std::string, Material> &Materials::all() {
    return m_materials;
}
