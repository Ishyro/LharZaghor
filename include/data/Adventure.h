#ifndef _ADVENTURE_H_
#define _ADVENTURE_H_

#include <list>
#include <string>
#include <algorithm>

#include "data/Settings.h"
#include "Values.h"

typedef struct Spawn {
  int x;
  int y;
  int map_id;
} Spawn;

class Adventure {
  public:
    const std::string name;
    const int maxPlayers;
    Adventure(
      std::string name,
      int maxPlayers,
      Database * database,
      World * world,
      std::list<Quest *> quests,
      std::list<Event *> events,
      std::list<Spawn *> spawns,
      std::list<Attributes *> startingAttributes,
      std::list<Way *> startingWays
    ):
      name(name),
      maxPlayers(maxPlayers),
      database(database),
      world(world),
      quests(quests),
      events(events),
      spawns(spawns),
      startingAttributes(startingAttributes),
      startingWays(startingWays),
      party(std::list<Character *>()),
      preserved_players(std::list<Character *>()),
      actions(std::list<Action *>())
    {
      tick = 0;
      round = 0L;
      light = Settings::getStartingHour() % Settings::getLightMaxPower();
      if(Settings::getStartingHour() < Settings::getLightMaxPower()) {
        lightUp = true;
      } else {
        lightUp = false;
      }
    }
    /*
    Adventure(Save * save) {
      // TODO INI_READER
    }
    */
    Save * save();
    void softMoveCharacterToMap(Character * character, int mapId, int y, int x);
    void hardMoveCharacterToMap(Character * character, int mapId, int y, int x);
    void addPlayer(Character * player);
    void removePlayer(Character * player);
    bool isWiped();
    std::list<Character *> getParty();
    std::list<Character *> getPreservedPlayers();
    void resurrect(Character * player, int map_id, int y, int x);
    long getRound();
    int getTick();
    void incrTick();
    void event();
    World * getWorld();
    int getLight();
    std::list<Attributes *> getStartingAttributes();
    std::list<Way *> getStartingWays();
    Database * getDatabase();
    void addQuest(Quest * quest);
    void removeQuest(Quest * quest);
    std::list<Character *> getCharacters();
    Character * getCharacter(long id);
    std::list<Projectile *> getProjectiles();
    void getNPCsActions();
    void mergeActions(std::list<Action *> to_add);
    void executeActions();
    void applyDayLight();
    void incrDayLight();
    std::string getTime();
    void actAllProjectiles();
    Character * spawnPlayer(std::string name, Attributes * attr, Way * race, Way * origin, Way * culture, Way * religion, Way * profession);
    void applyIteration();
  private:
    World * world;
    Database * database;
    int tick;
    long round;
    int light;
    bool lightUp;
    std::list<Character *> party;
    std::list<Character *> preserved_players;
    std::list<Quest *> quests;
    std::list<Event *> events;
    std::list<Spawn *> spawns;
    std::list<Attributes *> startingAttributes;
    std::list<Way *> startingWays;
    std::list<Action *> actions;
};

#endif // _ADVENTURE_H_
