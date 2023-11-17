//
// Created by henry on 5/24/23.
//

#ifndef HAGAME2_GUNS_H
#define HAGAME2_GUNS_H

#include "../utils/config.h"
#include "../utils/macros.h"
#include "../math/aliases.h"

namespace hg::common {

    struct WeaponDef : public utils::Configurable {

        std::string name;
        bool chambered = false;
        bool automatic = false;
        bool infinite = false;
        bool limitAmmo = true;
        int shotsPerSecond = 1;
        int clipSize = 10;
        int maxAmmo = 100;
        float minDamage = 10.0f;
        float maxDamage = 10.0f;

        void save(utils::Config& config) override;
        void load(const utils::Config& config) override;
    };

    class Weapon {
    public:

        Weapon(WeaponDef def):
            settings(def)
        {}

        WeaponDef settings;

        HG_GET(int, ammo);
        HG_GET(int, ammoInClip);

        // Run this function every frame. onFire will be called everytime the weapon is actually fired
        void update(hg::Vec3 pos, hg::Vec3 dir, double dt, bool triggerDown);

        // Automatically reload the weapon given the current ammo count
        bool reload();

        // Manually add a specific ammo count to the weapon. The unused ammo will be returned
        int reload(int bulletCount);

        // Manually add ammo to your stash. Unused ammo will be returned, if limitAmmo is true
        int addAmmo(int bulletCount);

        virtual Vec2 computeSpread() { return Vec2::Zero(); }

    protected:

        virtual void onFire(hg::Vec3 pos, hg::Vec3 dir) {}
        virtual void outOfAmmo() {}

    private:

        bool m_oneInChamber = false;
        int m_ammoInClip = 0;
        int m_ammo = 0;

        bool m_fired = false;
        bool m_triggerDown = false;
        double m_elapsedTime = 0;
        double m_lastFire;

    };

    template <typename T>
    concept IsWeapon = std::is_base_of<Weapon, T>::value;

    class Weapons {
    public:

        HG_GET(int, selectedWeapon);

        template <IsWeapon T, class... Args>
        T* add(WeaponDef def, Args &&... args) {
            auto weapon = std::make_shared<T>(def, std::forward<Args>(args)...);
            m_weapons.push_back(weapon);
            return weapon.get();
        }

        void selectWeapon(int index);
        Weapon* getWeapon(int index) const;
        Weapon* getWeapon() const;
        size_t getCount() const;

    private:

        int m_selectedWeapon = 0;
        std::vector<std::shared_ptr<Weapon>> m_weapons;

    };
}

#endif //HAGAME2_GUNS_H
