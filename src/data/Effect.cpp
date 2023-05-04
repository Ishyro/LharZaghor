#include "data/Effect.h"

#include "util/String.h"

void Effect::activate(Character * target) {
  if(duration != 0) {
    target->addEffect(this);
  }
  else {
    switch (type) {
      // no duration means INSTANT_DURATION for these effects
      case HP:
        target->hpHeal(power);
        break;
      case MANA:
        target->manaHeal(power);
        break;
      case HP_MAX:
        target->incrMaxHp();
        break;
      case MANA_MAX:
        target->incrMaxMana();
        break;
      case STAMINA:
        target->addStamina((float) power);
        break;
      case SATIETY:
        target->addSatiety((float) power);
        break;
      case ARMOR_MULTIPLIER:
        target->incrArmorMultiplier();
        break;
      case SOULBURNTRESHOLD:
        target->incrSoulBurnTreshold();
        break;
      case FLOW:
        target->incrFlow();
        break;
      case VISION_RANGE:
        target->incrVisionRange();
        break;
      case VISION_POWER:
        target->incrVisionPower();
        break;
      case DETECTION_RANGE:
        target->incrDetectionRange();
        break;
      case DAMAGE:
        target->receiveAttack(damages, 360.F, STRIKE);
        break;
      case EXPERIENCE:
        target->gainXP(power);
        break;
      // no duration means INFINITE for next effects (can be cancelled)
      default:
        target->addEffect(this);
    }
  }
}

bool Effect::tick(Character * target) {
  switch (type) {
    case HP:
      target->hpHeal(power);
      break;
    case MANA:
      target->manaHeal(power);
      break;
    case STAMINA:
      target->addStamina((float) power);
      break;
    case SATIETY:
      target->addSatiety((float) power);
      break;
    case DAMAGE:
      target->receiveAttack(damages, 360.F, STRIKE);
      break;
    case EXPERIENCE:
      target->gainXP(power);
      break;
    default:
      ;
  }
  return duration_type == TEMPORARY_DURATION && --tick_left == 0;
}

void Effect::desactivate(Character * target) {
  if(duration_type == INFINITE_DURATION || duration_type == TEMPORARY_DURATION) {
    target->removeEffect(this);
  }
}

int Effect::getTickLeft() { return tick_left; }

int Effect::getRawDamage() {
  int power = 0;
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    power += damages[i];
  }
  return power;
}

int Effect::getDamageFromType(int damage_type) { return damages[damage_type]; }
float Effect::getDamageReductionFromType(int damage_type) { return damage_reductions[damage_type]; }

std::string Effect::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, level);
  String::insert(ss, attributes);
  String::insert_int(ss, type);
  String::insert_int(ss, duration_type);
  String::insert_int(ss, power);
  String::insert_int(ss, duration);
  String::insert_int(ss, tick_left);
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_int(ss, damages[i]);
  }
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    String::insert_float(ss, damage_reductions[i]);
  }
  std::string result = ss->str();
  delete ss;
  return result;
}

Effect * Effect::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  long id = String::extract_long(ss);
  int level = String::extract_int(ss);
  std::string attributes = String::extract(ss);
  int type = String::extract_int(ss);
  int duration_type = String::extract_int(ss);
  int power = String::extract_int(ss);
  int duration = String::extract_int(ss);
  int tick_left = String::extract_int(ss);
  int damages[DAMAGE_TYPE_NUMBER];
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damages[i] = String::extract_int(ss);
  }
  float damage_reductions[DAMAGE_TYPE_NUMBER];
  for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
    damage_reductions[i] = String::extract_float(ss);
  }
  delete ss;
  return new Effect(name, id, level, attributes, type, duration_type, power, duration, tick_left, damages, damage_reductions);
 }
