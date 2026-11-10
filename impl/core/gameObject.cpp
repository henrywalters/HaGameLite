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
    if (parent() == nullptr) {
        return transform.position;
    }
    Vec3 parentPos = parent()->position();
    Quat parentRot = parent()->rotation();
    Vec3 parentScale = parent()->scale();
    return parentPos + parentRot.rotatePoint(parentScale.prod(transform.position));
}

hg::Vec3 GameObject::scale() {
    if (parent() == nullptr) {
        return transform.scale;
    }
    return parent()->scale().prod(transform.scale);
}

hg::Quat GameObject::rotation() {
    if (parent() == nullptr) {
        return transform.rotation;
    }
    return parent()->rotation() * transform.rotation;
}

hg::Vec3 GameObject::renderPosition() {
    // positionOffset is a render-only nudge (e.g. physics interpolation between fixed steps); children inherit it
    Vec3 offset = transform.positionOffset;
    for (auto node = parent(); node != nullptr; node = node->parent()) {
        offset += node->transform.positionOffset;
    }
    return position() + offset;
}

Mat4 GameObject::model() {
    return Mat4::Translation(renderPosition()) * Mat4::Rotation(rotation()) * Mat4::Scale(scale());
}

GameObject *GameObject::getChildByName(std::string name) {
    for (const auto& child : m_children) {
        if (child->name == name) {
            return child;
        }
    }

    return nullptr;
}
