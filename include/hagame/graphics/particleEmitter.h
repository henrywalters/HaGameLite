//
// Created by henry on 5/8/23.
//

#ifndef HAGAME2_PARTICLEEMITTER_H
#define HAGAME2_PARTICLEEMITTER_H

#include "buffer.h"
#include "mesh.h"
#include "shaderProgram.h"
#include "color.h"
#include "../utils/clock.h"
#include "../utils/random.h"
#include "../utils/config.h"
#include "../math/interval.h"

namespace hg::graphics {

    struct ParticleEmitterSettings {
        int particlesPerSecond = 1;
        hg::math::Interval<float> angle = hg::math::Interval<float>(0, 360.0f);
        hg::math::Interval<float> speed = hg::math::Interval<float>(100, 500);
        hg::math::Interval<float> aliveFor = hg::math::Interval<float>(0.1, 0.3);
        hg::graphics::Color startColor = hg::graphics::Color::red();
        hg::graphics::Color endColor = hg::graphics::Color::red();
        hg::Vec3 gravity = hg::Vec3(0, 0, 0);
        bool positionRelative = false;

        int requiredBufferSize() const {
            return aliveFor.upper * particlesPerSecond;
        }

        hg::utils::Config save();

        void load(hg::utils::Config config);
    };

    class ParticleEmitter {
    public:

        ParticleEmitter(size_t maxParticles, hg::graphics::MeshInstance* mesh);

        ParticleEmitterSettings settings;

        void resize(size_t maxParticles);
        void update(hg::Vec3 pos, double dt);
        void clear();
        void render(hg::graphics::ShaderProgram* shader);

    private:

        struct Particle {
            hg::Vec3 startPos;
            hg::Vec3 velocity;
            hg::graphics::Color startColor;
            hg::graphics::Color endColor;
            float startTime;
            float aliveFor;
            hg::Vec3 gravity;
        };

        void emit();

        size_t m_maxParticles;
        hg::graphics::MeshInstance* m_mesh;

        std::unique_ptr<hg::graphics::VertexBuffer<Particle>> m_buffer;
        int m_index = 0;

        double m_elapsedTime = 0;
        double m_lastEmission = 0;

        hg::Vec3 m_position;
    };
}

#endif //HAGAME2_PARTICLEEMITTER_H