//
// Created by henry on 12/28/22.
//

#include <iostream>
#include "../../../include/hagame/core/gameObject.h"

void hg::GameObject::addChild(GameObject *child) {

    if (child->m_parent != nullptr) {
        
        child->m_parent->removeChild(child);
    }
    child->m_parent = this;
    m_children.push_back(child);
}

void hg::GameObject::removeChild(GameObject *child) {

    bool hadChild = false;

    for (int i = 0; i < m_children.size(); i++) {
        if (m_children[i] == child) {
            m_children.erase(m_children.begin() + i);
            hadChild = true;
        }

        if (hadChild) {
            break;
        }
    }

    child->m_parent = nullptr;
}