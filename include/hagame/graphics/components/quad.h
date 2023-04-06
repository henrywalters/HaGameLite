//
// Created by henry on 3/25/23.
//

#ifndef HAGAME2_QUAD_H
#define HAGAME2_QUAD_H

#include "../../core/component.h"
#include "../primitives/quad.h"
#include "../color.h"
#include "../mesh.h"
#include "../../math/aliases.h"

namespace hg::graphics {
    class Quad : public Component {
    public:
        Quad(Vec2 size, Vec2 offset):
        quad(size, offset),
        m_mesh(std::make_unique<MeshInstance>(&quad))
        {
            quad.onUpdate.subscribe([&]() {
                m_mesh->update(&quad);
            });
        }

        Color color = Color::white();

        primitives::Quad quad;

        MeshInstance* mesh() const { return m_mesh.get(); }
    private:
        std::unique_ptr<MeshInstance> m_mesh;
    };
}

#endif //HAGAME2_QUAD_H
