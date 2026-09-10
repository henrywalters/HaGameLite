//
// Created by henry on 12/1/23.
//

#ifndef HAGAME2_LIGHT_H
#define HAGAME2_LIGHT_H

#include "../mesh.h"
#include "../triangle.h"
#include "../../utils/macros.h"
#include "../../math/lineSegment.h"

namespace hg::graphics::primitives {

    struct LightTriangles {
        std::vector<math::LineSegment> triangles;

        bool operator==(const LightTriangles &other) const {
            if (triangles.size() != other.triangles.size()) return false;
            for (int i = 0; i < triangles.size(); ++i) {
                if (triangles[i] != other.triangles[i]) return false;
            }
            return true;
        }

        bool operator!=(const LightTriangles &other) const {
            return !(*this == other);
        }
    };

    class Light : public Mesh {
    public:

        Light(LightTriangles triangles, Vec3 origin, float radius);

        HG_GET_SET(LightTriangles, triangles, update);
        HG_GET_SET(Vec3, origin, update);
        HG_GET_SET(float, radius, update);

    protected:

        void computeMesh() override;

    private:

        LightTriangles m_triangles;
        Vec3 m_origin;
        float m_radius;

    };
}

#endif //HAGAME2_LIGHT_H
