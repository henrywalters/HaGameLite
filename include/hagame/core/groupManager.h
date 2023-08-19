//
// Created by henry on 8/18/23.
//

#ifndef HAGAME2_GROUPMANAGER_H
#define HAGAME2_GROUPMANAGER_H

#include <unordered_map>
#include <vector>
#include <string>

namespace hg {

    class Entity;

    class GroupManager {
    public:

        void addToGroup(std::string group, hg::Entity* entity);
        void removeFromGroup(std::string group, hg::Entity* entity);
        void removeEntity(hg::Entity* entity);
        bool inGroup(hg::Entity* entity, std::string group);
        std::vector<std::string> getGroups(hg::Entity* entity);
        std::vector<hg::Entity*> getEntities(std::string group);

    private:

        std::unordered_map<std::string, std::vector<hg::Entity*>> m_groupToEntities;
        std::unordered_map<hg::Entity*, std::vector<std::string>> m_entityToGroups;

    };
}


#endif //HAGAME2_GROUPMANAGER_H
