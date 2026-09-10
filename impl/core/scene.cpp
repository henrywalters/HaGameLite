//
// Created by henry on 2/18/23.
//


#include <hagame/core/assets.h>
#include <hagame/core/scene.h>

#include <deque>

hg::utils::uuid_t hg::System::id = 0;

nlohmann::json hg::Scene::save(hg::Entity* entity) {
    nlohmann::json j;
    j["scripts"] = nlohmann::json::array();
    j["entities"] = nlohmann::json::array();

    for (const auto& script : m_scripts) {
        ScriptDef def = script->getDef();
        nlohmann::json scriptJ;
        scriptJ["path"] = def.path;
        scriptJ["extension"] = def.extension;
        scriptJ["libPath"] = def.libPath;
        j["scripts"].push_back(scriptJ);
    }

    std::function<void(hg::Entity*)> saveEntity = [&](hg::Entity* node) {
        if (node->name == "root") {
            // Never serialize the root sentinel itself — just its direct children
            for (const auto& child : node->children()) {
                saveEntity(static_cast<Entity*>(child));
            }
            return;
        }

        j["entities"].push_back(node->save());
    };

    saveEntity(entity ? entity : entities.root.get());
    return j;
}

void hg::Scene::load(nlohmann::json const &j) {
    std::unordered_map<hg::utils::uuid_t, Prefab*> prefabMap;

    for (const auto&[key, prefab] : assets::PREFABS.store()) {
        prefabMap.insert(std::make_pair(prefab->id(), prefab.get()));
    }

    for (const auto& script : j["scripts"]) {
        ScriptDef def;
        def.name = script["name"].get<std::string>();
        def.path = script["path"].get<std::string>();
        def.extension = script["extension"].get<std::string>();
        def.libPath = script["libPath"].get<std::string>();
        addScript(ScriptFactory::Create(def));
    }

    std::function<void(nlohmann::json, hg::Entity*)> loadEntity = [&](nlohmann::json const& entityJ, hg::Entity* parent = nullptr) {
        hg::Entity* entity;

        if (entityJ.find("prefab") != entityJ.end()) {
            auto prefabId = entityJ["prefab"].get<utils::uuid_t>();
            if (prefabMap.find(prefabId) == prefabMap.end()) {
                throw std::runtime_error("Prefab '" + std::to_string(prefabId) + "' not loaded into assets!");
            }
            entity = parent ? entities.add(parent, prefabMap.at(prefabId)) : entities.add(prefabMap.at(prefabId));
        } else {
            entity = parent ? entities.add(parent) : entities.add();
        }

        entity->load(entityJ);

        for (const auto& child : entityJ["children"]) {
            loadEntity(child, entity);
        }
    };

    for (const auto &entityJ : j["entities"]) {
        loadEntity(entityJ, nullptr);
    }
}

void hg::Scene::init() {

    onInit();
    events.emit(SceneEvent::Initialized);

    for (const auto& child : m_children) {
        child->init();
    }

    for (const auto& [key, system] : m_systems) {
        system->onInit();
    }
}

void hg::Scene::activate() {

    for (const auto& script : m_scripts) {
        script->init();
    }
    onActivate();
    events.emit(SceneEvent::Activated);

    for (const auto& child : m_children) {
        child->activate();
    }
}

void hg::Scene::update(double dt) {

    if (m_active) {
        for (const auto& [key, system] : m_systems) {
            system->onBeforeUpdate();
        }

        for (const auto& script : m_scripts) {
            script->update(dt);
        }
        onUpdate(dt);
        for (const auto& child : m_children) {
            child->update(dt);
        }

        for (const auto& [key, system] : m_systems) {
            system->onUpdate(dt);
        }
    }

    for (const auto& [key, system] : m_systems) {
        system->onRender(dt);
    }

    onRender(dt);

    if (m_active) {
        for (const auto& [key, system] : m_systems) {
            system->onAfterUpdate();
        }

        onAfterUpdate();
    }
}

void hg::Scene::fixedUpdate(double dt) {

    if (!m_active) {
        return;
    }

    onFixedUpdate(dt);
    for (const auto& child : m_children) {
        child->fixedUpdate(dt);
    }
    for (const auto& [key, system] : m_systems) {
        system->onFixedUpdate(dt);
    }
}

void hg::Scene::deactivate() {

    for (const auto& script : m_scripts) {
        script->close();
    }
    onDeactivate();
    events.emit(SceneEvent::Deactivated);

    for (const auto& child : m_children) {
        child->deactivate();
    }
}

void hg::Scene::clear() {
    entities.clear();
    m_scripts.clear();
}
