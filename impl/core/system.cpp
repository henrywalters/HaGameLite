//
// Created by henry on 12/12/23.
//
#include "../../../include/hagame/core/system.h"

using namespace hg;

std::unique_ptr<std::unordered_map<std::string, SystemFactory::RegisteredSystem>> SystemFactory::s_systems;

std::vector<SystemFactory::RegisteredSystem> SystemFactory::All() {
    std::vector<RegisteredSystem> systems;
    for (const auto& [name, system] : SystemMap()) {
        systems.push_back(system);
    }
    return systems;
}

SystemFactory::RegisteredSystem SystemFactory::Get(std::string name) {
    return SystemMap()[name];
}

std::unordered_map<std::string, SystemFactory::RegisteredSystem> &SystemFactory::SystemMap() {
    if (!s_systems) {
        s_systems = std::make_unique<std::unordered_map<std::string, RegisteredSystem>>();
    }
    return *s_systems;
}