#include "data/skills/SummonSkill.h"

#include "ai/AI.h"
#include "ai/PlayerAI.h"
#include "ai/DiurnalPassiveAI.h"
#include "ai/NocturnalPassiveAI.h"
#include "ai/DiurnalAgressiveAI.h"
#include "ai/NocturnalAgressiveAI.h"
#include "ai/EtheralCasterAI.h"

void SummonSkill::activate(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  // Target will be a Tile
  AI * ai;
  if (ai_str == "DiurnalPassiveAI") {
    ai = new DiurnalPassiveAI(target->x, target->y);
  }
  else if (ai_str == "NocturnalPassiveAI") {
    ai = new NocturnalPassiveAI(target->x, target->y);
  }
  else if (ai_str == "DiurnalAgressiveAI") {
    ai = new DiurnalAgressiveAI(target->x, target->y);
  }
  else if (ai_str == "NocturnalAgressiveAI") {
    ai = new NocturnalAgressiveAI(target->x, target->y);
  }
  else if (ai_str == "EtheralCasterAI") {
    ai = new EtheralCasterAI(target->x, target->y);
  }
  Character * c = new Character(
    character,
    character->name,
    xp,
    (int) target->x,
    (int) target->y,
    owner->getOrientation(),
    target->id,
    owner->getTeam(),
    ai,
    attributes,
    nullptr,
    (Gear *) adventure->getDatabase()->getGear("TXT_NO_GEAR"),
    race,
    origin,
    culture,
    religion,
    profession,
    titles
  );
  adventure->getWorld()->getMap(target->id)->addCharacter(c);
  if(apparition_type == SOFT) {
    adventure->softMoveCharacterToMap(c, target->id, target->y, target->x);
  }
  else {
    adventure->hardMoveCharacterToMap(c, target->id, target->y, target->x);
  }
}

bool SummonSkill::canCast(Character * owner, Target * target, Adventure * adventure, int overcharge_power_type, int overcharge_duration_type, int overcharge_range_type, int overcharge_power, int overcharge_duration, int overcharge_range, int range) {
  return true;
}

int SummonSkill::getPower() {
  return 0;
}

int SummonSkill::getDamageFromType(int damage_type, int overcharge_power) {
  return 0;
}

float SummonSkill::getDamageReductionFromType(int damage_type, int overcharge_power) {
  return 0.;
}

Character * SummonSkill::getCharacter() { return character; }
std::string SummonSkill::getAI() { return ai_str; }
Way * SummonSkill::getRace() { return race; }
Way * SummonSkill::getOrigin() { return origin; }
Way * SummonSkill::getCulture() { return culture; }
Way * SummonSkill::getReligion() { return religion; }
Way * SummonSkill::getProfession() { return profession; }
Attributes * SummonSkill::getAttributes() { return attributes; }
std::list<Way *> SummonSkill::getTitles() { return titles; }
int SummonSkill::getApparitionType() { return apparition_type; }
int SummonSkill::getXp() { return xp; }

void SummonSkill::setCharacter(Character * character) { this->character = character; }
void SummonSkill::setAI(std::string ai_str) { this->ai_str = ai_str; }
void SummonSkill::setRace(Way * race) { this->race = race; }
void SummonSkill::setOrigin(Way * origin) { this->origin = origin; }
void SummonSkill::setCulture(Way * culture) { this->culture = culture; }
void SummonSkill::setReligion(Way * religion) { this->religion = religion; }
void SummonSkill::setProfession(Way * profession) { this->profession = profession; }
void SummonSkill::setAttributes(Attributes * attributes) { this->attributes = attributes; }
void SummonSkill::setTitles(std::list<Way *> titles) { this->titles = titles; }
void SummonSkill::setApparitionType(int apparition_type) { this->apparition_type = apparition_type; }
void SummonSkill::setXp(int xp) { this->xp = xp; }
