#include "data/Adventure.h"
#include "data/Map.h"
#include "data/World.h"

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
  user->setOrientation(adventure->getWorld()->setPathToTarget(user->getCurrentMap()->id, user->getX(), user->getY(), target));
  switch(type) {
    case ACTION_MOVE: {
      float ap = 1.F;
      while(ap > 0.F) {
        ap = user->getCurrentMap()->move(user, user->getOrientation(), target->x, target->y, ap, adventure->getWorld());
        // we took a MapLink, or maybe got stuck on a wall that wasn't here before, but that would update our Actions anyway
        if(ap > 0.F && target->next != nullptr) {
          Target * temp = target;
          target = target->next;
          delete temp;
          setUserOrientationToTarget(adventure);
        }
      }
      if(ap == 0.F && target->next == nullptr && rangeFromTarget(adventure) > 0.01F && (next == nullptr || next->type != ACTION_MOVE)) {
        Action * temp = next;
        next = new TargetedAction(ACTION_MOVE, adventure, nullptr, user, target);
        next->setNext(temp);
      }
      break;
    }
    case ACTION_SHOOT: {
      Projectile * projectile = user->shoot(target, adventure);
      if(projectile != nullptr) {
        adventure->getWorld()->getMap(user->getCurrentMap()->id)->addProjectile(projectile);
      }
      break;
    }
    case ACTION_STRIKE: {
      if(target->type == TARGET_CHARACTER) {
        Character * other = adventure->getCharacter(target->id); 
        user->attack(other, ACTION_STRIKE);
        if(!other->isAlive()) {
          adventure->getWorld()->getMap(user->getCurrentMap()->id)->killCharacter(user, other);
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
      Action * temp = next;
      next = new BaseAction(ACTION_RESPITE, adventure, nullptr, user);
      next->setNext(temp);
      break;
    }
    case ACTION_HEAVY_STRIKE: {
      if(target->type == TARGET_CHARACTER) {
        Character * other = adventure->getCharacter(target->id); 
        user->attack(other, ACTION_HEAVY_STRIKE);
        if(!other->isAlive()) {
          adventure->getWorld()->getMap(user->getCurrentMap()->id)->killCharacter(user, other);
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
      user->setNeedToUpdateActions(true);
    }
  }
  return next;
}

void TargetedAction::computeTime(Adventure * adventure) {
  switch(type) {
    case ACTION_MOVE:
      time = 1.F;
      break;
    case ACTION_SHOOT:
      time = user->getStrikeTime();
      break;
    case ACTION_STRIKE:
      time = user->getStrikeTime();
      break;
    case ACTION_HEAVY_STRIKE:
      time = user->getStrikeTime() * 5;
      break;
    default:
      time = 0.F;
  }
  tick = time;
}

Target * TargetedAction::getTarget() { return target; }

void TargetedAction::setUserOrientationToTarget(Adventure * adventure) {
  if(target->type == TARGET_COORDINATES || target->type == TARGET_TILE) {
    if(target->id == user->getCurrentMap()->id) {
      user->setOrientation(MapUtil::getOrientationToTarget(user->getX(), user->getY(), target->x, target->y));
    }
    // shouldn't happen if Target is well built
    else {
      user->setOrientation(adventure->getWorld()->setPathToTarget(user->getCurrentMap()->id, user->getX(), user->getY(), target));
    }
  }
  else if(target->type == TARGET_CHARACTER) {
    if(target->id == user->getCurrentMap()->id) {
      Character * other = adventure->getCharacter(target->id);
      user->setOrientation(MapUtil::getOrientationToTarget(user->getX(), user->getY(), other->getX(), other->getY()));
    }
    // shouldn't happen if Target is well built
    else {
      user->setOrientation(adventure->getWorld()->setPathToTarget(user->getCurrentMap()->id, user->getX(), user->getY(), target));
    }
  }
}

float TargetedAction::rangeFromTarget(Adventure * adventure) {
  if(target->type == TARGET_COORDINATES || target->type == TARGET_TILE) {
    return MapUtil::distance(user->getX(), user->getY(), target->x, target->y);
  }
  else if(target->type == TARGET_CHARACTER) {
    Character * other = adventure->getCharacter(target->id);
    return MapUtil::distance(user->getX(), user->getY(), other->getX(), other->getY());
  }
}

