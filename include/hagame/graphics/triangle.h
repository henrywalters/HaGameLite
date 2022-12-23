//
// Created by henry on 12/18/22.
//

#ifndef HAGAME2_TRIANGLE_H
#define HAGAME2_TRIANGLE_H

#include "../math/aliases.h"
#include "vertex.h"

namespace hg::graphics {
    struct Triangle {

        std::vector<Vertex> verts;

        Triangle(Vec3 a, Vec3 b, Vec3 c) {
            for (int i = 0; i < 3; i++) {
                verts.push_back(Vertex());
            }
            setPositions({ a, b, c });
            setTextures({ a.resize<2>(), b.resize<2>(), c.resize<2>() });
            setNormal(cross(b - a, c - a));
        }

        void setPositions(std::vector<Vec3> points) {
            for (int i = 0; i < 3; i++) {
                verts[i].position = points[i];
            }
        }

        void setNormal(Vec3 normal) {
            setNormals({ normal, normal, normal });
        }

        void setNormals(std::vector<Vec3> normals) {
            for (int i = 0; i < 3; i++) {
                verts[i].normal = normals[i];
            }
        }

        void setTextures(std::vector<Vec2> textures) {
            for (int i = 0; i < 3; i++) {
                verts[i].texCoords = textures[i];
            }
        }

        // Insert the contents of this triangle into the data structures needed to construct a Mesh
        void insert(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
            unsigned int lastIdx = indices.size();
            indices.insert(indices.end(), { lastIdx, lastIdx + 1, lastIdx + 2 });
            vertices.insert(vertices.end(), { verts[0], verts[1], verts[2] });
        }
    };
}

#endif //HAGAME2_TRIANGLE_H
