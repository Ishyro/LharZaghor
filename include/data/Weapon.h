#ifndef _WEAPON_H_
#define _WEAPON_H_

#include <list>
#include <string>

#include "Values.h"

namespace weapon {
  static long id_cpt = 0;
}

class Weapon {
  public:
    const std::string name;
    const long id = ++weapon::id_cpt;
    const bool melee;
    const int range;
    const int type;
    const bool droppable;
    const float weight;
    const int gold_value;
    const bool use_ammo;
    const int ammo_type;
    const int capacity;
    const std::list<Effect *> effects;
    Weapon(
      std::string name,
      bool melee,
      int range,
      int type,
      bool droppable,
      int weight,
      int gold_value,
      bool use_ammo,
      int ammo_type,
      int capacity,
      std::list<Effect *> effects,
      int damages[DAMAGE_TYPE_NUMBER],
      Ammunition * ammo
    ):
      name(name),
      melee(melee),
      range(range),
      type(type),
      droppable(droppable),
      weight(weight),
      gold_value(gold_value),
      use_ammo(use_ammo),
      ammo_type(ammo_type),
      capacity(capacity),
      effects(effects),
      ammo(ammo)
    {
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
      }
    }
    Weapon(const Weapon * weapon):
      name(weapon->name),
      melee(weapon->melee),
      range(weapon->range),
      type(weapon->type),
      droppable(weapon->droppable),
      weight(weapon->weight),
      gold_value(weapon->gold_value),
      use_ammo(weapon->use_ammo),
      ammo_type(weapon->ammo_type),
      capacity(weapon->capacity),
      effects(weapon->effects),
      ammo(weapon->ammo)
    {
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        damages[i] = weapon->damages[i];
      }
    }
    int getRawDamage();
    int getDamageFromType(int damage_type);
    int getCurrentCapacity();
    Ammunition * getAmmo();
    void useAmmo();
    Ammunition * reload(Ammunition * ammo);
    std::string to_string();
    static Weapon * from_string(std::string to_read);
    bool operator == (const Weapon& w) const { return id == w.id; }
    bool operator != (const Weapon& w) const { return !operator==(w); }
  private:
    Ammunition * ammo;
    int damages[DAMAGE_TYPE_NUMBER];
};

#endif // _WEAPON_H_
