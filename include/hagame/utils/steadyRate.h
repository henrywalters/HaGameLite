//
// Created by henry on 10/11/23.
//

#ifndef HAGAME2_STEADYRATE_H
#define HAGAME2_STEADYRATE_H

class SteadyRate {
public:

    double ticksPerSecond = 1.0;

    void update(double dt) {
        m_elapsedTime += dt;
        double timeSinceLastTick = m_elapsedTime - m_lastTick;
        if (timeSinceLastTick >= 1.0 / ticksPerSecond) {
            onTick();
            m_lastTick = m_elapsedTime;
        }
    }

protected:

    virtual void onTick() = 0;

    double m_lastTick = 0;
    double m_elapsedTime = 0;
};

#endif //HAGAME2_STEADYRATE_H
