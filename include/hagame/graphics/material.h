//
// Created by henry on 9/9/24.
//

#ifndef HAGAME2_MATERIAL_H
#define HAGAME2_MATERIAL_H

#include "../math/aliases.h"
#include "color.h"

namespace hg::graphics {
    struct Material {
        graphics::Color albedo;
        graphics::Color diffuse;
        graphics::Color specular;
        graphics::Color emissive;
        float shininess;
        float opticalDensity;
        float dissolve;
        std::string mapTexture;
        int illumModel;

        std::string toString() const {
            return "Albedo: " + albedo.toString() + "\n" +
                "Diffuse: " + diffuse.toString() + "\n" +
                "Specular: " + specular.toString() + "\n" +
                "Shininess: " + std::to_string(shininess) + "\n";
        }
    };

    class Materials {
    public:

        Materials(std::string mtlPath);

        Material get(std::string name);
        std::unordered_map<std::string, Material>& all();

    private:

        std::unordered_map<std::string, Material> m_materials;

    };
}

#endif //HAGAME2_MATERIAL_H
