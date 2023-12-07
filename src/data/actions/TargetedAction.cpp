#include "data/Adventure.h"
#include "data/Map.h"
#include "data/Region.h"
#include "data/World.h"

#include "data/items/Gear.h"

#include "data/actions/BaseAction.h"
#include "data/actions/TargetedAction.h"

#include "util/MapUtil.h"

Action * TargetedAction::execute(Adventure * adventure) {
  if(next != nullptr) {
    next->computeTime(adventure);
    if(next->getTime() == 0 && next->type != ACTION_BREAKPOINT) {
      next->execute(adventure);
    }
  }
  user->setOrientation(adventure->getWorld()->setPathToTarget(user->getRegion(), user->getCoord().x, user->getCoord().y, target));
  switch(type) {
    case ACTION_MOVE: {
      float ap = 1.F;
      while(ap > 0.F) {
        //ap = user->getCurrentMap()->move(user, user->getOrientation(), target->x, target->y, ap, adventure->getWorld());
        // we took a MapLink, or maybe got stuck on a wall that wasn't here before, but that would update our Actions anyway
        if(ap > 0.F && target->next != nullptr) {
          Target * temp = target;
          target = target->next;
          delete temp;
          setUserOrientationToTarget(adventure);
        }
      }
      if(ap == 0.F && target->next == nullptr && rangeFromTarget(adventure) > 0.01F) {
        Action * temp = next;
        next = new TargetedAction(ACTION_MOVE, adventure, nullptr, user, target);
        next->setNext(temp);
      }
      break;
    }
    case ACTION_STRIKE: {
      if(user->getGear()->getWeapon_1()->use_projectile || user->getGear()->getWeapon_1()->use_projectile) {
        Projectile * projectile = user->shoot(target, adventure, ITEM_SLOT_WEAPON_1);
        if(projectile != nullptr) {
          //adventure->getWorld()->getMap(user->getCurrentMap()->id)->addProjectile(projectile);
        }
      }
      else {
        if(target->type == TARGET_CHARACTER) {
          Character * other = adventure->getCharacter(target->id); 
          user->attack(other, adventure, ACTION_STRIKE);
          if(!other->isAlive()) {
            //adventure->getWorld()->getMap(user->getCurrentMap()->id)->killCharacter(user, other);
          }
        }
        else {
          /*
          for(Character * c : adventure->getWorld()->getMap(user->getCurrentMap()->id)->getCharacters()) {
            if(c != nullptr && c != user && !c->isEtheral() && c->getX() == target_x && c->getY() == target_y) {
              user->attack(c);
              if(!c->isAlive()) {
                adventure->getWorld()->getMap(user->getCurrentMap()->id)->killCharacter(user, c);
              }
              break;
            }
          }
          */
        }
      }
      Action * temp = next;
      next = new BaseAction(ACTION_RESPITE, adventure, nullptr, user);
      next->setNext(temp);
      break;
    }
    case ACTION_ACTIVATION: {
      Furniture * furniture = user->getRegion()->getFurniture(MapUtil::makeVector3i(target->x, target->y, 0));
      if(furniture != nullptr && furniture->type != FURNITURE_BASIC) {
        ((ActivableFurniture *) furniture)->activate(user, false);
      }
      break;
    }
    default: ;
  }
  if(previous != nullptr) {
    previous->setNext(next);
    if(next != nullptr) {
      next->setPrevious(previous);
    }
  }
  else {
    if(next != nullptr) {
      next->setPrevious(nullptr);
      // tick is in range [0;1]
      next->computeTick(1 - tick);
    }
    else {
      user->setCurrentAction(nullptr);
    }
  }
  return next;
}

void TargetedAction::computeTime(Adventure * adventure) {
  switch(type) {
    case ACTION_MOVE:
      time = 1.F;
      break;
    case ACTION_STRIKE:
      time = user->getStrikeTime(ITEM_SLOT_WEAPON_1);
      break;
    case ACTION_ACTIVATION: {
      Furniture * furniture = user->getRegion()->getFurniture(MapUtil::makeVector3i(target->x, target->y, 0));
      if(furniture != nullptr && furniture->type != FURNITURE_BASIC) {
        time = user->getHandActionTimeModifier() * ((ActivableFurniture *) furniture)->activation_time;
      }
      break;
    }
    default:
      time = 0.F;
  }
  tick = time;
}

Target * TargetedAction::getTarget() { return target; }

void TargetedAction::setUserOrientationToTarget(Adventure * adventure) {
  if(target->type == TARGET_COORDINATES || target->type == TARGET_TILE) {
    user->setOrientation(MapUtil::getOrientationToTarget(user->getCoord().x, user->getCoord().y, target->x, target->y));
  }
  else if(target->type == TARGET_CHARACTER) {
    Character * other = adventure->getCharacter(target->id);
    user->setOrientation(MapUtil::getOrientationToTarget(user->getCoord().x, user->getCoord().y, other->getCoord().x, other->getCoord().y));
  }
}

float TargetedAction::rangeFromTarget(Adventure * adventure) {
  if(target->type == TARGET_COORDINATES || target->type == TARGET_TILE) {
    return MapUtil::distance(user->getCoord(), MapUtil::makeVector3(target->x, target->y, 0));
  }
  else if(target->type == TARGET_CHARACTER) {
    Character * other = adventure->getCharacter(target->id);
    return MapUtil::distance(user->getCoord(), other->getCoord());
  }
  return 0.F;
}

