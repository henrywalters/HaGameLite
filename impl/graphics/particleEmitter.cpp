//
// Created by henry on 5/8/23.
//
#include <thread>

#include "../../include/hagame/graphics/particleEmitter.h"
#include "../../include/hagame/utils/profiler.h"
#include "../../include/hagame/math/constants.h"

using namespace hg;
using namespace hg::utils;
using namespace hg::graphics;

template <class T>
class Thread {

public:

    Thread(T* instance, std::function<void(T*)> fn):
        m_instance(instance),
        m_fn(fn)
    {
        std::thread thread(&Thread::run, this);
        thread.detach();
    }

private:

    T* m_instance;
    std::function<void(T*)> m_fn;

    void run() {
        m_fn(m_instance);
    }

};

ParticleEmitter::ParticleEmitter(size_t maxParticles, MeshInstance* mesh):
        m_maxParticles(maxParticles),
        m_mesh(mesh),
        m_elapsedTime(0)
{
    Profiler::Start();
    std::vector<Particle> particles;

    for (int i = 0; i < maxParticles; i++) {
        Particle particle;
        particle.initialized = false;
        particles.push_back(particle);
    }

    m_buffer = VertexBuffer<Particle>::Dynamic(particles);

    auto vao = m_mesh->getVAO();
    m_buffer->bind();
    vao->bind();
    vao->defineAttribute(m_buffer.get(), DataType::Float, 3, 3, offsetof(Particle, startPos));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 4, 3, offsetof(Particle, velocity));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 5, 4, offsetof(Particle, startColor));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 6, 4, offsetof(Particle, endColor));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 7, 1, offsetof(Particle, startTime));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 8, 1, offsetof(Particle, aliveFor));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 9, 3, offsetof(Particle, gravity));
    vao->defineAttribute(m_buffer.get(), DataType::Float, 10, 1, offsetof(Particle, scale));
    vao->defineAttribute(m_buffer.get(), DataType::Int, 11, 1, offsetof(Particle, initialized));
    vao->setInstanced(3, 11);
    Profiler::End();
}

void ParticleEmitter::resize(size_t maxParticles) {
    m_buffer->resize(maxParticles);
}

void ParticleEmitter::fire() {
    m_fire = true;
}

bool ParticleEmitter::finished() const {
    return settings.singleShot && m_fired;
}

void ParticleEmitter::update(hg::Vec3 pos, double dt) {
    utils::Profiler::Start("ParticleEmitter::update");

    m_position = pos;

    double now = m_elapsedTime + dt;
    double timeSinceLastEmission = now - m_lastEmission;
    m_elapsedTime = now;

    if (settings.singleShot) {

        if (!m_fire) {
            return;
        }

        int particlesLeft = settings.singleShotParticles - m_particlesEmitted;
        int particles;

        if (settings.singleShotDuration == 0) {
            particles = settings.singleShotParticles;
        } else {
            particles = (timeSinceLastEmission / settings.singleShotDuration) * settings.singleShotParticles;
        }

        particles = particles < particlesLeft ? particles : particlesLeft;

        for (int i = 0; i < particles; i++) {
            emit();
        }

        if (particles == 0 || settings.singleShotDuration == 0) {
            m_fired = true;
            m_fire = false;
            m_particlesEmitted = 0;
        }

        if (particles > 0) {
            m_lastEmission = m_elapsedTime;
        }

    } else {
        double emissionRate = 1.0 / settings.particlesPerSecond;
        if (timeSinceLastEmission >= emissionRate) {
            int particles = timeSinceLastEmission / emissionRate;
            for (int i = 0; i < particles; i++) {
                emit();
            }

            m_lastEmission = m_elapsedTime;
        }
    }

    utils::Profiler::End("ParticleEmitter::update");
}

void ParticleEmitter::singleshot() {

    Profiler::Start();

    for (int i = 0; i < settings.singleShotParticles; i++) {
        emit();
    }
    m_fire = false;
    m_fired = true;
    m_lastFire = m_elapsedTime;

    Profiler::End();
}

