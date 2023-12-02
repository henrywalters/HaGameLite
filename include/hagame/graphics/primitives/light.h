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
    class Light : public Mesh {
    public:

        Light(std::vector<math::LineSegment> triangles, Vec3 origin, float radius);

        HG_GET_SET(std::vector<math::LineSegment>, triangles, update);
        HG_GET_SET(Vec3, origin, update);
        HG_GET_SET(float, radius, update);

    protected:

        void computeMesh() override;

    private:

        std::vector<math::LineSegment> m_triangles;
        Vec3 m_origin;
        float m_radius;

    };
}

#endif //HAGAME2_LIGHT_H
