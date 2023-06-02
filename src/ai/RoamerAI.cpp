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
  Map * visionMap = new Map(adventure->getWorld()->getMap(c->getCurrentMapId()), c, adventure->getDatabase());
  std::list<Character *> threats = getThreats(adventure, visionMap, c, 5);
  float orientation = 0.F;
  if(!threats.empty()) {
    delete visionMap;
    return attack(adventure, threats, c);
  }
  selectHungriness(c);
  selectTiredness(c);
  if(hungry) {
    Action * eat_food = eat(adventure, c);
    if(eat_food != nullptr) {
      delete visionMap;
      return eat_food;
    }
  }
  if(sleepy && adventure->getLight() < 4) {
    delete visionMap;
    return new BaseAction(ACTION_IDLE, adventure, nullptr, c);
  }
  orientation = getFollowOrientation(adventure, c, origin_x, origin_y);
  if(orientation != 360.F) {
    delete visionMap;
    Target * t = new Target();
    t->type = TARGET_TILE;
    t->id = c->getCurrentMapId();
    t->x = origin_x;
    t->y = origin_y;
    return new TargetedAction(ACTION_MOVE, adventure, nullptr, c, t);
  }
  // we are at destination
  origin_x = rand() % adventure->getWorld()->getMap(c->getCurrentMapId())->sizeX;
  origin_y = rand() % adventure->getWorld()->getMap(c->getCurrentMapId())->sizeY;
  delete visionMap;
    return new BaseAction(ACTION_IDLE, adventure, nullptr, c);
}
