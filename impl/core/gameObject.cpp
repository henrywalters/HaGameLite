//
// Created by henry on 12/28/22.
//

#include <iostream>
#include "../../../include/hagame/core/gameObject.h"

using namespace hg;

void GameObject::addChild(GameObject *child) {

    if (child->m_parent != nullptr) {
        
        child->m_parent->removeChild(child);
    }
    child->m_parent = this;
    m_children.push_back(child);
}

void GameObject::removeChild(GameObject *child) {

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

Vec3 GameObject::position() {
    Vec3 pos;
    GameObject* node = this;

    while (node != nullptr) {
        pos += node->transform.position;
        node = node->parent();
    }

    return pos;
}

hg::Vec3 GameObject::scale() {
    Vec3 scale = Vec3::Identity();
    GameObject* node = this;

    while (node != nullptr) {
        scale = scale.prod(node->transform.scale);
        node = node->parent();
    }

    return scale;
}

hg::Quat GameObject::rotation() {
    Quat rotation = Quat(0, Vec3(1, 0, 0));
    GameObject* node = this;

    while (node != nullptr) {
        rotation = node->transform.rotation * rotation;
        node = node->parent();
    }

    return rotation;
}

Mat4 GameObject::model() {
    return Mat4::Translation(position()) * Mat4::Rotation(rotation()) * Mat4::Scale(scale());
}
