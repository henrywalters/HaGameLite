//
// Created by henry on 1/7/23.
//

#ifndef HAGAME2_SCENE_H
#define HAGAME2_SCENE_H

#include <typeinfo>
#include "object.h"
#include "entity.h"
#include "script.h"
#include "system.h"
#include "../utils/random.h"
#include "../utils/config.h"

class b2World;

namespace hg {

    class Game;

    struct SceneDescriptor {
        std::vector<std::string> scripts;
    };

    class Scene : public Object {
    public:

        Scene():
            m_random(std::make_unique<utils::Random>()),
            m_game(nullptr)
        {}

        friend class Game;

        std::vector<std::shared_ptr<Script>> scripts;

        EntityManager entities;

        Game* game() { return m_game; }
        b2World* physics2d() { return m_physics2d; }

        utils::Random* random() { return m_random.get(); }

        void init();

        void activate() {
            for (const auto& script : scripts) {
                script->init();
            }
            onActivate();
        }

        void update(double dt) {

            for (const auto& [key, system] : m_systems) {
                system->onBeforeUpdate();
            }

            for (const auto& script : scripts) {
                script->update(dt);
            }
            onUpdate(dt);

            for (const auto& [key, system] : m_systems) {
                system->onUpdate(dt);
            }

            for (const auto& [key, system] : m_systems) {
                system->onAfterUpdate();
            }
        }

        void fixedUpdate(double dt) {
            onFixedUpdate(dt);
            for (const auto& [key, system] : m_systems) {
                system->onFixedUpdate(dt);
            }
        }

        void deactivate() {
            for (const auto& script : scripts) {
                script->close();
            }
            onDeactivate();
        }

        hg::utils::MultiConfig save();
        void load(hg::utils::MultiConfig scene);

        template <IsSystem System, class... Args>
        System* addSystem(Args &&... args) {
            auto system = std::make_shared<System>(std::forward<Args>(args)...);
            system->scene = this;
            m_systems.insert(std::make_pair(typeid(System).hash_code(), system));
            return system.get();
        }

        template <IsSystem System>
        System* getSystem() {
            if (m_systems.find(typeid(System).hash_code()) == m_systems.end()) {
                throw std::runtime_error("System: " + std::string(typeid(System).name()) + " does not exist");
            }
            return (System*) m_systems[typeid(System).hash_code()].get();
        }

        void addToGroup(std::string group, hg::Entity* entity) {
            entities.groups.addToGroup(group, entity);
        }

    protected:

        OBJECT_NAME(Scene)

        virtual void onInit() {}
        virtual void onActivate() {}
        virtual void onDeactivate() {}
        virtual void onUpdate(double dt) {}
        virtual void onFixedUpdate(double dt) {}

    private:

        std::unique_ptr<utils::Random> m_random;
        b2World* m_physics2d;

        Game* m_game;

        std::unordered_map<size_t, std::shared_ptr<System>> m_systems;
    };

}

#endif //HAGAME2_SCENE_H
