#ifndef _PROJECTILE_SKILL_H_
#define _PROJECTILE_SKILL_H_

#include "data/Projectile.h"

#include "data/skills/PseudoSkill.h"

class ProjectileSkill : public PseudoSkill {
  public:
    ProjectileSkill(
      std::string name,
      int32_t skill_type,
      int32_t target_type,
      int32_t mana_cost,
      std::list<Effect *> effects,
      Projectile * projectile
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, effects),
      projectile(projectile)
    {}
    void activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range);
    int32_t getPower();
    int32_t getDamageFromType(int32_t dammage_type, int32_t overcharge_power);
    float getDamageReductionFromType(int32_t dammage_type, int32_t overcharge_power);
    Projectile * getProjectile();
  private:
    Projectile * projectile;
};

#endif // _PROJECTILE_SKILL_H_
