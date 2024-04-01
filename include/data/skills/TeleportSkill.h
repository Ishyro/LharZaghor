#ifndef _TELEPORT_SKILL_H_
#define _TELEPORT_SKILL_H_

#include "data/skills/PseudoSkill.h"

class TeleportSkill : public PseudoSkill {
  public:
    const int32_t apparition_type;
    const int32_t movement_type;
    TeleportSkill(
      std::string name,
      int32_t skill_type,
      int32_t target_type,
      int32_t mana_cost,
      std::list<Effect *> effects,
      int32_t apparition_type,
      int32_t movement_type
    ):
      PseudoSkill(name, skill_type, target_type, mana_cost, effects),
      apparition_type(apparition_type),
      movement_type(movement_type)
    {}
    void activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range);
    int32_t getPower();
    int32_t getDamageFromType(int32_t dammage_type, int32_t overcharge_power);
    float getDamageReductionFromType(int32_t dammage_type, int32_t overcharge_power);
};

#endif // _TELEPORT_SKILL_H_
