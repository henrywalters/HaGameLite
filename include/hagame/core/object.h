//
// Created by henry on 12/23/22.
//

#ifndef HAGAME2_OBJECT_H
#define HAGAME2_OBJECT_H

#include <string>
#include <ostream>
#include "../utils/macros.h"

namespace hg {

    static long LAST_OBJECT_ID = 0;

    class Object {
    public:

        Object():
            m_id(LAST_OBJECT_ID++)
        {}

        HG_GET(long, id);

        bool operator ==(const Object& object) {
            return id() == object.id();
        }

        bool operator !=(const Object& object) {
            return id() != object.id();
        }

        operator std::string() const {
            return toString();
        }

        friend std::ostream& operator <<(std::ostream& out, const Object& object) {
            out << object.toString();
            return out;
        }

    protected:
        virtual std::string toString() const = 0;

    private:

        unsigned long m_id;

    };
}

#endif //HAGAME2_OBJECT_H
