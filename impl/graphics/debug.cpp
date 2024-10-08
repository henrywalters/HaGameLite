//
// Created by henry on 4/30/23.
//

#include "../../include/hagame/graphics/debug.h"
#include "../../include/hagame/utils/clock.h"
#include "../../include/hagame/math/constants.h"
#include "../../include/hagame/math/constants.h"

bool hg::graphics::Debug::ENABLED = true;

void hg::graphics::Debug::Initialize(hg::graphics::ShaderProgram* shader, ShaderProgram* textShader, Font* font) {
    s_shader = shader;
    s_textShader = textShader;
    s_quad = std::make_unique<primitives::Quad>(Vec2::Zero(), Vec2::Zero());
    s_quadMesh = std::make_unique<MeshInstance>(s_quad.get());
    s_cube = std::make_unique<primitives::Cube>(Vec3::Identity());
    s_cubeMesh = std::make_unique<MeshInstance>(s_cube.get());
    s_disc = std::make_unique<primitives::Disc>(0, hg::graphics::DEBUG_CIRCLE_DIVISIONS);
    s_discMesh = std::make_unique<MeshInstance>(s_disc.get());
    s_line = std::make_unique<primitives::Line>();
    s_lineMesh = std::make_unique<MeshInstance>(s_line.get());
    s_text = std::make_unique<Text>();
    s_font = font;
}

void hg::graphics::Debug::FillRect(float x, float y, float width, float height, hg::graphics::Color color, double duration) {

    if (!Debug::ENABLED) return;

    s_calls.emplace_back(Call{[x, y, width, height, color](){
        s_quad->size(Vec2(width, height));
        s_shader->use();
        s_shader->setMat4("model", Mat4::Translation(Vec3(x, y, 0)));
        s_shader->setVec4("color", color);
        s_quadMesh->update(s_quad.get());
        s_quadMesh->render();
    },duration, utils::Clock::Now()});
}

void hg::graphics::Debug::FillCircle(float x, float y, float radius, hg::graphics::Color color, double duration) {
    if (!Debug::ENABLED) return;
    s_calls.emplace_back(Call{[x, y, radius, color](){
        s_disc->radius(radius);
        s_shader->use();
        s_shader->setMat4("model", Mat4::Translation(Vec3(x, y, 0)));
        s_shader->setVec4("color", color);
        s_discMesh->update(s_disc.get());
        s_discMesh->render();
    }, duration, utils::Clock::Now()});
}

void hg::graphics::Debug::FillRect(hg::Rect rect, hg::graphics::Color color, double duration) {
    if (!Debug::ENABLED) return;
    FillRect(rect.pos[0], rect.pos[1], rect.size[0], rect.size[1], color, duration);
}

void hg::graphics::Debug::DrawLine(float x1, float y1, float x2, float y2, hg::graphics::Color color, float thickness, double duration) {
    if (!Debug::ENABLED) return;
    s_calls.emplace_back(Call{[x1, y1, x2, y2, color, thickness](){
        s_line->clearPoints();
        s_line->addPoints({Vec3(x1, y1, 0), Vec3(x2, y2, 0)});
        s_line->thickness(thickness);
        s_lineMesh->update(s_line.get());
        s_shader->use();
        s_shader->setMat4("model", Mat4::Translation(Vec3(0, 0, 0)));
        s_shader->setVec4("color", color);
        s_lineMesh->render();
    }, duration, utils::Clock::Now()});
}

void
hg::graphics::Debug::DrawLine(hg::Vec2 p1, hg::Vec2 p2, hg::graphics::Color color, float thickness, double duration) {
    DrawLine(p1.x(), p1.y(), p2.x(), p2.y(), color, thickness, duration);
}

void
hg::graphics::Debug::DrawRect(float x, float y, float width, float height, hg::graphics::Color color, float thickness, double duration) {
    if (!Debug::ENABLED) return;
    s_calls.emplace_back(Call{[x, y, width, height, color, thickness](){
        s_line->clearPoints();
        s_line->thickness(thickness);
        s_line->addPoints({
              Vec3(x, y, 0),
              Vec3(x + width, y, 0),
              Vec3(x + width, y + height, 0),
              Vec3(x, y + height, 0),
              Vec3(x, y, 0)
          });
        s_lineMesh->update(s_line.get());
        s_shader->use();
        s_shader->setMat4("model", Mat4::Translation(Vec3(0, 0, 0)));
        s_shader->setVec4("color", color);
        s_lineMesh->render();
    }, duration, utils::Clock::Now()});
}

