//
// Created by henry on 2/18/23.
//


#include <hagame/core/assets.h>
#include <hagame/core/scene.h>

#include <deque>

hg::utils::uuid_t hg::System::id = 0;

hg::utils::MultiConfig hg::Scene::save(hg::Entity* entity) {
    hg::utils::MultiConfig scene;
    auto scripts = scene.addPage("Scripts");
    auto meta = scene.addPage("Scenes");
    scene.addPage("Entities");
    scene.addPage("Components");

    for (const auto& script : m_scripts) {
        ScriptDef def = script->getDef();
        scripts->addSection(def.name);
        scripts->setRaw(def.name, "path", def.path);
        scripts->setRaw(def.name, "extension", def.extension);
        scripts->setRaw(def.name, "libPath", def.libPath);
    }

    std::function<void(hg::Entity*)> saveEntity = [&](hg::Entity* node) {
        if (node->name != "root") {
            entities.save(scene, node);
        }
        if (!node->prefab) { // Only save the children & components of non-prefab entities
            for (const auto &child: node->children()) {
                saveEntity(static_cast<Entity *>(child));
            }
        }
    };

    saveEntity(entity ? entity : entities.root.get());

    return scene;
}

void hg::Scene::load(hg::utils::MultiConfig scene) {
    std::unordered_map<std::string, Entity*> entityMap;
    std::unordered_map<hg::utils::uuid_t, Prefab*> prefabMap;

    for (const auto&[key, prefab] : assets::PREFABS.store()) {
        prefabMap.insert(std::make_pair(prefab->id(), prefab.get()));
    }

    auto scripts = scene.getPage("Scripts");

    for (const auto& section : scripts->sections()) {
        ScriptDef def;
        def.name = section;
        def.path = scripts->getRaw(section, "path");
        def.extension = scripts->getRaw(section, "extension");
        def.libPath = scripts->getRaw(section, "libPath");
        addScript(ScriptFactory::Create(def));
    }

    std::unordered_map<utils::uuid_t, hg::Entity*> idMap;

    auto entitiesPage = scene.getPage("Entities");

    for (const auto& section : entitiesPage->sections()) {

        hg::Entity* entity;

        if (entitiesPage->has(section, "prefab")) {
            auto prefabId = entitiesPage->get<utils::uuid_t>(section, "prefab");
            if (prefabMap.find(prefabId) == prefabMap.end()) {
                throw std::runtime_error("Prefab '" + std::to_string(prefabId) + "' not loaded into assets!");
            }
            entity = entities.add(prefabMap.at(prefabId));
        } else {
            entity = entities.add();
        }

        idMap.insert(std::make_pair(std::stol(section), entity));
        entity->name = entitiesPage->getRaw(section, "name");

        entitiesPage->getArray<float, 3>(section, "position", entity->transform.position.vector);
        entitiesPage->getArray<float, 3>(section, "scale", entity->transform.scale.vector);
        entitiesPage->getArray<float, 4>(section, "rotation", entity->transform.rotation.vector);

        entityMap.insert(std::make_pair(section, entity));
    }

    for (const auto& section : entitiesPage->sections()) {
        if (entitiesPage->has(section, "parent")) {
            //auto parent = entities.get();
            auto entity = idMap.at(std::stol(section));
            idMap.at(entitiesPage->get<hg::utils::uuid_t>(section, "parent"))->addChild(entity);
        }
    }

    for (const auto& section : scene.getPage("Components")->sections()) {
        auto parts = utils::s_split(section, '_');
        auto name = parts[0];
        auto entityId = std::stol(parts[1]);

        auto componentDef = ComponentFactory::Get(name);
        auto component = componentDef.attach(idMap.at(entityId));

        for (const auto& field : ComponentFactory::GetFields(name)) {
            auto value = scene.getPage("Components")->getRaw(section, field.field);
            field.setter(component, utils::deserialize(field.type, value));
        }

        component->load(scene.getPage("Components"), section);
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
