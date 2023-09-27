//
// Created by henry on 4/30/23.
//

#ifndef HAGAME2_DEBUG_H
#define HAGAME2_DEBUG_H

#include "shaderProgram.h"
#include "color.h"
#include "font.h"
#include "text.h"
#include "mesh.h"
#include "primitives/quad.h"
#include "primitives/disc.h"
#include "primitives/line.h"
#include "../math/aliases.h"
#include "../math/lineSegment.h"
#include "../math/ray.h"

namespace hg::graphics {

    const int DEBUG_CIRCLE_DIVISIONS = 10;

    /*
     * The Debug class provides a simple way for rendering graphical primitives. Usage is simple.
     *
     * 1) At program startup, call the Initialize function to set the proper shaders and Font
     * 2) After every frame, call the Render method to flush all of the draw calls made
     */
    class Debug {
    public:

        static bool ENABLED;

        static void Initialize(ShaderProgram* shader, ShaderProgram* textShader, Font* font);

        static void FillRect(Rect rect, Color color);
        static void FillRect(float x, float y, float width, float height, Color color);
        static void FillCircle(float x, float y, float radius, Color color);

        static void DrawLine(float x1, float y1, float x2, float y2, Color color, float thickness = 1.0f);
        static void DrawLine(math::LineSegment line, Color color, float thickness = 1.0f);
        static void DrawPath(std::vector<hg::Vec3> points, Color color, float thickness);
        static void DrawRect(float x, float y, float width, float height, Color color, float thickness = 1.0f);
        static void DrawRect(Rect rect, Color color, float thickness = 1.0f);
        static void DrawTriangle(hg::Vec3 a, hg::Vec3 b, hg::Vec3 c, Color color, float thickness = 1.0f);
        static void DrawRay(math::Ray ray, Color color, float triangleSize = 5.0f, float thickness = 1.0f);
        static void DrawCircle(float x, float y, float radius, Color color, float thickness = 1.0f);
        static void DrawText(hg::Vec3 pos, std::string text, Color color, TextHAlignment alignmentH = TextHAlignment::Center, TextVAlignment alignmentV = TextVAlignment::Top);

        static void Render();

    private:

        static inline ShaderProgram* s_shader;
        static inline ShaderProgram* s_textShader;

        static inline std::unique_ptr<primitives::Quad> s_quad;
        static inline std::unique_ptr<MeshInstance> s_quadMesh;

        static inline std::unique_ptr<primitives::Disc> s_disc;
        static inline std::unique_ptr<MeshInstance> s_discMesh;

        static inline std::unique_ptr<primitives::Line> s_line;
        static inline std::unique_ptr<MeshInstance> s_lineMesh;

        static inline std::unique_ptr<Text> s_text;
        static inline Font* s_font;

        static inline std::vector<std::function<void()>> s_calls;

    };
}

#endif //HAGAME2_DEBUG_H
