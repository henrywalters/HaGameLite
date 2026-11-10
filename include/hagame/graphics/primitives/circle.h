//
// Created by henry on 12/22/22.
//

#ifndef HAGAME2_CIRCLE_PRIM_H
#define HAGAME2_CIRCLE_PRIM_H

#include "../mesh.h"
#include "../../utils/macros.h"

namespace hg::graphics::primitives {
    class Circle : public Mesh {
    public:

        Circle(float radius, int divisions, float thickness = 1.0, Vec3 offset = Vec3::Zero()):
            m_radius(radius),
            m_divisions(divisions),
            m_offset(offset),
            m_thickness(thickness)
        {
            computeMesh();
        }

        HG_GET_SET(float, radius, computeMesh);
        HG_GET_SET(int, divisions, computeMesh);
        HG_GET_SET(Vec3, offset, computeMesh);
        HG_GET_SET(float, thickness, computeMesh);

    private:

        float m_radius;
        float m_thickness;
        int m_divisions;
        Vec3 m_offset;

        void computeMesh();
    };
}

#endif //HAGAME2_DISC_H
