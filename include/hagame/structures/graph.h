//
// Created by henry on 11/7/23.
//

#ifndef HAGAME2_GRAPH_H
#define HAGAME2_GRAPH_H

#include "../utils/helpers.h"

#include <vector>
#include <memory>

namespace hg::structures {
   template <typename T>
   class Graph {
   public:

       struct Node {
           T value;
           std::vector<Node*> parents;
           std::vector<Node*> children;

           Node(const T& _value): value(_value) {}
           Node() {}
       };

       // Sets the root node and releases any previous nodes
       Node* setRoot(const T& value) {
           auto node = std::make_unique<Node>(value);
           m_nodes.clear();
           m_nodes.push_back(node);
           m_current = node.get();
           return m_current;
       }

       void setCurrent(const T& value) {
           auto current = get(value);
           if (!current) {
               throw std::runtime_error("Value does not exist in graph");
           }
           m_current = current;
       }

       bool exists(const T& value) const {
           for (const auto& node : m_nodes) {
               if (node->value == value) {
                   return true;
               }
           }
           return false;
       }

       Node* get(const T& value) const {
           for (const auto& node : m_nodes) {
               if (node->value == value) {
                   return node.get();
               }
           }
           return nullptr;
       }

       Node* get() const {
           return m_current;
       }

       // Inserts a new, unconnected node into the graph. If it is the first one, it will be the root
       void insert(const T& value) {
           if (exists(value)) {
               throw std::runtime_error("Value already exists");
           }
           auto node = std::make_shared<Node>(value);
           m_nodes.push_back(node);

           if (!m_current) {
               m_current = node.get();
           }
       }

       void remove(const T& value) {
            auto node = get(value);
            if (!node) { return; }
            for (const auto& parent : node->parents) {
                utils::v_remove(parent->children, node);
            }
            m_nodes.erase(std::find(m_nodes.begin(), m_nodes.end(), value));
       }

       // Add b as a child of a
       void connect(const T& a, const T& b) {
           auto nodeA = get(a);
           auto nodeB = get(b);

           if (!nodeA && !nodeB) {
               throw std::runtime_error("Nodes do not exist");
           } else if (!nodeA) {
               throw std::runtime_error("Node A does not exist");
           } else if (!nodeB) {
               throw std::runtime_error("Node B does not exist");
           }

           nodeA->children.push_back(nodeB);
           nodeB->parents.push_back(nodeA);
       }

   private:

       Node* m_current;
       std::vector<std::shared_ptr<Node>> m_nodes;

   };
};

#endif //HAGAME2_GRAPH_H
