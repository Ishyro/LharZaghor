#ifndef _MAP_LINKER_SKILL_H_
#define _MAP_LINKER_SKILL_H_

#include "data/skills/PseudoSkill.h"

class MapLinkerSkill : public PseudoSkill {
  using PseudoSkill::PseudoSkill;
  public:
    void activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, float overcharge_power, float overcharge_duration, float overcharge_range, int32_t range);
    float getPower();
    float getDamageFromType(int32_t dammage_type, Character * owner, float overcharge);
    float getDamageReductionFromType(int32_t dammage_type, float overcharge);
};

#endif // _MAP_LINKER_SKILL_H_
