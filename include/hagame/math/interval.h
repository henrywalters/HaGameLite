//
// Created by henry on 1/4/23.
//

#ifndef HAGAME2_INTERVAL_H
#define HAGAME2_INTERVAL_H

namespace hg::math {
    template <class T>
    class Interval {
    public:

        T lower, upper;

        Interval(T _lower, T _upper) :
                lower(_lower),
                upper(_upper) {}

        Interval(T _upper) :
                lower(0),
                upper(_upper) {}

        Interval() :
                lower(0),
                upper(0) {}

        T span() { return upper - lower; }

        bool contains(Interval interval) {
            return lower <= interval.lower&& upper >= interval.upper;
        }

        bool overlaps(Interval interval) {
            return (lower >= interval.lower && lower <= interval.upper) || (upper >= interval.lower && upper <= interval.upper);
        }

        bool valid() {
            return upper >= lower;
        }

        bool operator==(const Interval& in) const {
            return lower == in.lower && upper == in.upper;
        }

        bool operator<(const Interval& in) const {
            return lower < in.lower;
        }

        Interval operator* (const T& scalar) const {
            return Interval(lower * scalar, upper * scalar);
        }

        Interval& operator*=(const T& scalar) {
            lower *= scalar;
            upper *= scalar;
        }
    };
}

#endif //HAGAME2_INTERVAL_H
