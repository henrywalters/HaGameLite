//
// Created by henry on 3/4/24.
//
#include <hagame/core/scene.h>
#include <hagame/core/entity.h>
#include <hagame/utils/config.h>

using namespace hg;

static int entityCount = 0;

hg::Entity::Entity(hg::utils::uuid_t enttId,
                   entt::basic_registry<utils::uuid_t, std::allocator<utils::uuid_t>> *registry):
        m_enttId(enttId),
        m_registry(registry)
{
    name = "Entity " + std::to_string(entityCount++);
};

hg::Entity::Entity(hg::utils::uuid_t id, hg::utils::uuid_t enttId,
                   entt::basic_registry<utils::uuid_t, std::allocator<utils::uuid_t>> *registry):
        GameObject(id),
        m_enttId(enttId),
        m_registry(registry)
{
    name = "Entity " + std::to_string(entityCount++);
};

hg::Entity *hg::Entity::add() {
    return scene->entities.add(this);
}

void hg::Entity::remove() {
    return scene->entities.remove(this);
}

std::string hg::Entity::toString() const {
    return name;
}