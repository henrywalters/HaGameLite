//
// Created by henry on 2/27/24.
//

#ifndef HAGAME2_TREE_H
#define HAGAME2_TREE_H

#include <functional>
#include <vector>
#include <stack>
#include <deque>
#include "../utils/macros.h"

namespace hg::structures {

    class Tree {
    public:

        void addChild(Tree* child) {
            child->m_parent = this;
            child->m_depth = this->m_depth + 1;
            m_children.push_back(child);
        }

        void removeChild(Tree* child) {
            m_children.erase(std::find(m_children.begin(), m_children.end(), child));
        }

        Tree* parent() const {
            return m_parent;
        }

        std::vector<Tree*> children() const {
            return m_children;
        }

        static Tree* Root(Tree* tree) {
            Tree* node = tree;
            while (node->parent()) {
                node = node->parent();
            }
            return node;
        }

        template <typename T> requires std::is_base_of_v<Tree, T>
        static void Traverse(Tree* tree, std::function<void(T*)> lambda) {
            lambda(static_cast<T*>(tree));
            for (auto& child : tree->children()) {
                Traverse(child, lambda);
            }
        }

        template <typename T> requires std::is_base_of_v<Tree, T>
        static void DepthFirstTraverse(Tree* tree, std::function<bool(T*)> lambda) {
            std::stack<Tree*> nodes;
            nodes.push(tree);
            while (!nodes.empty()) {
                auto node = nodes.top();
                nodes.pop();
                if (!lambda(static_cast<T*>(node))) { return ; }
                for (auto& child : node->children()) {
                    nodes.push(child);
                }
            }
        }

        template <typename T> requires std::is_base_of_v<Tree, T>
        static void BreadthFirstTraverse(Tree* tree, std::function<bool(T*)>lambda) {
            std::deque<Tree*> nodes;
            nodes.push_back(tree);
            while (!nodes.empty()) {
                auto node = nodes.front();
                nodes.pop_front();
                if (!lambda(static_cast<T*>(node))) { return ; }
                for (auto& child : node->children()) {
                    nodes.push_back(child);
                }
            }
        }

        HG_GET(uint32_t, depth)

    private:

        uint32_t m_depth = 0;
        std::vector<Tree*> m_children;
        Tree* m_parent = nullptr;

    };
}

#endif //HAGAME2_TREE_H
