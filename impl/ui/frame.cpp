//
// Created by henry on 8/3/24.
//
#include "../../../include/hagame/ui/frame.h"
#include "../../../include/hagame/graphics/debug.h"

using namespace hg::graphics;

hg::ui::Frame::Frame(hg::Rect rect):
    m_rect(rect)
{}

hg::ui::Container *hg::ui::Frame::root() {
    return &m_root;
}

void hg::ui::Frame::render(double dt, bool debug) {
    initializeShader(&m_context.colorShader);
    initializeShader(&m_context.textShader);
    initializeShader(&m_context.textBufferShader);
    initializeShader(&m_context.textureShader);

    structures::Tree::BreadthFirstTraverse(&m_root, [&](auto node) {
        auto ui = static_cast<Element*>(node);
        if (debug) {
            auto rect = ui->getRect(m_rect);
            Debug::DrawRect(rect, Color::blue(), 1);
        }
        ui->render(&m_context, m_rect, dt);
        return true;
    });
}

void hg::ui::Frame::initializeShader(hg::graphics::ShaderProgram *shader) {
    shader->use();
    shader->setMat4("view", Mat4::Identity());
    shader->setMat4("projection", Mat4::Orthographic(m_rect.pos[0], m_rect.pos[0] + m_rect.size[0], m_rect.pos[1], m_rect.pos[1] + m_rect.size[1], -100, 100));
    shader->setMat4("model", Mat4::Identity());
}

bool hg::ui::Frame::contains(hg::ui::Element *element, hg::Vec2 pos) const {
    return element->contains(m_rect, pos);
}

void hg::ui::Frame::update(Vec2 mousePos, double dt) {
    structures::Tree::BreadthFirstTraverse(&m_root, [&](auto node) {
        auto ui = static_cast<Element*>(node);
        if (contains(ui, mousePos)) {
            if (!m_hovered.contains(ui->id())) {
                m_hovered.insert(ui->id());
                ui->events.emit(UIEvents::MouseEnter);
            }
        } else {
            if (m_hovered.contains(ui->id())) {
                m_hovered.erase(ui->id());
                ui->events.emit(UIEvents::MouseLeave);
            }
        }
        return true;
    });
}
