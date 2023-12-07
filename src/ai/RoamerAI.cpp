#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "data/actions/Action.h"
#include "data/actions/BaseAction.h"
#include "data/actions/EconomicsAction.h"
#include "data/actions/GearAction.h"
#include "data/actions/SkillAction.h"
#include "data/actions/TalkingAction.h"
#include "data/actions/TargetedAction.h"

#include "ai/AI.h"

#include "ai/RoamerAI.h"

#include <random>

Action * RoamerAI::getActions(Adventure * adventure, Character * c) {
  return new BaseAction(ACTION_IDLE, adventure, nullptr, c);
  /*
  Action * result;
  Map * visionMap = updateMap(adventure, c);
  std::list<Character *> threats = getThreats(adventure, visionMap, c, 5);
  float orientation = 0.F;
  if(!threats.empty()) {
    result = attack(adventure, threats, c);
    c->setCurrentAction(result);
    return result;
  }
  selectHungriness(c);
  selectTiredness(c);
  if(hungry) {
    Action * eat_food = eat(adventure, c);
    if(eat_food != nullptr) {
      result = eat_food;
      c->setCurrentAction(result);
      return result;
    }
  }
  if(sleepy && adventure->getLight() < 4) {
    result = new BaseAction(ACTION_IDLE, adventure, nullptr, c);
    c->setCurrentAction(result);
    return result;
  }
  orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
  if(orientation != 360.F) {
    Target * t = new Target();
    t->type = TARGET_TILE;
    t->id = c->getCurrentMap()->id;
    t->x = origin_x;
    t->y = origin_y;
    result = new TargetedAction(ACTION_MOVE, adventure, nullptr, c, t);
    c->setCurrentAction(result);
    return result;
  }
  // we are at destination
  origin_x = rand() % adventure->getWorld()->getMap(c->getCurrentMap()->id)->sizeX;
  origin_y = rand() % adventure->getWorld()->getMap(c->getCurrentMap()->id)->sizeY;
  result = new BaseAction(ACTION_IDLE, adventure, nullptr, c);
  c->setCurrentAction(result);
  return result;
  */
}
