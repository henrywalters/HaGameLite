//
// Created by henry on 5/8/23.
//

#ifndef HAGAME2_PARTICLEEMITTER_H
#define HAGAME2_PARTICLEEMITTER_H

#include <optional>
#include "buffer.h"
#include "mesh.h"
#include "shaderProgram.h"
#include "color.h"
#include "../utils/clock.h"
#include "../utils/random.h"
#include "../utils/config.h"
#include "../math/interval.h"

namespace hg::graphics {

    enum class EmitterShape {
        Point,
        Disc,
        Donut,
    };

    const std::vector<std::string> EMITTER_SHAPE_NAMES = {
            "Point",
            "Disc",
            "Donut",
    };

    struct EmitterShapeSettings {

        EmitterShape type = EmitterShape::Point;

        std::optional<float> radius;
        std::optional<float> innerRadius;
        std::optional<float> outerRadius;

        void load(utils::Config& config);
        void save(utils::Config& config);

        Vec3 generate();
    };

    struct ParticleEmitterSettings {

        EmitterShapeSettings shape;
        int particlesPerSecond = 1;
        hg::math::Interval<float> angle = hg::math::Interval<float>(0, 360.0f);
        hg::math::Interval<float> speed = hg::math::Interval<float>(100, 500);
        hg::math::Interval<float> aliveFor = hg::math::Interval<float>(0.1, 0.3);
        hg::math::Interval<float> scale = hg::math::Interval<float>(1, 2);
        hg::graphics::Color startColor = hg::graphics::Color::red();
        hg::graphics::Color endColor = hg::graphics::Color::red();
        hg::Vec3 gravity = hg::Vec3(0, 0, 0);
        bool positionRelative = false;
        bool singleShot = false;
        int singleShotParticles = 100;
        float singleShotDuration = 1.0f;

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

        // Change the number of particles in the buffer
        void resize(size_t maxParticles);

        // Will trigger the necessary emitions
        void update(hg::Vec3 pos, double dt);

        // If single shot is turned on, this will cause particles to emit in the next
        void fire();

        // Clear all particles
        void clear();

        // Render particles
        void render(hg::graphics::ShaderProgram* shader);

        std::function<void()> onFinished = [](){};

        bool finished() const;

    private:

        struct Particle {
            hg::Vec3 startPos;
            hg::Vec3 velocity;
            hg::graphics::Color startColor;
            hg::graphics::Color endColor;
            float scale;
            float startTime;
            float aliveFor;
            hg::Vec3 gravity;
            int initialized = 0;
        };

        void emit();

        void singleshot();

        size_t m_maxParticles;
        hg::graphics::MeshInstance* m_mesh;

        std::unique_ptr<hg::graphics::VertexBuffer<Particle>> m_buffer;
        int m_index = 0;

        double m_elapsedTime = 0;
        double m_lastEmission = 0;

        int m_particlesEmitted = 0;
        bool m_fire = false;
        bool m_fired = false;
        double m_lastFire = 0;

        hg::Vec3 m_position;
    };
}

#endif //HAGAME2_PARTICLEEMITTERCOMPONENT_H
