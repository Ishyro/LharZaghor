#include "data/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "ai/AI.h"

#include "ai/PassiveAI.h"

Action * PassiveAI::getAction(Adventure * adventure, Character * c) {
  if(adventure->getLight() >= 4) {
    Map * map = adventure->getWorld()->getMap(c->getCurrentMapId());
    int orientation = NORTH;
    Character * threat;
    int distance_threat = 100;
    for(Character * other : map->getCharacters()) {
      if(other->getTeam() != c->getTeam()) {
        int distance = std::max(abs(c->getX() - other->getX()), abs(c->getY() - other->getY()));
        if(distance <= c->getVisionRange() && distance < distance_threat) {
          threat = other;
          distance_threat = distance;
          // orientation = threat->orientation;
        }
      }
    }
    return new Action(MOVE, c, orientation, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
  } else {
    return new Action(REST, c, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
  }
  return nullptr;
}