void ParticleEmitter::emit() {
    utils::Random rand;
    Particle particle;
    particle.gravity = settings.gravity;
    float theta = rand.real<float>(settings.angle.lower, settings.angle.upper) * (math::PI / 180.0f);
    particle.velocity = Vec3(cos(theta), sin(theta), 0).normalized() * rand.real<float>(settings.speed.lower, settings.speed.upper);
    particle.startColor = settings.startColor;
    particle.endColor = settings.endColor;
    particle.startTime = m_elapsedTime;
    particle.startPos = settings.shape.generate() + (settings.positionRelative ? hg::Vec3::Zero() : m_position);
    particle.aliveFor = rand.real<float>(settings.aliveFor.lower, settings.aliveFor.upper);
    particle.scale = rand.real<float>(settings.scale.lower, settings.scale.upper);
    particle.initialized = true;
    m_buffer->update(m_index++, particle);

    m_particlesEmitted++;

    if (m_index == m_maxParticles) {
        m_index = 0;
    }
}

void ParticleEmitter::render(hg::graphics::ShaderProgram* shader) {
    Profiler::Start("ParticleEmitter::render");
    shader->use();
    m_mesh->getVAO()->bind();
    shader->setVec3("position", m_position);
    shader->setInt("positionRelative", settings.positionRelative);
    shader->setFloat("currentTime", m_elapsedTime);
    glDrawArraysInstanced(GL_TRIANGLES, 0, m_mesh->size(), m_maxParticles);
    Profiler::End("ParticleEmitter::render");
}

hg::utils::Config ParticleEmitterSettings::save() {
    const std::string section = "ParticleEmitter";
    Config config;
    config.addSection(section);
    config.set(section, "particlesPerSecond", particlesPerSecond);
    config.set(section, "positionRelative", positionRelative);
    config.set(section, "singleShot", singleShot);
    config.set(section, "singleShotParticles", singleShotParticles);
    config.set(section, "singleShotDuration", singleShotDuration);
    config.setInterval<float>(section, "angle", angle);
    config.setInterval<float>(section, "speed", speed);
    config.setInterval<float>(section, "aliveFor", aliveFor);
    config.setInterval<float>(section, "scale", scale);
    config.setArray<float, 4>(section, "startColor", startColor.vector);
    config.setArray<float, 4>(section, "endColor", endColor.vector);
    config.setArray<float, 3>(section, "gravity", gravity.vector);

    shape.save(config);

    return config;
}

void ParticleEmitterSettings::load(utils::Config config) {
    const std::string section = "ParticleEmitter";

    particlesPerSecond = config.get<int>(section, "particlesPerSecond");
    positionRelative = config.get<bool>(section, "positionRelative");
    singleShot = config.get<bool>(section, "singleShot");
    singleShotParticles = config.get<int>(section, "singleShotParticles");
    singleShotDuration = config.get<float>(section, "singleShotDuration");
    angle = config.getInterval<float>(section, "angle");
    speed = config.getInterval<float>(section, "speed");
    aliveFor = config.getInterval<float>(section, "aliveFor");
    scale = config.getInterval<float>(section, "scale");

    config.getArray<float, 4>(section, "startColor", startColor.vector);
    config.getArray<float, 4>(section, "endColor", endColor.vector);
    config.getArray<float, 3>(section, "gravity", gravity.vector);

    shape.load(config);
}

void EmitterShapeSettings::load(Config &config) {
    const std::string section = "Shape";

    type = (EmitterShape) config.get<int>(section, "type");

    if (type == EmitterShape::Disc) {
        radius = config.get<float>(section, "radius");
    } else if (type == EmitterShape::Donut) {
        innerRadius = config.get<float>(section, "innerRadius");
        outerRadius = config.get<float>(section, "outerRadius");
    }
}

void EmitterShapeSettings::save(Config &config) {

    const std::string section = "Shape";

    config.addSection(section);

    config.set(section, "type", (int)type);

    if (type == EmitterShape::Disc) {
        config.set(section, "radius", radius.value());
    } else if (type == EmitterShape::Donut) {
        config.set(section, "innerRadius", innerRadius.value());
        config.set(section, "outerRadius", outerRadius.value());
    }
}

Vec3 EmitterShapeSettings::generate() {
    Random rand;
    if (type == EmitterShape::Point) {
        return Vec3::Zero();
    } else if (type == EmitterShape::Disc) {
        float angle = rand.real<float>(0, math::PI * 2);
        float randRadius = rand.real<float>(0, radius.value());
        return randRadius * Vec3(cos(angle), sin(angle), 0);
    } else if (type == EmitterShape::Donut) {
        float angle = rand.real<float>(0, math::PI * 2);
        float randRadius = rand.real<float>(innerRadius.value(), outerRadius.value());
        return randRadius * Vec3(cos(angle), sin(angle), 0);
    }

    throw std::runtime_error("UNSUPPORTED EMITTER SHAPE");
}
