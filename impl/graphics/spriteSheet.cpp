//
// Created by henry on 11/5/23.
//
#include <array>
#include "../../../include/hagame/graphics/spriteSheet.h"
#include "../../../include/hagame/utils/helpers.h"
#include "../../include/hagame/core/assets.h"

using namespace hg;
using namespace hg::graphics;


SpriteSheet::SpriteSheet(utils::MultiConfig &config):
    m_texture(hg::Vec2i::Zero())
{
    load(config);
    m_texture = Texture(m_path);
}


void SpriteSheet::activateGroup(std::string group) {
    if (!m_selectedGroup.has_value() || m_selectedGroup.value() != group) {
        m_selectedGroup = group;
        reset();
    }
}

void SpriteSheet::addGroup(std::string group) {
    if (!utils::contains(m_groups, group)) {
        m_groups.insert(std::make_pair(group, SpriteGroup()));
    }
    if (m_groups.size() == 1) {
        activateGroup(group);
    }
}

void SpriteSheet::addToGroup(std::string group, Vec2i index) {
    addGroup(group);
    if (std::find(m_groups[group].members.begin(), m_groups[group].members.end(), index) != m_groups[group].members.end()) {
        return;
    }
    m_groups[group].members.push_back(index);
}

void SpriteSheet::addToGroup(hg::Vec2i index) {
    if (!m_selectedGroup.has_value()) {
        return;
    }
    addToGroup(m_selectedGroup.value(), index);
}

SpriteGroup *SpriteSheet::getGroup(std::string group) {
    if (!utils::contains(m_groups, group)) {
        return nullptr;
    }
    return &m_groups[group];
}

void SpriteSheet::removeFromGroup(std::string group, hg::Vec2i index) {
    if (utils::contains(m_groups, group)) {
        utils::v_remove(m_groups[group].members, index);
    }
}

void SpriteSheet::removeFromGroup(hg::Vec2i index) {
    if (m_selectedGroup.has_value()) {
        removeFromGroup(m_selectedGroup.value(), index);
    }
}

std::unordered_map<std::string, SpriteGroup> *hg::graphics::SpriteSheet::groups() {
    return &m_groups;
}

hg::graphics::Texture *SpriteSheet::texture() {
    return &m_texture;
}

void SpriteSheet::save(utils::MultiConfig &config) {
    config.addPage("SpriteSheet");
    auto page = config.getPage("SpriteSheet");
    page->addSection("settings");
    page->setRaw("settings", "path", m_path);
    page->setArray<int, 2>("settings", "size", size.vector);
    page->set<bool>("settings", "animated", animated);
    page->set<int>("settings", "fps", fps);
    page->set<int>("settings", "frameCount", frameCount);
    page->set<bool>("settings", "looping", looping);

    config.addPage("Groups");
    for (const auto& [name, group] : m_groups) {
        config.getPage("Groups")->addSection(name);
        std::vector<std::array<int, 2>> members;

        for (const auto& member : group.members) {
            members.push_back(std::array<int, 2>({member.vector[0], member.vector[1]}));
        }

        config.getPage("Groups")->setListOfArrays<int, 2>(name, "members", members);
    }
}

void SpriteSheet::load(utils::MultiConfig &config) {
    auto page = config.getPage("SpriteSheet");
    page->getArray<int, 2>("settings", "size", size.vector);
    animated = page->get<bool>("settings", "animated");
    fps = page->get<int>("settings", "fps");
    frameCount = page->get<int>("settings", "frameCount");
    m_path = ASSET_DIR + page->getRaw("settings", "path");
    looping = page->get<bool>("settings", "looping");

    for (const auto& name : config.getPage("Groups")->sections()) {
        SpriteGroup group;
        auto members = config.getPage("Groups")->getListOfArrays<int, 2>(name, "members");
        for (const auto& member : members) {
            group.members.push_back(hg::Vec2i(member[0], member[1]));
        }
        m_groups.insert(std::make_pair(name, group));
    }
}

hg::Rect SpriteSheet::getRect(hg::Vec2i index) {
    Vec2 imageSize = m_texture.image->size.cast<float>();
    Vec2 cellSize = m_texture.image->size.div(size).cast<float>();
    Vec2 cellPos = cellSize.prod(index.cast<float>()).div(imageSize);
    return Rect(cellPos, cellSize.div(imageSize));
}

hg::Rect SpriteSheet::getRect() {
    if (m_selectedGroup.has_value()) {
        if (m_groups[m_selectedGroup.value()].members.size() == 0) {
            return getRect(hg::Vec2i(0, 0));
        }
        return getRect(m_groups[m_selectedGroup.value()].members[m_currentIndex]);
    } else {
        int row = m_currentIndex / size[0];
        int col = m_currentIndex % size[0];
        return getRect(hg::Vec2i(col, row));
    }
}

void SpriteSheet::update(double dt) {

    if (!m_playing) {
        return;
    }

    double frameRate = 1.0 / (double) fps;
    m_elapsedTime += dt;
    int frame = m_elapsedTime / frameRate;

    int clampedCount;

    if (m_selectedGroup.has_value()) {
        std::cout << m_selectedGroup.value() << "\n";
        auto memberCount = members().size();
        std::cout << memberCount << "\n";
        clampedCount = frameCount < 0 ? 0 : (frameCount > memberCount ? memberCount : frameCount);
    } else {
        std::cout << "DEFAULT GROUP\n";
        clampedCount = frameCount < 0 ? 0 : (frameCount > size[0] * size[1] ? size[0] * size[1] : frameCount);
    }

    m_currentIndex = frame % clampedCount;

    if (frame > 0 && m_currentIndex == 0) {
        m_currentIndex = m_currentIndex % clampedCount;
        onFinish.emit(this);
    }
}

void SpriteSheet::play() {
    m_playing = true;
}

void SpriteSheet::stop() {
    m_playing = false;
}

void SpriteSheet::reset() {
    m_currentIndex = 0;
    m_elapsedTime = 0;
    if (!looping) {
        m_playing = false;
    }
}

std::vector<hg::Vec2i> SpriteSheet::members() {
    if (m_selectedGroup.has_value()) {
        return m_groups[m_selectedGroup.value()].members;
    }
    return std::vector<hg::Vec2i>();
}

std::optional<std::string> SpriteSheet::activeGroup() {
    return m_selectedGroup;
}


