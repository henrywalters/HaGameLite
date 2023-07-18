//
// Created by henry on 6/28/23.
//

#ifndef HAGAME2_HEALTHBAR_H
#define HAGAME2_HEALTHBAR_H

#include "../../graphics/primitives/quad.h"
#include "../../graphics/mesh.h"
#include "../../core/component.h"
#include "../../core/entity.h"
#include "../../math/aliases.h"
#include "../../graphics/shaderProgram.h"
#include "../../graphics/color.h"

namespace hg {
    const hg::Vec2 DEFAULT_HEALTH_BAR_SIZE = hg::Vec2(100, 10);

    class HealthBar : public hg::Component {
    public:

        HealthBar():
                m_quad(DEFAULT_HEALTH_BAR_SIZE, DEFAULT_HEALTH_BAR_SIZE * -0.5),
                m_mesh(&m_quad)
        {
            m_quad.centered(false);
        }

        float health = 100.0f;
        float maxHealth = 100.0f;
        hg::Vec2 offset = hg::Vec2::Zero();
        hg::Vec2 size = DEFAULT_HEALTH_BAR_SIZE;

        void render(hg::graphics::ShaderProgram* shader) {
            float percentHealth = health / maxHealth;
            hg::Vec2 healthSize(percentHealth * size.x(), size.y());
            hg::Vec2 missingSize((1.0f - percentHealth) * size.x(), size.y());
            hg::Vec2 healthOffset(0, size.y() * -0.5);
            hg::Vec2 missingOffset(healthSize.x(), size.y() * -0.5);

            shader->use();
            shader->setMat4("model", entity->model());

            shader->setVec4("color", hg::graphics::Color::green());

            m_quad.size(healthSize);
            m_quad.offset(healthOffset + offset);
            m_quad.update();
            m_mesh.update(&m_quad);
            m_mesh.render();

            shader->setVec4("color", hg::graphics::Color::red());

            m_quad.size(missingSize);
            m_quad.offset(missingOffset + offset);
            m_quad.update();
            m_mesh.update(&m_quad);
            m_mesh.render();
        }

    private:

        hg::graphics::primitives::Quad m_quad;
        hg::graphics::MeshInstance m_mesh;

    };

}

#endif //HAGAME2_HEALTHBAR_H
