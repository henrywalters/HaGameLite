//
// Created by henry on 2/18/23.
//


#include "../../../include/hagame/core/scene.h"

#include <box2d/box2d.h>
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
        for (const auto& child : node->children()) {
            saveEntity(static_cast<Entity*>(child));
        }
    };

    saveEntity(entity ? entity : entities.root.get());

//    while (nodes.size() > 0) {
//        auto front = nodes.front();
//        nodes.pop_front();
//
//        if (front->name != "root") {
//            entities.save(scene, front);
//        }
//
//        for (const auto& child : front->children()) {
//            nodes.push_back(static_cast<hg::Entity*>(child));
//        }
//    }
//
//    entities.forEach([&](auto entity) {
//
//        if (entity == nullptr || entity->name == "root") {
//            return;
//        }
//
//        entities.save(scene, entity);
//    });

    return scene;
}

void hg::Scene::load(hg::utils::MultiConfig scene) {
    std::unordered_map<std::string, Entity*> entityMap;

    // std::unordered_map<utils::uuid_t, utils::uuid_t> idMap;

    auto scripts = scene.getPage("Scripts");

    for (const auto& section : scripts->sections()) {
        ScriptDef def;
        def.name = section;
        def.path = scripts->getRaw(section, "path");
        def.extension = scripts->getRaw(section, "extension");
        def.libPath = scripts->getRaw(section, "libPath");
        addScript(ScriptFactory::Create(def));
    }

    std::cout << "Loaded script\n";

    std::unordered_map<utils::uuid_t, hg::Entity*> idMap;

    for (const auto& section : scene.getPage("Entities")->sections()) {
        auto entity = entities.add();
        idMap.insert(std::make_pair(std::stol(section), entity));
        entity->name = scene.getPage("Entities")->getRaw(section, "name");
        scene.getPage("Entities")->getArray<float, 3>(section, "position", entity->transform.position.vector);
        scene.getPage("Entities")->getArray<float, 3>(section, "scale", entity->transform.scale.vector);
        scene.getPage("Entities")->getArray<float, 4>(section, "rotation", entity->transform.rotation.vector);
        entityMap.insert(std::make_pair(section, entity));
    }

    std::cout << "Loaded Entities\n";

    for (const auto& section : scene.getPage("Entities")->sections()) {
        if (scene.getPage("Entities")->has(section, "parent")) {
            //auto parent = entities.get();
            auto entity = idMap.at(std::stol(section));
            idMap.at(scene.getPage("Entities")->get<hg::utils::uuid_t>(section, "parent"))->addChild(entity);
        }
    }

    for (const auto& section : scene.getPage("Components")->sections()) {
        std::cout << section << "\n";
        auto parts = utils::s_split(section, '_');
        auto name = parts[0];
        auto entityId = std::stol(parts[1]);

        auto componentDef = ComponentFactory::Get(name);
        auto component = componentDef.attach(idMap.at(entityId));

        std::cout << componentDef.name << "\n";

        for (const auto& field : ComponentFactory::GetFields(name)) {
            auto value = scene.getPage("Components")->getRaw(section, field.field);
            std::cout << field.field << " = " << value << "\n";
            field.setter(component, utils::deserialize(field.type, value));
            std::cout << "Success\n";
        }

        component->load(scene.getPage("Components"), section);
    }

    std::cout << "Loaded Components\n";
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
