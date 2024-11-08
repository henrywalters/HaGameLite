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
#include "primitives/cube.h"

namespace hg::graphics {

    const int DEBUG_CIRCLE_DIVISIONS = 20;

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

        static void FillRect(Rect rect, Color color, double duration = 0.0);
        static void FillRect(float x, float y, float width, float height, Color color, double duration = 0.0);
        static void FillCircle(float x, float y, float radius, Color color, double duration = 0.0);
        static void FillCube(Cube cube, Color color, double duration = 0.0);

        static void DrawLine(float x1, float y1, float x2, float y2, Color color, float thickness = 1.0f, double duration = 0.0);
        static void DrawLine(math::LineSegment line, Color color, float thickness = 1.0f, double duration = 0.0);
        static void DrawLine(Vec2 p1, Vec2 p2, Color color, float thickness = 1.0f, double duration = 0.0);
        static void DrawPath(std::vector<hg::Vec3> points, Color color, float thickness, double duration = 0.0);
        static void DrawRect(float x, float y, float width, float height, Color color, float thickness = 1.0f, double duration = 0.0);
        static void DrawRect(Rect rect, Color color, float thickness = 1.0f, double duration = 0.0);
        static void DrawRotatedRect(float x, float y, float width, float height, const Quat& rotation, Color color, float thickness = 1.0f, double duration = 0.0);
        static void DrawRotatedRect(Rect rect, const Quat& rotation, Color color, float thickness = 1.0f, double duration = 0.0);
        static void DrawTriangle(hg::Vec3 a, hg::Vec3 b, hg::Vec3 c, Color color, float thickness = 1.0f, double duration = 0.0);
        static void DrawRay(math::Ray ray, Color color, float triangleSize = 5.0f, float thickness = 1.0f, double duration = 0.0);
        static void DrawCircle(float x, float y, float radius, Color color, float thickness = 1.0f, double duration = 0.0);
        static void DrawText(hg::Vec3 pos, std::string text, Color color, TextHAlignment alignmentH = TextHAlignment::Left, TextVAlignment alignmentV = TextVAlignment::Top, double duration = 0.0);


        static void Render();

    private:

        struct Call {
            std::function<void()> call;
            double duration;
            long long calledAt;
        };

        static inline ShaderProgram* s_shader;
        static inline ShaderProgram* s_textShader;

        static inline std::unique_ptr<primitives::Quad> s_quad;
        static inline std::unique_ptr<MeshInstance> s_quadMesh;

        static inline std::unique_ptr<primitives::Cube> s_cube;
        static inline std::unique_ptr<MeshInstance> s_cubeMesh;

        static inline std::unique_ptr<primitives::Disc> s_disc;
        static inline std::unique_ptr<MeshInstance> s_discMesh;

        static inline std::unique_ptr<primitives::Line> s_line;
        static inline std::unique_ptr<MeshInstance> s_lineMesh;

        static inline std::unique_ptr<Text> s_text;
        static inline Font* s_font;

        static inline std::vector<Call> s_calls;

    };
}

#endif //HAGAME2_DEBUG_H
