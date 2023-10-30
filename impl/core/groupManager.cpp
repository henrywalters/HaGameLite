//
// Created by henry on 8/18/23.
//
#include "../../../include/hagame/core/groupManager.h"

void hg::GroupManager::addToGroup(std::string group, hg::Entity *entity) {
    if (m_entityToGroups.find(entity) == m_entityToGroups.end()) {
        m_entityToGroups.insert(std::make_pair(entity, std::vector<std::string>()));
    }

    if (m_groupToEntities.find(group) == m_groupToEntities.end()) {
        m_groupToEntities.insert(std::make_pair(group, std::vector<hg::Entity*>()));
    }

    m_entityToGroups[entity].push_back(group);
    m_groupToEntities[group].push_back(entity);
}

void hg::GroupManager::removeFromGroup(std::string group, hg::Entity *entity) {
    if (m_entityToGroups.find(entity) != m_entityToGroups.end()) {
        m_entityToGroups.erase(entity);
    }

    if (m_groupToEntities.find(group) != m_groupToEntities.end()) {
        m_groupToEntities[group].erase(std::remove_if(
            m_groupToEntities[group].begin(),
            m_groupToEntities[group].end(),
            [entity](hg::Entity* other) {
                return other == entity;
            }
            ), m_groupToEntities[group].end());
    }
}

void hg::GroupManager::removeEntity(hg::Entity *entity) {
    for (const auto& group : getGroups(entity)) {
        removeFromGroup(group, entity);
    }
}

bool hg::GroupManager::inGroup(hg::Entity *entity, std::string group) {
    auto groups = getGroups(entity);
    return std::find(groups.begin(), groups.end(), group) != groups.end();
}

std::vector<std::string> hg::GroupManager::getGroups(hg::Entity *entity) {
    return m_entityToGroups.find(entity) == m_entityToGroups.end() ? std::vector<std::string>() : m_entityToGroups[entity];
}

std::vector<hg::Entity *> hg::GroupManager::getEntities(std::string group) {
    return m_groupToEntities.find(group) == m_groupToEntities.end() ? std::vector<hg::Entity*>() : m_groupToEntities[group];
}

bool hg::GroupManager::inOneOfGroups(hg::Entity *entity, std::vector<std::string> groups) {
    for (const auto& group : groups) {
        if (inGroup(entity, group)) {
            return true;
        }
    }
    return false;
}
