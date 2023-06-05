//
// Created by henry on 5/22/23.
//

#ifndef HAGAME2_TIMEOUT_H
#define HAGAME2_TIMEOUT_H

#include "../../core/component.h"

namespace hg::utils::components {

class Timeout : public hg::Component {
public:

    Timeout(std::function<void()> onTimeout, double duration);

protected:

    void onUpdate(double dt) override;

    std::string toString() const override;

private:

    double m_elapsed = 0;
    double m_duration = 0;
    std::function<void()> m_onTimeout;

};

}

#endif //HAGAME2_TIMEOUT_H
