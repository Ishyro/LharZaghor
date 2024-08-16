#include "data/Map.h"

#include "data/skills/Skill.h"

#include "data/skills/BlockSwapSkill.h"

#include "util/String.h"

void Skill::activate(Character * owner, Target * target, Adventure * adventure, int32_t mana_spent) {
  float overcharge_power;
  float overcharge_duration;
  float overcharge_range;
  computeOvercharges(mana_spent, overcharge_power, overcharge_duration, overcharge_range);
  owner->payMana(mana_spent);
  for(PseudoSkill * skill : skills) {
    skill->activate(owner, target, adventure, overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range, range);
  }
}

bool Skill::canCast(Character * owner, Target * target, Adventure * adventure, int32_t mana_spent) {
  float overcharge_power;
  float overcharge_duration;
  float overcharge_range;
  computeOvercharges(mana_spent, overcharge_power, overcharge_duration, overcharge_range);
  if(getManaCost() > owner->getChanneledMana() || mana_spent > owner->getChanneledMana()) {
    return false;
  }
  for(PseudoSkill * skill : skills) {
    if(!skill->canCast(owner, target, adventure, overcharge_power_type, overcharge_duration_type, overcharge_range_type, overcharge_power, overcharge_duration, overcharge_range, range)) {
      return false;
    }
  }
  return true;
}

void Skill::computeOvercharges(int32_t mana_spent, float & overcharge_power, float & overcharge_duration, float & overcharge_range) {
  
}

int32_t Skill::getManaCost() {
  int32_t mana_cost = 0;
  for(PseudoSkill * skill : skills) {
    mana_cost += skill->mana_cost;
  }
  return mana_cost;
}

float Skill::getTime(Character * owner) {
  /*
  if(scalling_type == SKILL_SCALE_MAIN_WEAPON) {
    return owner->getGear()->getWeapon_1()->strike_time;
  }
  else if(scalling_type == SKILL_SCALE_SUB_WEAPON) {
    return owner->getGear()->getWeapon_2()->strike_time;
  }
  else if(scalling_type == SKILL_SCALE_WEAPONS) {
    return owner->getGear()->getWeapon_1()->strike_time + owner->getGear()->getWeapon_2()->strike_time;
  }
  else {
  */
    return time;
  //}
}

float Skill::getPower() {
  float power = 0;
  for(PseudoSkill * skill : skills) {
    power += skill->getPower();
  }
  return power;
}

float Skill::getDamageFromType(int32_t damage_type, Character * owner, float overcharge) {
  float damage = 0;
  for(PseudoSkill * s : skills) {
    damage += s->getDamageFromType(damage_type, owner, overcharge);
  }
  return damage;
}

float Skill::getDamageReductionFromType(int32_t damage_type, float overcharge) {
  float reduction = 0.;
  for(PseudoSkill * s : skills) {
    reduction += s->getDamageReductionFromType(damage_type, overcharge);
  }
  return reduction;
}

Block * Skill::isEatingSkill() {
  bool is_eating_skill = false;
  Block * target = nullptr;
  for(PseudoSkill * s : skills) {
    if(s->skill_type == SKILL_TILE_SWAP) {
      target = ( (BlockSwapSkill *) s)->getCurrentBlock();
    }
    if(s->skill_type == SKILL_SIMPLE) {
      for(Effect * effect : s->effects) {
        if(effect->type == EFFECT_HUNGER) {
          is_eating_skill = true;
        }
      }
    }
  }
  if(is_eating_skill) {
    return target;
  }
  return nullptr;
}


float Skill::isTeleportSkill() {
  for(PseudoSkill * s : skills) {
    if(s->skill_type == SKILL_TELEPORT) {
      return range;
    }
  }
  return 0.F;
}
