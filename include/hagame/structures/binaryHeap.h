//
// Created by henry on 8/19/23.
//

#ifndef HAGAME2_BINARYHEAP_H
#define HAGAME2_BINARYHEAP_H

#include <vector>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include "comparable.h"

namespace hg::structures {
    template <typename T>
    class BinaryHeap {
    public:

        std::vector<T> data;

        BinaryHeap(
                std::function<bool(T, T)> compare = [](T a, T b) { return a > b; },
                std::function<bool(T, T)> equal = [](T a, T b) { return a == b; }
                ):
            m_compare(compare),
            m_equals(equal)
        {}

        size_t size() const;
        void clear();
        void insert(T el);
        T getTop() const;
        void deleteTop();
        bool exists(T el, int idx = 0) const;

    private:

        int getParentIdx(int idx) const;
        int getLeftChildIdx(int idx) const;
        int getRightChildIdx(int idx) const;

        std::function<bool(T, T)> m_compare;
        std::function<bool(T, T)> m_equals;

        void cascadeDown(int idx);
        void cascadeUp(int idx);

    };

    template<typename T>
    bool BinaryHeap<T>::exists(T el, int idx) const {

        std::deque<int> queue;

        queue.push_back(idx);

        while (queue.size() > 0) {

            int elements = queue.size();

            for (int i = 0; i < elements; i++) {
                if (m_equals(el, data[queue[0]])) {
                    return true;
                }

                int lIdx = getLeftChildIdx(queue[0]);
                int rIdx = getRightChildIdx(queue[0]);

                if (lIdx < size()) {
                    if (m_equals(el, data[lIdx])) {
                        return true;
                    }
                    if (!m_compare(el, data[lIdx])) {
                        queue.push_back(lIdx);
                    }
                }

                if (rIdx < size()) {
                    if (m_equals(el, data[rIdx])) {
                        return true;
                    }
                    if (!m_compare(el, data[rIdx])) {
                        queue.push_back(rIdx);
                    }
                }

                queue.pop_front();
            }


        }

        return false;
    }

    template<typename T>
    void BinaryHeap<T>::clear() {
        return data.clear();
    }

    template<typename T>
    size_t BinaryHeap<T>::size() const {
        return data.size();
    }

    template<typename T>
    void BinaryHeap<T>::cascadeDown(int idx) {
        int leftIdx = getLeftChildIdx(idx);
        int rightIdx = getRightChildIdx(idx);
        int smallest = idx;

        if (leftIdx < data.size() && m_compare(data[leftIdx], data[idx])) {
            smallest = leftIdx;
        }

        if (rightIdx < data.size() && m_compare(data[rightIdx], data[smallest])) {
            smallest = rightIdx;
        }

        if (smallest != idx) {
            std::swap(data[idx], data[smallest]);
            cascadeDown(smallest);
        }
    }

    template<typename T>
    void BinaryHeap<T>::cascadeUp(int idx) {
        int parentIdx = getParentIdx(idx);

        if (parentIdx >= 0) {
            if (m_compare(data[idx], data[parentIdx])) {
                std::swap(data[idx], data[parentIdx]);
                cascadeUp(parentIdx);
            }
        }
    }

    template<typename T>
    void BinaryHeap<T>::deleteTop() {
        if (data.size() == 0) {
            return;
        } else if (data.size() == 1) {
            data.clear();
            return;
        }

        data.erase(data.begin());

        cascadeDown(0);
    }

    template<typename T>
    T BinaryHeap<T>::getTop() const {
        return data[0];
    }

    template<typename T>
    void BinaryHeap<T>::insert(T el) {
        data.push_back(el);
        cascadeUp(data.size() - 1);
    }

    template<typename T>
    int BinaryHeap<T>::getRightChildIdx(int idx) const {
        return 2 * idx + 2;
    }

    template<typename T>
    int BinaryHeap<T>::getLeftChildIdx(int idx) const {
        return 2 * idx + 1;
    }

    template<typename T>
    int BinaryHeap<T>::getParentIdx(int idx) const {
        return std::floor((idx - 1) / 2);
    }
}

#endif //HAGAME2_BINARYHEAP_H
