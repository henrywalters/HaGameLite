//
// Created by henry on 8/3/24.
//
#include "../../../include/hagame/ui/frame.h"
#include "../../../include/hagame/graphics/debug.h"

using namespace hg::graphics;

hg::ui::Container *hg::ui::Frame::root() {
    return &m_root;
}

void hg::ui::Frame::render(double dt, bool debug) {
    initializeShader(&m_context.colorShader);
    initializeShader(&m_context.textShader);
    initializeShader(&m_context.textBufferShader);
    initializeShader(&m_context.textureShader);

    structures::Tree::BreadthFirstTraverse<Element>(&m_root, [&](Element* ui) {
        if (debug) {
            auto nodeRect = ui->getRect(rect);
            Debug::DrawRect(nodeRect, Color::blue(), 1);
        }
        ui->render(&m_context, rect, dt);
        return true;
    });
}

void hg::ui::Frame::initializeShader(hg::graphics::ShaderProgram *shader) {
    shader->use();
    shader->setMat4("view", Mat4::Identity());
    shader->setMat4("projection", Mat4::Orthographic(rect.pos[0], rect.pos[0] + rect.size[0], rect.pos[1], rect.pos[1] + rect.size[1], -100, 100));
    shader->setMat4("model", Mat4::Identity());
}

bool hg::ui::Frame::contains(hg::ui::Element *element, hg::Vec2 pos) const {
    return element->contains(rect, pos);
}

void hg::ui::Frame::update(Vec2 mousePos, double dt) {
    structures::Tree::BreadthFirstTraverse<Element>(&m_root, [&](auto ui) {
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