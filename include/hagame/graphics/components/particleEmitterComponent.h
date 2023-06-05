//
// Created by henry on 5/9/23.
//

#ifndef HAGAME2_PARTICLEEMITTERCOMPONENT_H
#define HAGAME2_PARTICLEEMITTERCOMPONENT_H

#include "../primitives/disc.h"
#include "../particleEmitter.h"
#include "../../core/component.h"
#include "../shaderProgram.h"

namespace hg::graphics {
    class ParticleEmitterComponent : public hg::Component {
    public:

        ParticleEmitterComponent(ParticleEmitterSettings settings)
        {
            m_disc = std::make_unique<primitives::Disc>(1, 10);
            m_mesh = std::make_unique<MeshInstance>(m_disc.get());
            m_emitter = std::make_unique<ParticleEmitter>(1000, m_mesh.get());
            m_emitter->settings = settings;
        }

        void render(ShaderProgram* shader);

        ParticleEmitter* emitter() { return m_emitter.get(); }

    protected:

        void onUpdate(double dt) override;

        std::string toString() const override;

    private:

        std::unique_ptr<primitives::Disc> m_disc;
        std::unique_ptr<MeshInstance> m_mesh;
        std::unique_ptr<ParticleEmitter> m_emitter;

    };
}

#endif //HAGAME2_PARTICLEEMITTERCOMPONENT_H
