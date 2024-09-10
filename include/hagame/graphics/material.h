//
// Created by henry on 9/9/24.
//

#ifndef HAGAME2_MATERIAL_H
#define HAGAME2_MATERIAL_H

#include "../math/aliases.h"

namespace hg::graphics {
    struct Material {
        Vec3 albedo;
        Vec3 diffuse;
        Vec3 specular;
        float shininess;
        float opticalDensity;
        float dissolve;
        std::string mapTexture;
        int illumModel;
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
