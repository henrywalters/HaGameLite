//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_CUBE_H
#define HAGAME2_CUBE_H

#include "../mesh.h"
#include "../triangle.h"
#include "../../utils/macros.h"

namespace hg::graphics::primitives {
    class Cube : public Mesh {
    public:

        Cube(Vec3 size): m_size(size) {
            computeMesh();
        }

        HG_GET_SET(Vec3, size, computeMesh);

    private:

        Vec3 m_size;

        void computeMesh();
    };
}

#endif //HAGAME2_CUBE_H
