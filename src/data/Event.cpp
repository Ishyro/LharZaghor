#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"
#include "data/Adventure.h"
#include "data/Speech.h"

#include "communication/SpeechManager.h"

#include "data/Event.h"

void Event::activate(Adventure * adventure) {
  for(Map * map : maps) {
    adventure->getWorld()->addMap(map);
  }
  for(MapLink * link : links) {
    adventure->getWorld()->addMapLink(link);
  }
  switch(character_apparition_type) {
    case SOFT:
      for(Character * c : characters) {
        adventure->softMoveCharacterToMap(c, c->getCurrentMapId(), c->getX(), c->getY());
      }
      break;
    case HARD:
      for(Character * c : characters) {
        adventure->softMoveCharacterToMap(c, c->getCurrentMapId(), c->getX(), c->getY());
      }
      break;
  }
  if(hasQuest) {
    adventure->addQuest(quest);
  }
  for(Effect * e : effects) {
    for(Character * c : adventure->getParty()) {
      c->addEffect(e);
    }
  }
  SpeechManager::add(speech);
}
