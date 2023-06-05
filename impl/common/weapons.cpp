//
// Created by henry on 5/24/23.
//
#include "../../include/hagame/common/weapons.h"

using namespace hg;
using namespace hg::utils;

void hg::common::WeaponDef::save(Config& config) {
    config.addSection(name);
    config.set<bool>(name, "automatic", automatic);
    config.set<bool>(name, "chambered", chambered);
    config.set<bool>(name, "infinite", infinite);
    config.set<bool>(name, "limitAmmo", limitAmmo);
    config.set<int>(name, "shotsPerSecond", shotsPerSecond);
    config.set<int>(name, "clipSize", clipSize);
    config.set<int>(name, "maxAmmo", maxAmmo);
}

void hg::common::WeaponDef::load(hg::utils::Config config) {
    automatic = config.get<bool>(name, "automatic");
    chambered = config.get<bool>(name, "chambered");
    infinite = config.get<bool>(name, "infinite");
    limitAmmo = config.get<bool>(name, "limitAmmo");
    shotsPerSecond = config.get<int>(name, "shotsPerSecond");
    clipSize = config.get<int>(name, "clipSize");
    maxAmmo = config.get<int>(name, "maxAmmo");
}

void common::Weapon::update(hg::Vec3 pos, hg::Vec3 dir, double dt, bool triggerDown) {
    bool canFire = false;

    m_elapsedTime += dt;

    if (!triggerDown) {
        m_triggerDown = false;
        return;
    }

    if (settings.automatic) {
        double fireRate = 1.0 / settings.shotsPerSecond;

        if (!m_fired || m_elapsedTime - m_lastFire >= fireRate) {
            canFire = true;
        }

    } else {
        if (!m_fired || !m_triggerDown) {
            canFire = true;
            m_triggerDown = true;
        }
    }

    if (canFire) {

        if (settings.infinite) {
            onFire(pos, dir);
        } else {
            if (m_ammoInClip <= 0 && !m_oneInChamber) {
                outOfAmmo();
                return;
            }

            onFire(pos, dir);
            if (m_oneInChamber) {
                m_oneInChamber = false;
            } else {
                m_ammoInClip--;
            }
        }

        m_fired = true;
        m_lastFire = m_elapsedTime;
    }
}

 void common::Weapon::reload() {
    reload(std::min(m_ammo, settings.clipSize - m_ammoInClip));
}

int common::Weapon::reload(int bulletCount) {
    int ammoNeeded = settings.clipSize - m_ammoInClip;

    if (ammoNeeded >= bulletCount) {
        m_ammoInClip += bulletCount;
        return 0;
    } else {
        m_ammoInClip += ammoNeeded;
        return bulletCount - ammoNeeded;
    }
}

int common::Weapon::addAmmo(int bulletCount) {
    if (settings.limitAmmo) {
        int ammoNeeded = settings.maxAmmo - m_ammo;
        if (ammoNeeded >= bulletCount) {
            m_ammo += bulletCount;
            return 0;
        } else {
            m_ammo += ammoNeeded;
            return bulletCount - ammoNeeded;
        }
    } else {
        m_ammo += bulletCount;
        return 0;
    }
}

void common::Weapons::selectWeapon(int index) {
    m_selectedWeapon = index;
}

common::Weapon *common::Weapons::getWeapon(int index) {
    return m_weapons[index].get();
}

common::Weapon* common::Weapons::getWeapon() {
    return m_weapons.size() > 0 ? m_weapons[m_selectedWeapon].get() : nullptr;
}


