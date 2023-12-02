//
// Created by henry on 1/4/23.
//

#ifndef hg2_LINEINTERSECTION_H
#define hg2_LINEINTERSECTION_H

#include "aliases.h"
#include "interval.h"
#include "lineSegment.h"

namespace hg::math {
    class LineIntersection {
    public:

        LineIntersection() {}
        LineIntersection(float t, Vec2 pos) :
                m_intersected(true),
                m_t(t),
                m_pos(pos) {}


        static LineIntersection Parallel() {
            LineIntersection intersection;
            intersection.m_parallel = true;
            return intersection;
        }

        static LineIntersection Collinear(hg::math::Interval<float> tInterval, Vec2 a, Vec2 b) {
            LineIntersection intersection;
            intersection.m_collinear = true;
            intersection.m_intersected = true;
            intersection.m_tInterval = tInterval;
            intersection.m_segment = hg::math::LineSegment(a.resize<3>(), b.resize<3>());
            return intersection;
        }

        static LineIntersection CollinearDisjoint() {
            LineIntersection intersection;
            intersection.m_collinear = true;
            return intersection;
        }

        hg::math::LineSegment segment() {
            if (!m_collinear) throw std::runtime_error("Intersection is not collinear");
            return m_segment;
        }

        hg::math::Interval<float> tInterval() {
            if (!m_collinear) throw std::runtime_error("Intersection is not collinear");
            return m_tInterval;
        }

        Vec2 pos() {
            if (!m_intersected) throw std::runtime_error("Non-intersection");
            if (m_collinear) throw std::runtime_error("Collinear - use segment instead");
            return m_pos;
        }

        float t() {
            if (!m_intersected) throw std::runtime_error("Non-intersection");
            if (m_collinear) throw std::runtime_error("Collinear - use segment instead");
            return m_t;
        }

        bool intersected() {
            return m_intersected;
        }

        bool collinear() {
            return m_collinear;
        }

        bool parallel() {
            return m_parallel;
        }

    private:
        bool m_intersected, m_parallel, m_collinear = false;
        float m_t;
        hg::math::Interval<float> m_tInterval;
        Vec2 m_pos;
        hg::math::LineSegment m_segment;
    };

    // Checks if the lines AB and PQ intersect. If so, t and intersection are set relative to AB.
    bool linesIntersect(Vec2 a, Vec2 b, Vec2 p, Vec2 q, LineIntersection& intersection);
}



#endif //hg2_LINEINTERSECTION_H
