#include "data/skills/ResurrectionSkill.h"

void ResurrectionSkill::activate(Character * owner, Character * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_area_type, int overcharge_power, int overcharge_duration, int overcharge_area, int map_id, int x, int y) {}

int ResurrectionSkill::getPower() {
  return 0;
}

int ResurrectionSkill::getDamageFromType(int damage_type, int overcharge_power) {
  return 0;
}

float ResurrectionSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  return 0.;
}
