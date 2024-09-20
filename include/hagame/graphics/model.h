//
// Created by henry on 9/14/24.
//

#ifndef HAGAME2_MODEL_H
#define HAGAME2_MODEL_H

#include <string>
#include "../structures/tree.h"
#include "mesh.h"
#include "material.h"

namespace hg::graphics {

    struct ModelNode : public structures::Tree {
        std::vector<Mesh> meshes;
        std::vector<Material> materials;
        std::vector<MeshInstance> instances;
    };

    class Model {
    public:

        Model(std::string path);

        ModelNode* root() {
            return m_nodes.size() > 0 ? m_nodes[0].get() : nullptr;
        }

        void render();

    private:

        std::vector<std::unique_ptr<ModelNode>> m_nodes;

    };
}

#endif //HAGAME2_MODEL_H
