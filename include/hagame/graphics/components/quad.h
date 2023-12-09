//
// Created by henry on 3/25/23.
//

#ifndef HAGAME2_QUAD_H
#define HAGAME2_QUAD_H

#include "../../core/component.h"
#include "../../core/entity.h"
#include "../primitives/quad.h"
#include "../color.h"
#include "../mesh.h"
#include "../../math/aliases.h"

namespace hg::graphics {
    class Quad : public Component {
    public:

        Quad():
            quad(std::make_unique<primitives::Quad>()),
            m_mesh(std::make_unique<MeshInstance>(quad.get()))
        {}

        Quad(Vec2 size, Vec2 offset):
            quad(std::make_unique<primitives::Quad>(size, offset)),
            m_mesh(std::make_unique<MeshInstance>(quad.get()))
        {
            quad->onUpdate.subscribe([&]() {
                m_mesh->update(quad.get());
            });
        }

        Color color = Color::white();

        std::unique_ptr<primitives::Quad> quad;

        MeshInstance* mesh() const { return m_mesh.get(); }

        void onParamChange() override {
            m_mesh->update(quad.get());
        }

    private:

        std::unique_ptr<MeshInstance> m_mesh;
    };

    HG_COMPONENT(Graphics, Quad)

}

#endif //HAGAME2_QUAD_H
