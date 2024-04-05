#ifndef _ADVENTURE_H_
#define _ADVENTURE_H_

#include <list>
#include <map>
#include <string>
#include <algorithm>

#include "data/Settings.h"

#include "util/MathUtil.h"

#include "Values.h"

typedef struct Spawn {
  int32_t x;
  int32_t y;
  int32_t z;
} Spawn;

typedef struct StateDisplay {
  std::list<CharacterDisplay *> characters;
  std::list<ProjectileDisplay *> projectiles;
  std::list<Loot *> loots;
  std::list<Speech *> speeches;
  std::list<Furniture *> changed_furnitures;
  bool need_to_send_actions;
} StateDisplay;

class Adventure {
  public:
    const std::string name;
    const std::string filePath;
    const int32_t maxPlayers;
    Adventure(
      std::string name,
      std::string filePath,
      int32_t maxPlayers,
      Database * database,
      World * world,
      std::list<Quest *> quests,
      std::list<Event *> events,
      std::list<Spawn *> spawns,
      std::list<Attributes *> startingAttributes,
      std::list<Way *> startingWays
    ):
      name(name),
      filePath(filePath),
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
      light = 0;
      /*
      light = Settings::getStartingHour() % Settings::getLightMaxPower();
      if(Settings::getStartingHour() < Settings::getLightMaxPower()) {
        lightUp = true;
      } else {
        lightUp = false;
      }
      */
    }
    /*
    Adventure(Save * save) {
      // TODO INI_READER
    }
    */
    Save * save();
    void softMoveCharacterToMap(Character * character, MathUtil::Vector3 coord);
    void hardMoveCharacterToMap(Character * character, MathUtil::Vector3 coord);
    void addPlayer(Character * player);
    void removePlayer(Character * player);
    bool isWiped();
    std::list<Character *> getParty();
    std::list<Character *> getPreservedPlayers();
    void resurrect(Character * player, MathUtil::Vector3 coord);
    int64_t getRound();
    int32_t getTick();
    void incrTick();
    void event();
    World * getWorld();
    int32_t getLight();
    std::list<Attributes *> getStartingAttributes();
    std::list<Way *> getStartingWays();
    Database * getDatabase();
    void addQuest(Quest * quest);
    void removeQuest(Quest * quest);
    std::list<Character *> getCharacters();
    Character * getCharacter(int64_t id);
    Furniture * getFurniture(int64_t id);
    std::list<Projectile *> getProjectiles();
    void getNPCsActions();
    void mergeActions(std::list<Action *> to_add);
    void executeActions();
    void applyDayLight();
    void incrDayLight();
    void actAllProjectiles();
    Character * spawnPlayer(std::string name, Attributes * attr, Race * race, Way * origin, Way * culture, Way * religion, Way * profession);
    void applyIteration();
    std::string getTime();
    std::string state_to_string(std::map<const int64_t, Character *> players);
    StateDisplay * update_state(std::string to_read);
  private:
    World * world;
    Database * database;
    int32_t tick;
    int64_t round;
    int32_t light;
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
