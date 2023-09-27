//
// Created by henry on 8/19/23.
//

#ifndef HAGAME2_COMPARABLE_H
#define HAGAME2_COMPARABLE_H

namespace hg::structures {
    template <typename T>
    concept Comparable = requires (T a, T b) {
        a == b;
        a > b;
        a < b;
    };
}

#endif //HAGAME2_COMPARABLE_H
