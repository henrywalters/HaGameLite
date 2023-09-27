//
// Created by henry on 8/19/23.
//

#ifndef HAGAME2_VALUED_H
#define HAGAME2_VALUED_H

#include <type_traits>

namespace hg::structures {
    template <typename T>
    concept Valued = requires (T t) {
        {t.value()} -> float;
    };
}

#endif //HAGAME2_VALUED_H
