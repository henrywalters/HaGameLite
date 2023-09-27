//
// Created by henry on 2/18/23.
//


#include "../../../include/hagame/core/scene.h"

#include <box2d/box2d.h>


hg::utils::MultiConfig hg::Scene::save() {
    hg::utils::MultiConfig scene;
    scene.addPage("Scripts");
    scene.addPage("Entities");
    scene.addPage("Components");

    std::cout << "ADDED PAGES\n";

    for (const auto& script : scripts) {

    }

    entities.forEach([&](auto entity) {

        if (entity == nullptr) {
            return;
        }

        std::cout << "ENTITY = " << entity << "\n";
        scene.getPage("Entities")->addSection(std::to_string(entity->id()));
        scene.getPage("Entities")->setRaw(std::to_string(entity->id()), "name", entity->name);
        scene.getPage("Entities")->setArray<float, 3>(std::to_string(entity->id()), "position", entity->transform.position.vector);
        scene.getPage("Entities")->setArray<float, 3>(std::to_string(entity->id()), "scale", entity->transform.scale.vector);
        scene.getPage("Entities")->setArray<float, 4>(std::to_string(entity->id()), "rotation", entity->transform.rotation.vector);

        for (hg::Component* component : entity->components()) {
            auto id = ((std::string)*component) + "_" + std::to_string(entity->id()) + "_" + std::to_string(component->id());
            scene.getPage("Components")->addSection(id);
            component->save(scene.getPage("Components"), id);
        }
    });

    return scene;
}

void hg::Scene::load(hg::utils::MultiConfig scene) {
    std::unordered_map<std::string, Entity*> entityMap;

    for (const auto& section : scene.getPage("Entities")->sections()) {
        auto entity = entities.add();
        entity->name = scene.getPage("Entities")->getRaw(section, "name");
        scene.getPage("Entities")->getArray<float, 3>(section, std::to_string(entity->id()), entity->transform.position.vector);
        scene.getPage("Entities")->getArray<float, 3>(section, std::to_string(entity->id()), entity->transform.scale.vector);
        scene.getPage("Entities")->getArray<float, 4>(section, std::to_string(entity->id()), entity->transform.rotation.vector);
        entityMap.insert(std::make_pair(section, entity));
    }

    std::cout << "LOADING COMPONENTS\n";

    for (const auto& section : scene.getPage("Components")->sections()) {
        auto parts = utils::s_split(section, '_');
        std::cout << parts[0] << "\n";
        entityMap[parts[1]]->addComponent(parts[0]);
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
