//
// Created by henry on 5/24/23.
//
#include "../../include/hagame/common/weapons.h"

using namespace hg;
using namespace hg::utils;

void hg::common::WeaponDef::save(Config& config) {
    std::string idStr = std::to_string(id);
    config.addSection(idStr);
    config.set(idStr, "automatic", automatic);
    config.set(idStr, "chambered", chambered);
    config.set(idStr, "infinite", infinite);
    config.set(idStr, "limitAmmo", limitAmmo);
    config.set(idStr, "shotsPerSecond", shotsPerSecond);
    config.set(idStr, "clipSize", clipSize);
    config.set(idStr, "maxAmmo", maxAmmo);
    config.set(idStr, "minDamage", minDamage);
    config.set(idStr, "maxDamage", maxDamage);
}

void hg::common::WeaponDef::load(const hg::utils::Config& config) {
    std::string idStr = std::to_string(id);
    automatic = config.get<bool>(idStr, "automatic", false);
    chambered = config.get<bool>(idStr, "chambered", false);
    infinite = config.get<bool>(idStr, "infinite", false);
    limitAmmo = config.get<bool>(idStr, "limitAmmo", false);
    shotsPerSecond = config.get<int>(idStr, "shotsPerSecond", 1);
    clipSize = config.get<int>(idStr, "clipSize", 10);
    maxAmmo = config.get<int>(idStr, "maxAmmo", 100);
    minDamage = config.get<float>(idStr, "minDamage", 10);
    maxDamage = config.get<float>(idStr, "maxDamage", 10);
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

bool common::Weapon::reload() {
    int ammoUsed = std::min(m_ammo, settings.clipSize - m_ammoInClip);
    int unusedAmmo = reload(ammoUsed);
    m_ammo -= ammoUsed + unusedAmmo;
    return unusedAmmo != ammoUsed;
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
    if (index >= m_weapons.size()) {
        throw std::runtime_error("Weapon index does not exist");
    }
    m_selectedWeapon = index;
}

common::Weapon *common::Weapons::getWeapon(int index) const {
    return m_weapons[index].get();
}

common::Weapon* common::Weapons::getWeapon() const {
    return m_weapons.size() > 0 ? m_weapons[m_selectedWeapon].get() : nullptr;
}

size_t common::Weapons::getCount() const {
    return m_weapons.size();
}

