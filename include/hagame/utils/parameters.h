//
// Created by henry on 4/24/23.
//

#ifndef HAGAME2_PARAMETERS_H
#define HAGAME2_PARAMETERS_H

#include <variant>
#include <string>

namespace hg::utils {

    using parameter_t = std::variant<int, float, bool, std::string>;

    template <typename KeyType>
    class Parameters {
    public:

        enum class Type {
            Integer,
            Float,
            Bool,
            String,
        };

        struct Parameter {
            Type type;
            parameter_t value;
        };

    private:



    };
}

#endif //HAGAME2_PARAMETERS_H
