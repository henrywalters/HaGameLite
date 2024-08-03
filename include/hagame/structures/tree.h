//
// Created by henry on 2/27/24.
//

#ifndef HAGAME2_TREE_H
#define HAGAME2_TREE_H

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

        static void Traverse(Tree* tree, std::function<void(Tree*)> lambda) {
            lambda(tree);
            for (auto& child : tree->children()) {
                Traverse(child, lambda);
            }
        }

        static void DepthFirstTraverse(Tree* tree, std::function<bool(Tree*)> lambda) {
            std::stack<Tree*> nodes;
            nodes.push(tree);
            while (!nodes.empty()) {
                auto node = nodes.top();
                nodes.pop();
                if (!lambda(node)) { return ; }
                for (auto& child : node->children()) {
                    nodes.push(child);
                }
            }
        }

        static void BreadthFirstTraverse(Tree* tree, std::function<bool(Tree*)>lambda) {
            std::deque<Tree*> nodes;
            nodes.push_back(tree);
            while (!nodes.empty()) {
                auto node = nodes.front();
                nodes.pop_front();
                if (!lambda(node)) { return ; }
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
