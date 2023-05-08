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

namespace hg::graphics {

    const int DEBUG_CIRCLE_DIVISIONS = 10;

    class Debug {
    public:

        static void Initialize(ShaderProgram* shader);

        static void FillRect(Rect rect, Color color);
        static void FillRect(float x, float y, float width, float height, Color color);
        static void FillCircle(float x, float y, float radius, Color color);

        static void DrawLine(float x1, float y1, float x2, float y2, Color color, float thickness = 1.0f);
        static void DrawLine(math::LineSegment line, Color color, float thickness = 1.0f);
        static void DrawRect(float x, float y, float width, float height, Color color, float thickness = 1.0f);
        static void DrawRect(Rect rect, Color color, float thickness = 1.0f);
        static void DrawCircle(float x, float y, float radius, Color color, float thickness = 1.0f);

    private:

        static inline ShaderProgram* s_shader;

        static inline std::unique_ptr<primitives::Quad> s_quad;
        static inline std::unique_ptr<MeshInstance> s_quadMesh;

        static inline std::unique_ptr<primitives::Disc> s_disc;
        static inline std::unique_ptr<MeshInstance> s_discMesh;

        static inline std::unique_ptr<primitives::Line> s_line;
        static inline std::unique_ptr<MeshInstance> s_lineMesh;

    };
}

#endif //HAGAME2_DEBUG_H