void hg::graphics::Debug::DrawCircle(float x, float y, float radius, hg::graphics::Color color, float thickness, double duration) {
    if (!Debug::ENABLED) return;
    s_calls.emplace_back(Call{[x, y, radius, color, thickness](){
        s_line->clearPoints();
        s_line->thickness(thickness);
        float step = (2 * math::PI) / hg::graphics::DEBUG_CIRCLE_DIVISIONS;
        for (int i = 0; i <= hg::graphics::DEBUG_CIRCLE_DIVISIONS; i++) {
            float theta = i * step;
            s_line->addPoint(Vec3(radius * cos(theta), radius * sin(theta), 0));
        }
        s_lineMesh->update(s_line.get());
        s_shader->use();
        s_shader->setMat4("model", Mat4::Translation(Vec3(x, y, 0)));
        s_shader->setVec4("color", color);
        s_lineMesh->render();
    }, duration, utils::Clock::Now()});
}

void hg::graphics::Debug::DrawLine(hg::math::LineSegment line, hg::graphics::Color color, float thickness, double duration) {
    if (!Debug::ENABLED) return;
    DrawLine(line.a[0], line.a[1], line.b[0], line.b[1], color, thickness, duration);
}

void hg::graphics::Debug::DrawRect(hg::Rect rect, hg::graphics::Color color, float thickness, double duration) {
    if (!Debug::ENABLED) return;
    DrawRect(rect.pos[0], rect.pos[1], rect.size[0], rect.size[1], color, thickness, duration);
}

void hg::graphics::Debug::DrawPath(std::vector<hg::Vec3> points, hg::graphics::Color color, float thickness, double duration) {
    if (!Debug::ENABLED) return;
    s_calls.emplace_back(Call{[points, color, thickness](){
        s_line->clearPoints();
        s_line->addPoints(points);
        s_line->thickness(thickness);
        s_lineMesh->update(s_line.get());
        s_shader->use();
        s_shader->setMat4("model", Mat4::Translation(Vec3(0, 0, 0)));
        s_shader->setVec4("color", color);
        s_lineMesh->render();
    }, duration, utils::Clock::Now()});
}

void hg::graphics::Debug::DrawRay(hg::math::Ray ray, hg::graphics::Color color, float triangleSize, float thickness, double duration) {
    if (!Debug::ENABLED) return;
    s_calls.emplace_back(Call{[ray, color, triangleSize, thickness](){
        auto end =  ray.origin + ray.direction;
        DrawLine(math::LineSegment(ray.origin, end), color, thickness);
    }, duration, utils::Clock::Now()});
}

void hg::graphics::Debug::DrawTriangle(hg::Vec3 a, hg::Vec3 b, hg::Vec3 c, hg::graphics::Color color, float thickness, double duration) {
    if (!Debug::ENABLED) return;
    DrawPath({a, b, c, a}, color, thickness, duration);
}

void hg::graphics::Debug::DrawText(hg::Vec3 pos, std::string text, Color color, TextHAlignment alignmentH, TextVAlignment alignmentV, double duration) {
    if (!Debug::ENABLED) return;
    s_calls.emplace_back(Call{[pos, text, color, alignmentH, alignmentV](){
        s_textShader->use();
        s_textShader->setMat4("model", Mat4::Identity());
        s_textShader->setVec4("textColor", color);
        s_text->draw(s_font, text, pos, alignmentH, alignmentV);
    }, duration, utils::Clock::Now()});
}

void hg::graphics::Debug::Render() {
    try {
        std::vector<Call> tmpCalls;
        for (const auto &call: s_calls) {
            call.call();
            if (utils::Clock::ToSeconds(utils::Clock::Now() - call.calledAt) < call.duration) {
                tmpCalls.push_back(call);
            }
        }
        s_calls = tmpCalls;
    } catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }
}

void hg::graphics::Debug::FillCube(hg::Cube cube, hg::graphics::Color color, double duration) {
    if (!Debug::ENABLED) return;
    s_calls.emplace_back(Call{[cube, color](){
        s_shader->use();
        s_shader->setMat4("model", Mat4::Translation(cube.pos) * Mat4::Scale(cube.size));
        s_shader->setVec4("color", color);
        s_cubeMesh->render();
    }});
}


