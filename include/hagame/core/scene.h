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

        EntityManager entities;

        Game* game() { return m_game; }
        b2World* physics2d() { return m_physics2d; }

        utils::Random* random() { return m_random.get(); }

        HG_GET(std::vector<std::shared_ptr<Script>>, scripts)

        void init();

        void activate() {
            for (const auto& script : m_scripts) {
                script->init();
            }
            onActivate();
        }

        void update(double dt) {

            //onBeforeUpdate();

            for (const auto& [key, system] : m_systems) {
                system->onBeforeUpdate();
            }

            for (const auto& script : m_scripts) {
                script->update(dt);
            }
            onUpdate(dt);

            for (const auto& [key, system] : m_systems) {
                system->onUpdate(dt);
            }

            for (const auto& [key, system] : m_systems) {
                system->onAfterUpdate();
            }

            onAfterUpdate();
        }

        void fixedUpdate(double dt) {
            onFixedUpdate(dt);
            for (const auto& [key, system] : m_systems) {
                system->onFixedUpdate(dt);
            }
        }

        void deactivate() {
            for (const auto& script : m_scripts) {
                script->close();
            }
            onDeactivate();
        }

        hg::utils::MultiConfig save();
        void load(hg::utils::MultiConfig scene);

        template <IsScript _Script, class... Args>
        _Script* addScript(Args &&... args) {
            auto script = std::make_shared<_Script>(std::forward<Args>(args)...);
            script->m_scene = this;
            m_scripts.push_back(script);
            return script.get();
        }

        Script* addScript(std::shared_ptr<Script> script) {
            script->m_scene = this;
            m_scripts.push_back(script);
            return script.get();
        }

        void removeScript(Script* script) {
            m_scripts.erase(std::remove_if(m_scripts.begin(), m_scripts.end(), [script](auto other) {return script->getDef().name == other->getDef().name; }), m_scripts.end());
        }

        template <IsSystem _System, class... Args>
        _System* addSystem(Args &&... args) {
            auto system = std::make_shared<_System>(std::forward<Args>(args)...);
            system->scene = this;
            m_systems.insert(std::make_pair(typeid(_System).hash_code(), system));
            return system.get();
        }

        template <IsSystem _System>
        _System* getSystem() {
            size_t id = typeid(_System).hash_code();
            if (m_systems.find(id) == m_systems.end()) {
                throw std::runtime_error("System does not exist");
            }
            return (_System*) m_systems[id].get();
        }

        // Use this when you don't have access to the Typeinfo (i.e. in a script)
        template <IsSystem _System>
        _System* getSystem(std::string name) {
            size_t id = hg::SystemFactory::Get(name).hashCode;
            if (m_systems.find(id) == m_systems.end()) {
                throw std::runtime_error("System does not exist");
            }
            return (_System*) m_systems[id].get();
        }

        void addToGroup(std::string group, hg::Entity* entity) {
            entities.groups.addToGroup(group, entity);
        }

    protected:

        OBJECT_NAME(Scene)

        virtual void onInit() {}
        virtual void onActivate() {}
        virtual void onDeactivate() {}
        // virtual void onBeforeUpdate() {}
        virtual void onUpdate(double dt) {}
        virtual void onAfterUpdate() {}
        virtual void onFixedUpdate(double dt) {}

    private:

        std::unique_ptr<utils::Random> m_random;
        b2World* m_physics2d;

        Game* m_game;

        std::unordered_map<size_t, std::shared_ptr<System>> m_systems;
        std::vector<std::shared_ptr<Script>> m_scripts;

    };

}

#endif //HAGAME2_SCENE_H
