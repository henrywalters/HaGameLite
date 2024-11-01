//
// Created by henry on 10/30/24.
//

#ifndef HAGAME2_QUADTREE_H
#define HAGAME2_QUADTREE_H

#include <aliases.h>
#include "../math/vector.h"
#include "../math/hypercube.h"
#include "../structures/tree.h"
#include "../graphics/debug.h"

namespace hg::utils {

    template <typename DataType, typename SpatialType, typename ID = Identity>
    class QuadTree {
    public:

        using Vec = hg::math::Vector<2, SpatialType>;
        using Rect = hg::math::Hypercube<2, SpatialType>;

        ID idFn;

        struct Node {

            Node(Rect _rect): rect(_rect) {}

            Rect rect;
            std::vector<DataType> elements;
            bool hasChildren = false;
            std::array<std::unique_ptr<Node>, 4> quads;
        };

        QuadTree(Vec min, Vec max, int maxElements = 4):
            m_min(min),
            m_max(max),
            m_maxElements(maxElements),
            m_root(Rect(m_min, m_max - m_min))
        {}

        void clear() {
            m_entityMap.clear();
            m_root.hasChildren = false;
            m_root.elements.clear();
            if (m_root.hasChildren) {
                for (auto &child: m_root.quads) {
                    child.release();
                }
            }
        }

        void render() {
            render(&m_root);
        }

        void insert(DataType data, Vec pos) {
            std::deque<Node*> nodes;
            nodes.push_back(&m_root);
            bool inserted = false;

            while (nodes.size() > 0) {
                auto node = nodes.front();
                nodes.pop_front();

                if (!node->rect.contains(pos)) {
                    continue;
                }

                if (!node->hasChildren) {
                    if (node->elements.size() == m_maxElements) {
                        // Split the node
                        splitNode(node);
                    } else {
                        auto id = idFn(data);
                        if (m_entityMap.find(id) == m_entityMap.end()) {
                            m_entityMap.insert(std::make_pair(id, std::vector<Node*>()));
                        }
                        m_entityMap.at(id).push_back(node);
                        node->elements.push_back(data);
                    }
                }

                if (node->hasChildren) {
                    for (auto& child : node->quads) {
                        nodes.push_back(child.get());
                    }
                }
            }
        }

        void insert(DataType data, Vec pos, Vec size) {
            std::array<Vec, 4> points {
                pos,
                pos + Vec(0, size[1]),
                pos + size,
                pos + Vec(size[0], 0)
            };

            for (const auto& pt : points) {
                insert(data, pt);
            }
        }

    private:

        int m_maxElements;
        Vec m_min;
        Vec m_max;

        std::unordered_map<hg::utils::uuid_t, std::vector<Node*>> m_entityMap;

        Node m_root;

        void splitNode(Node* node) {
            node->hasChildren = true;
            node->quads = {
                std::make_unique<Node>(Rect(node->rect.pos, node->rect.size * 0.5)),
                std::make_unique<Node>(Rect(node->rect.pos + Vec2(0, node->rect.size[1]) * 0.5, node->rect.size * 0.5)),
                std::make_unique<Node>(Rect(node->rect.pos + node->rect.size * 0.5, node->rect.size * 0.5)),
                std::make_unique<Node>(Rect(node->rect.pos + Vec2(node->rect.size[0], 0) * 0.5, node->rect.size * 0.5))
            };
        }

        void render(Node* node) {
            if (node->hasChildren) {
                for (const auto& child : node->quads) {
                    render(child.get());
                }
            } else {
                hg::graphics::Debug::DrawRect(node->rect, hg::graphics::Color::green(), 0.01);
            }
        }

    };

};

#endif //HAGAME2_QUADTREE_H
