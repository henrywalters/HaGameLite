//
// Created by henry on 5/8/23.
//
#include "../../include/hagame/graphics/particleEmitter.h"

using namespace hg;
using namespace hg::utils;
using namespace hg::graphics;

ParticleEmitter::ParticleEmitter(size_t maxParticles, MeshInstance* mesh):
        m_maxParticles(maxParticles),
        m_mesh(mesh),
        m_buffer(VertexBuffer<Particle>::Dynamic(m_maxParticles)),
        m_elapsedTime(0)
{
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
    vao->setInstanced(3, 10);

}

void ParticleEmitter::resize(size_t maxParticles) {
    m_buffer->resize(maxParticles);
}

void ParticleEmitter::fire() {
    m_fire = true;
}

bool ParticleEmitter::finished() const {
    return settings.singleShot && (m_elapsedTime - m_lastFire) >= settings.aliveFor.upper;
}

void ParticleEmitter::update(hg::Vec3 pos, double dt) {
    m_position = pos;
    m_elapsedTime += dt;

    if (settings.singleShot) {

        if (!m_fire) {
            return;
        }
        for (int i = 0; i < settings.singleShotParticles; i++) {
            emit();
        }
        m_fire = false;
        m_fired = true;
        m_lastFire = m_elapsedTime;
    } else {
        double timeSinceLastEmission = m_elapsedTime - m_lastEmission;
        double emissionRate = 1.0 / settings.particlesPerSecond;
        if (timeSinceLastEmission >= emissionRate) {
            int particles = timeSinceLastEmission / emissionRate;
            for (int i = 0; i < particles; i++) {
                emit();
            }
            m_lastEmission = m_elapsedTime;
        }
    }
}

void ParticleEmitter::emit() {
    utils::Random rand;
    Particle particle;
    particle.gravity = settings.gravity;
    float theta = rand.real<float>(settings.angle.lower, settings.angle.upper) * (M_PI / 180.0f);
    particle.velocity = Vec3(cos(theta), sin(theta), 0).normalized() * rand.real<float>(settings.speed.lower, settings.speed.upper);
    particle.startColor = settings.startColor;
    particle.endColor = settings.endColor;
    particle.startTime = m_elapsedTime;
    particle.startPos = settings.positionRelative ? hg::Vec3::Zero() : m_position;
    particle.aliveFor = rand.real<float>(settings.aliveFor.lower, settings.aliveFor.upper);
    particle.scale = rand.real<float>(settings.scale.lower, settings.scale.upper);
    m_buffer->update(m_index++, particle);

    if (m_index == m_maxParticles) {
        m_index = 0;
    }
}

void ParticleEmitter::render(hg::graphics::ShaderProgram* shader) {
    shader->use();
    m_mesh->getVAO()->bind();
    shader->setVec3("position", m_position);
    shader->setInt("positionRelative", settings.positionRelative);
    shader->setFloat("currentTime", m_elapsedTime);
    glDrawArraysInstanced(GL_TRIANGLES, 0, m_mesh->size(), m_maxParticles);
}

hg::utils::Config ParticleEmitterSettings::save() {
    const std::string section = "ParticleEmitter";
    Config config;
    config.addSection(section);
    config.set(section, "particlesPerSecond", particlesPerSecond);
    config.set(section, "positionRelative", positionRelative);
    config.set(section, "singleShot", singleShot);
    config.set(section, "singleShotParticles", singleShotParticles);
    config.setInterval<float>(section, "angle", angle);
    config.setInterval<float>(section, "speed", speed);
    config.setInterval<float>(section, "aliveFor", aliveFor);
    config.setInterval<float>(section, "scale", scale);
    config.setArray<float, 4>(section, "startColor", startColor.vector);
    config.setArray<float, 4>(section, "endColor", endColor.vector);
    config.setArray<float, 3>(section, "gravity", gravity.vector);
    return config;
}

void ParticleEmitterSettings::load(utils::Config config) {
    const std::string section = "ParticleEmitter";
    particlesPerSecond = config.get<int>(section, "particlesPerSecond");
    positionRelative = config.get<bool>(section, "positionRelative");
    singleShot = config.get<bool>(section, "singleShot");
    singleShotParticles = config.get<int>(section, "singleShotParticles");
    angle = config.getInterval<float>(section, "angle");
    speed = config.getInterval<float>(section, "speed");
    aliveFor = config.getInterval<float>(section, "aliveFor");
    scale = config.getInterval<float>(section, "scale");

    config.getArray<float, 4>(section, "startColor", startColor.vector);
    config.getArray<float, 4>(section, "endColor", endColor.vector);
    config.getArray<float, 3>(section, "gravity", gravity.vector);

}
