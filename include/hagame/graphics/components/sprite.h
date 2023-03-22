//
// Created by henry on 3/5/23.
//

#ifndef HAGAME2_SPRITE_H
#define HAGAME2_SPRITE_H

#include "../../core/component.h"
#include "../primitives/quad.h"

namespace hg::graphics {
    class Sprite : public hg::Component {
    public:

        Sprite(Vec2 size):
            quad(size),
            m_mesh(std::make_unique<MeshInstance>(&quad))
        {
            quad.onUpdate.subscribe([&]() {
                m_mesh->update(&quad);
            });
        }

        std::string texture;
        primitives::Quad quad;

        MeshInstance* mesh() const { return m_mesh.get(); }

    protected:
        std::string toString() const override {
            return "Sprite<" + std::to_string(id()) + ">";
        }
    private:

        std::unique_ptr<MeshInstance> m_mesh;


    };
}

#endif //HAGAME2_SPRITE_H
