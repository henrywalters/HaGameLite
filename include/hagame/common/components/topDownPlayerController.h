//
// Created by henry on 4/23/23.
//

#ifndef HAGAME2_TOPDOWNPLAYERCONTROLLER_H
#define HAGAME2_TOPDOWNPLAYERCONTROLLER_H

#include "../../core/entity.h"
#include "../../core/component.h"
#include "../../math/aliases.h"
#include "../../utils/macros.h"

namespace hg {
    class TopDownPlayerController : public hg::Component {
    public:

        float epsilon = 0.00001f;
        float acceleration = 1.0f;
        float deacceleration = 2.0f;
        float maxSpeed = 1.0f;
        HG_GET_SET_SIMPLE(hg::Vec3, velocity);

        void addVelocity(hg::Vec3 vel) {
            m_velocity += vel;
        }

        void updateVelocity(double dt, hg::Vec3 direction) {

            if (direction != hg::Vec3::Zero()) {
                direction.normalize();
            }

            for (int i = 0; i < 3; i++) {

                if (nearEqual<float>(direction[i], 0, 0.1)) {
                    if (nearEqual<float>(m_velocity[i], 0, epsilon)) {
                        m_velocity[i] = 0;
                    } else {
                        // Need a check to make sure we don't over shoot!
                        float newVel = m_velocity[i] + deacceleration * (m_velocity[i] > 0 ? -1 : 1) * dt;
                        bool overshot = (newVel > 0 && m_velocity[i] < 0) || (newVel < 0 && m_velocity[i] > 0);

                        // Branchless method
                        m_velocity[i] = !overshot * newVel;
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

    HG_COMPONENT(Core, TopDownPlayerController)
    HG_FIELD(TopDownPlayerController, float, acceleration)
    HG_FIELD(TopDownPlayerController, float, deacceleration)
    HG_FIELD(TopDownPlayerController, float, maxSpeed)
};

#endif //HAGAME2_TOPDOWNPLAYERCONTROLLER_H
