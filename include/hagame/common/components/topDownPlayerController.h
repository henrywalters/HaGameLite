//
// Created by henry on 4/23/23.
//

#ifndef HAGAME2_TOPDOWNPLAYERCONTROLLER_H
#define HAGAME2_TOPDOWNPLAYERCONTROLLER_H

#include "../../core/component.h"
#include "../../math/aliases.h"

namespace hg {
    class TopDownPlayerController : public hg::Component {
    public:

        float epsilon = 1.0f;
        float acceleration = 1000.0f;
        float deacceleration = 2000.0f;
        float maxSpeed = 1000.0f;
        HG_GET_SET_SIMPLE(hg::Vec3, velocity);

        void addVelocity(hg::Vec3 vel) {
            m_velocity += vel;
        }

        void updateVelocity(double dt, hg::Vec3 direction) {

            if (direction != hg::Vec3::Zero()) {
                direction.normalize();
            }

            for (int i = 0; i < 3; i++) {

                if (nearEqual<float>(direction[i], 0, 0.001)) {
                    if (m_velocity[i] > -epsilon && m_velocity[i] < epsilon) {
                        m_velocity[i] = 0;
                    } else {
                        m_velocity[i] += deacceleration * (m_velocity[i] > 0 ? -1 : 1) * dt;
                    }

                } else {
                    m_velocity[i] += direction[i] * (sign(m_velocity[i]) == sign(direction[i]) ? acceleration : deacceleration) * dt;
                }


            }


            float speed = m_velocity.magnitude();

            if (speed >= maxSpeed) {
                m_velocity = m_velocity.normalized() * maxSpeed;
            }
        }

    private:

        hg::Vec3 m_velocity = hg::Vec3::Zero();

    };
};

#endif //HAGAME2_TOPDOWNPLAYERCONTROLLER_H