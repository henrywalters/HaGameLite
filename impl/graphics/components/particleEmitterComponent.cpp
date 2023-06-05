//
// Created by henry on 5/9/23.
//
#include "../../../include/hagame/graphics/components/particleEmitterComponent.h"
#include "../../../include/hagame/core/entity.h"

void hg::graphics::ParticleEmitterComponent::onUpdate(double dt) {
    m_emitter->update(entity->transform.position, dt);
}

std::string hg::graphics::ParticleEmitterComponent::toString() const {
    return "ParticleEmitterComponent<" + std::to_string(id()) + ">";
}

void hg::graphics::ParticleEmitterComponent::render(hg::graphics::ShaderProgram* shader) {
    m_emitter->render(shader);
}
