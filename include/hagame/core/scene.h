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
#include "../utils/pubsub.h"

class b2World;

namespace hg {

    class Game;

    struct SceneDescriptor {
        std::vector<std::string> scripts;
    };

    class Scene;

    template <typename T>
    concept IsScene = std::is_base_of<Scene, T>::value;

    enum class SceneEvent {
        Initialized,
        Activated,
        Deactivated,
    };

    class Scene : public Object {
    public:

        EventEmitter<SceneEvent> events;

        Scene():
            entities(this),
            m_random(std::make_unique<utils::Random>()),
            m_game(nullptr)
        {}

        friend class Game;

        EntityManager entities;

        Game* game() { return m_game; }
        b2World* physics2d() { return m_physics2d; }

        utils::Random* random() { return m_random.get(); }

        HG_GET(std::vector<std::shared_ptr<Script>>, scripts)

        HG_GET_SET_SIMPLE(bool, active)

        void init();

        void activate();

        void update(double dt);

        void fixedUpdate(double dt);

        void deactivate();

        void clear();

        // TODO: Add Json saving cause multi config is a bit whack
        //hg::utils::JsonConfig saveToJson(hg::Entity* entity = nullptr);
        //void loadFromJson(hg::utils::JsonConfig const& config);

        hg::utils::MultiConfig save(hg::Entity* entity = nullptr);
        void load(hg::utils::MultiConfig scene);

        template <IsScene _Scene, class... Args>
        _Scene* addChild(Args &&... args) {
            m_children.push_back(std::make_unique<_Scene>(std::forward<Args>(args)...));
            return (_Scene*) m_children[m_children.size() - 1].get();
        }

        void removeChild(Scene* scene) {
            m_children.erase(std::remove_if(
                m_children.begin(),
                m_children.end(),
                [scene](const auto& a) {
                    return scene == a.get();
                }
            ), m_children.end());
        }

        std::vector<Scene*> children() {
            std::vector<Scene*> out;
            for (const auto& child : m_children) {
                out.push_back(child.get());
            }
            return out;
        }

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
        bool hasSystem() {
            return m_systems.find(typeid(_System).hash_code()) != m_systems.end();
        }

        bool hasSystem(std::string name) {
            size_t id = hg::SystemFactory::Get(name).hashCode;
            return m_systems.find(id) != m_systems.end();
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
        virtual void onRender(double dt) {}

    private:

        bool m_active = true;

        std::unique_ptr<utils::Random> m_random;
        b2World* m_physics2d;

        Game* m_game;

        std::unordered_map<size_t, std::shared_ptr<System>> m_systems;
        std::vector<std::shared_ptr<Script>> m_scripts;

        std::vector<std::unique_ptr<Scene>> m_children;
    };

}

#endif //HAGAME2_SCENE_H
