//
// Created by henry on 5/27/23.
//

#ifndef HAGAME2_INODEMAP_H
#define HAGAME2_INODEMAP_H

#include <type_traits>

namespace hg::interfaces {

    template <typename IndexType, typename ValueType>
    class INode {
    public:
        IndexType index;
        ValueType value;
    };

    template <typename IndexType, typename ValueType>
    class INodeMap {
    public:

        virtual void clear() = 0;
        virtual bool has(IndexType index) = 0;
        virtual INode<IndexType, ValueType> get(IndexType index) = 0;
        virtual void set(IndexType index, ValueType value) = 0;
        virtual std::vector<INode<IndexType, ValueType>> getNeighbors(IndexType index) = 0;
        virtual bool isEmpty(INode<IndexType, ValueType> node) = 0;

    };

    template <typename IndexType, typename ValueType, class T>
    concept IsINodeMap = std::is_base_of<INodeMap<IndexType, ValueType>, T>::value;
}

#endif //HAGAME2_INODEMAP_H
