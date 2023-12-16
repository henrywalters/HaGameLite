//
// Created by henry on 2/18/23.
//


#include "../../../include/hagame/core/scene.h"

#include <box2d/box2d.h>

hg::utils::uuid_t hg::System::id = 0;

hg::utils::MultiConfig hg::Scene::save() {
    hg::utils::MultiConfig scene;
    scene.addPage("Scripts");
    scene.addPage("Entities");
    scene.addPage("Components");

    entities.forEach([&](auto entity) {

        if (entity == nullptr || entity->name == "root") {
            return;
        }

        scene.getPage("Entities")->addSection(std::to_string(entity->id()));
        scene.getPage("Entities")->setRaw(std::to_string(entity->id()), "name", entity->name);
        scene.getPage("Entities")->setArray<float, 3>(std::to_string(entity->id()), "position", entity->transform.position.vector);
        scene.getPage("Entities")->setArray<float, 3>(std::to_string(entity->id()), "scale", entity->transform.scale.vector);
        scene.getPage("Entities")->setArray<float, 4>(std::to_string(entity->id()), "rotation", entity->transform.rotation.vector);

        if (entity->parent() && entity->parent()->id() != entities.root->id()) {
            scene.getPage("Entities")->set<int>(std::to_string(entity->id()), "parent", entity->parent()->id());
        }

        for (hg::Component* component : entity->components()) {
            auto id = component->className() + "_" + std::to_string(entity->id()) + "_" + std::to_string(component->id());
            scene.getPage("Components")->addSection(id);
            for (const auto& field : ComponentFactory::GetFields(component->className())) {
                auto value = field.getter(component);
                scene.getPage("Components")->setRaw(id, field.field, hg::utils::serialize(value));
            }
        }
    });

    return scene;
}

void hg::Scene::load(hg::utils::MultiConfig scene) {
    std::unordered_map<std::string, Entity*> entityMap;

    std::unordered_map<utils::uuid_t, utils::uuid_t> idMap;

    for (const auto& section : scene.getPage("Entities")->sections()) {
        auto entity = entities.add();
        idMap.insert(std::make_pair(std::stol(section), entity->id()));
        entity->name = scene.getPage("Entities")->getRaw(section, "name");
        scene.getPage("Entities")->getArray<float, 3>(section, "position", entity->transform.position.vector);
        scene.getPage("Entities")->getArray<float, 3>(section, "scale", entity->transform.scale.vector);
        scene.getPage("Entities")->getArray<float, 4>(section, "rotation", entity->transform.rotation.vector);
        entityMap.insert(std::make_pair(section, entity));
    }

    for (const auto& section : scene.getPage("Entities")->sections()) {
        if (scene.getPage("Entities")->has(section, "parent")) {
            auto parent = entities.get(idMap[scene.getPage("Entities")->get<int>(section, "parent")]);
            auto entity = entities.get(idMap[std::stol(section)]);
            parent->addChild(entity);
        }
    }

    for (const auto& section : scene.getPage("Components")->sections()) {
        auto parts = utils::s_split(section, '_');
        std::cout << section << "\n";
        auto name = parts[0];
        auto entityId = std::stol(parts[1]);

        auto componentDef = ComponentFactory::Get(name);
        auto component = componentDef.attach(entities.get(idMap[entityId]));

        for (const auto& field : ComponentFactory::GetFields(name)) {
            auto value = scene.getPage("Components")->getRaw(section, field.field);
            std::cout << field.field << "(" << field.type << ") = " << value << "\n";
            field.setter(component, utils::deserialize(field.type, value));
        }
    }
}

void hg::Scene::init() {

    /*
    b2AABB worldAABB;
    worldAABB.lowerBound.Set(-200, -100);
    worldAABB.upperBound.Set(200, 500);
    m_physics2d = new b2World(b2Vec2(0, -9.8));
    m_physics2d->ClearForces();
    */

    onInit();

    for (const auto& [key, system] : m_systems) {
        system->onInit();
    }
}
