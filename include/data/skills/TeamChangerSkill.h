#ifndef _TEAM_CHANGER_SKILL_H_
#define _TEAM_CHANGER_SKILL_H_

#include "data/skills/PseudoSkill.h"

class TeamChangerSkill : public PseudoSkill {
  using PseudoSkill::PseudoSkill;
  public:
    void activate(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range);
    bool canCast(Character * owner, Target * target, Adventure * adventure, int32_t overcharge_power_type, int32_t overcharge_duration_type, int32_t overcharge_range_type, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range, int32_t range);
    int32_t getPower();
    int32_t getDamageFromType(int32_t dammage_type, int32_t overcharge_power);
    float getDamageReductionFromType(int32_t dammage_type, int32_t overcharge_power);
};

#endif // _TEAM_CHANGER_SKILL_H_
