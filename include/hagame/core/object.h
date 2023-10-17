//
// Created by henry on 12/23/22.
//

#ifndef HAGAME2_OBJECT_H
#define HAGAME2_OBJECT_H

#include <string>
#include <ostream>
#include "../utils/macros.h"
#include "../utils/uuid.h"

namespace hg {

#define OBJECT_NAME(name) std::string toString() const override { return #name + std::string("<" + std::to_string(id()) + ">"); }

    class Object : public utils::UUID {
    public:

        virtual ~Object() {}

        Object() = default;

        operator std::string() const {
            return toString();
        }

        friend std::ostream& operator <<(std::ostream& out, const Object& object) {
            out << object.toString();
            return out;
        }

    protected:
        virtual std::string toString() const = 0;

    };
}

#endif //HAGAME2_OBJECT_H
