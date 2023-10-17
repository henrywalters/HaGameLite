//
// Created by henry on 12/22/22.
//

#ifndef HAGAME2_DISC_H
#define HAGAME2_DISC_H

#include "../mesh.h"
#include "../triangle.h"
#include "../../utils/macros.h"

namespace hg::graphics::primitives {
    class Disc : public Mesh {
    public:

        Disc(float radius, int divisions, Vec3 offset = Vec3::Zero()):
            m_radius(radius),
            m_divisions(divisions),
            m_offset(offset)
        {
            computeMesh();
        }

        HG_GET_SET(float, radius, computeMesh);
        HG_GET_SET(int, divisions, computeMesh);
        HG_GET_SET(Vec3, offset, computeMesh);

    private:

        float m_radius;
        int m_divisions;
        Vec3 m_offset;

        void computeMesh();
    };
}

#endif //HAGAME2_DISC_H
