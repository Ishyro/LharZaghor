#include "utils/FileOpener.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>

#include "ai/AI.h"
#include "ai/PlayerAI.h"
#include "ai/DiurnalPassiveAI.h"
#include "ai/NocturnalPassiveAI.h"
#include "ai/DiurnalAgressiveAI.h"
#include "ai/NocturnalAgressiveAI.h"
#include "ai/EtheralCasterAI.h"

#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Character.h"
#include "data/Effect.h"
#include "data/Event.h"
#include "data/Item.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Quest.h"
#include "data/skills/Skill.h"
#include "data/Speech.h"
#include "data/Tile.h"
#include "data/Way.h"
#include "data/Weapon.h"
#include "data/World.h"
#include "data/Database.h"

#include "data/skills/PseudoSkill.h"
#include "data/skills/Skill.h"

#include "data/skills/ChanneledSkill.h"
#include "data/skills/InstantSkill.h"
#include "data/skills/MapLinkerSkill.h"
#include "data/skills/MindControlSkill.h"
#include "data/skills/ProjectileSkill.h"
#include "data/skills/ResurrectionSkill.h"
#include "data/skills/SimpleSkill.h"
#include "data/skills/SummonSkill.h"
#include "data/skills/TeamChangerSkill.h"
#include "data/skills/TeleportSkill.h"
#include "data/skills/TileSwapSkill.h"

#include "data/Settings.h"

namespace FileOpener {

  std::map<const std::string,std::string> getValuesFromFile(std::string fileName) {
    std::map<const std::string, std::string> result = std::map<const std::string,std::string>();
    std::fstream file;
    file.open(fileName, std::ios::in);
    if(!file) {
      std::cout << "File not found: " + fileName << std::endl;
    }
    std::string line;
    std::string delimiter = "=";
    while(getline(file,line)) {
      while(line != "" && std::isspace(line.at(0))) {
        line = line.substr(1, line.length());
      }
      if(line == "!end") {
        break;
      }
      if(line != "" && line.at(0) != '#') {
        std::string key = line.substr(0, line.find(delimiter));
        while(std::isspace(key.at(key.length() - 1))) {
          key = key.substr(0, key.length() - 1);
        }
        if(line.length() - 1 > line.find(delimiter)) {
          std::string value = line.substr(line.find(delimiter) + 1, line.length() - 1);
          while(std::isspace(value.at(0))) {
            value = value.substr(1, value.length() - 1);
          }
          while(std::isspace(value.at(value.length() - 1))) {
            value = value.substr(0, value.length() - 1);
          }
          result.insert(std::make_pair(key,value));
        }
        else {
          result.insert(std::make_pair(key,""));
        }
      }
    }
    file.close();
    return result;
  }

  Adventure * AdventureOpener(std::string fileName) {
    std::string delimiter = ".";
    std::string dataFile = fileName.substr(0, fileName.find(delimiter) + 1) + "data";
    Database * database = DatabaseOpener(dataFile);

    std::fstream file;
    file.open(fileName, std::ios::in);
    if(!file) {
      std::cout << "File not found: " + fileName << std::endl;
    }
    std::string line;
    std::string name;
    // first line is the adventure name
    do {
      getline(file, name);
    } while(name == "" || name.at(0) == '#');
    World * world = new World(name);
    std::list<Quest *> * quests = new std::list<Quest *>();
    std::list<Event *> * events = new std::list<Event *>();
    std::list<Spawn *> * spawns = new std::list<Spawn *>();
    std::list<Attributes *> * startingAttributes = new std::list<Attributes *>();
    std::list<Way *> * startingWays = new std::list<Way *>();

    while(getline(file,line)) {
      while(line != "" && std::isspace(line.at(0))) {
        line = line.substr(1, line.length());
      }
      if(line != "" && line.at(0) != '#') {
        std::string keyword = line.substr(0, line.find(delimiter));
        while(std::isspace(keyword.at(keyword.length() - 1))) {
          keyword = keyword.substr(0, keyword.length() - 1);
        }
        std::string command = line.substr(line.find(delimiter) + 1, line.length() - 1);
        while(std::isspace(command.at(0))) {
          command = command.substr(1, command.length());
        }
        while(std::isspace(command.at(command.length() - 1))) {
          command = command.substr(0, command.length() - 1);
        }
        executeCommand(keyword, command, world, quests, events, spawns, startingAttributes, startingWays, database);
      }
    }
    file.close();
    Adventure * adventure = new Adventure(name, spawns->size(), database, world, *quests, *events, *spawns, *startingAttributes, *startingWays);
    delete quests;
    delete events;
    delete spawns;
    delete startingAttributes;
    delete startingWays;
    return adventure;
  }

  void executeCommand(std::string keyword, std::string command, World * world, std::list<Quest *> * quests, std::list<Event *> * events, std::list<Spawn *> * spawns, std::list<Attributes *> * startingAttributes, std::list<Way *> * startingWays, Database * database) {
    if(keyword == "Character") {
      std::string name = command.substr(0, command.find('%'));
      command = command.substr(command.find('%') + 1, command.length());
      int xp = stoi(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      int x = stoi(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      int y = stoi(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      int orientation = database->getTargetFromMacro(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      std::string map_str = command.substr(0, command.find('%'));
      command = command.substr(command.find('%') + 1, command.length());
      std::string team = command.substr(0, command.find('%'));
      command = command.substr(command.find('%') + 1, command.length());
      std::string ai_str = command.substr(0, command.find('%'));
      command = command.substr(command.find('%') + 1, command.length());
      Way * race = (Way *) database->getWay(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      Way * origin = (Way *) database->getWay(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      Way * culture = (Way *) database->getWay(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      Way * religion = (Way *) database->getWay(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      Way * profession = (Way *) database->getWay(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      Attributes * attributes = (Attributes *) database->getAttributes(command.substr(0, command.find('%')));
      std::list<Way *> * titles = new std::list<Way *>();
      std::istringstream is_titles(command);
      std::string title;
      while(getline(is_titles, title, '%') && title != "none") {
        titles->push_back((Way *) database->getWay(title));
      }
      Map * map = world->getMap(map_str);
      AI * ai;
      if (ai_str == "DiurnalPassiveAI") {
        ai = new DiurnalPassiveAI(x, y);
      }
      else if (ai_str == "NocturnalPassiveAI") {
        ai = new NocturnalPassiveAI(x, y);
      }
      else if (ai_str == "DiurnalAgressiveAI") {
        ai = new DiurnalAgressiveAI(x, y);
      }
      else if (ai_str == "NocturnalAgressiveAI") {
        ai = new NocturnalAgressiveAI(x, y);
      }
      Character * c = new Character(database->getCharacter(name), name, xp, x, y, orientation, map->id, team, ai, attributes, race, origin, culture, religion, profession, *titles);
      map->addCharacter(c);
      delete titles;
    }
    else if(keyword == "Event") {
      Event * event = new Event(database->getEvent(command));
      events->push_back(event);
    }
    else if(keyword == "Map") {
      Map * map = new Map(database->getMap(command.substr(0, command.find('#'))), command);
      world->addMap(map);
    }
    else if(keyword == "MapLink") {
      std::string map1_str = command.substr(0, command.find('%'));
      command = command.substr(command.find('%') + 1, command.length());
      int x1 = stoi(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      int y1 = stoi(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      std::string map2_str = command.substr(0, command.find('%'));
      command = command.substr(command.find('%') + 1, command.length());
      int x2 = stoi(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      int y2 = stoi(command.substr(0, command.find('%')));
      Map * map1 = world->getMap(map1_str);
      Map * map2 = world->getMap(map2_str);
      MapLink * link = new MapLink();
      link->map1 = map1;
      link->map2 = map2;
      link->x1 = x1;
      link->y1 = y1;
      link->x2 = x2;
      link->y2 = y2;
      world->addMapLink(link);
    }
    else if(keyword == "Loot") {

    }
    else if(keyword == "Quest") {
      Quest * quest = new Quest(database->getQuest(command));
      quests->push_back(quest);
    }
    else if(keyword == "Spawn") {
      Spawn * spawn = new Spawn();
      std::string map_str = command.substr(0, command.find('%'));
      command = command.substr(command.find('%') + 1, command.length());
      spawn->x = stoi(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      spawn->y = stoi(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      spawn->orientation = database->getTargetFromMacro(command.substr(0, command.find('%')));
      spawn->map_id = world->getMap(map_str)->id;
      spawns->push_back(spawn);
    }
    else if(keyword == "StartingAttributes") {
      startingAttributes->push_back( (Attributes *) database->getAttributes(command));
    }
    else if(keyword == "StartingWay") {
      startingWays->push_back( (Way *) database->getWay(command));
    }
    else if(keyword == "TeamRelation") {
      int relation = database->getTargetFromMacro(command.substr(0, command.find('%')));
      command = command.substr(command.find('%') + 1, command.length());
      std::string team1 = command.substr(0, command.find('%'));
      command = command.substr(command.find('%') + 1, command.length());
      std::string team2 = command.substr(0, command.find('%'));
      command = command.substr(command.find('%') + 1, command.length());
      database->addRelation(team1, team2, relation);
    }
    else if(keyword == "Traduction") {
      database->addTraductionPath(command);
    }
    else if(keyword == "WayImcompatibility") {
      std::string way1 = command.substr(0, command.find('%'));
      command = command.substr(command.find('%') + 1, command.length());
      std::string way2 = command.substr(0, command.find('%'));
      command = command.substr(command.find('%') + 1, command.length());
      database->addWayImcompatibility(way1, way2);
    }
  }

  void AttributesOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int baseHp = stoi(values.at("baseHp"));
    int baseMana = stoi(values.at("baseMana"));
    int baseArmor = stoi(values.at("baseArmor"));
    int baseSoulBurn = stoi(values.at("baseSoulBurn"));
    int baseFlow = stoi(values.at("baseFlow"));
    int baseVisionRange = stoi(values.at("baseVisionRange"));
    int baseVisionPower = stoi(values.at("baseVisionPower"));
    int baseDetectionRange = stoi(values.at("baseDetectionRange"));
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_effects(values.at("effects"));
    std::string effect;
    while(getline(is_effects, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    std::list<Skill *> * skills = new std::list<Skill *>();
    std::istringstream is_skills(values.at("skills"));
    std::string skill;
    while(getline(is_skills, skill, '%')) {
      skills->push_back((Skill *) database->getSkill(skill));
    }
    std::string head_str = values.at("head");
    Item * head = head_str != "none" ? (Item *) database->getItem(head_str) : nullptr;
    std::string arms_str = values.at("arms");
    Item * arms = arms_str != "none" ? (Item *) database->getItem(arms_str) : nullptr;
    std::string legs_str = values.at("legs");
    Item * legs = legs_str != "none" ? (Item *) database->getItem(legs_str) : nullptr;
    std::string body_str = values.at("body");
    Item * body = body_str != "none" ? (Item *) database->getItem(body_str) : nullptr;
    std::string left_ring_str = values.at("left_ring");
    Item * left_ring = left_ring_str != "none" ? (Item *) database->getItem(left_ring_str) : nullptr;
    std::string right_ring_str = values.at("right_ring");
    Item * right_ring = right_ring_str != "none" ? (Item *) database->getItem(right_ring_str) : nullptr;
    std::string amulet_str = values.at("amulet");
    Item * amulet = amulet_str != "none" ? (Item *) database->getItem(amulet_str) : nullptr;
    std::string weapon_str = values.at("weapon");
    Weapon * weapon = weapon_str != "none" ? (Weapon *) database->getWeapon(weapon_str) : nullptr;
    Gear * gear = new Gear(head, arms, legs, body, left_ring, right_ring, amulet, weapon);
    Attributes * attributes = new Attributes(name, baseHp, baseMana, baseArmor, baseSoulBurn, baseFlow, baseVisionRange, baseVisionPower, baseDetectionRange, *effects, *skills, gear);
    database->addAttributes(attributes);
    delete effects;
    delete skills;
  }

  void CharacterOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    std::istringstream is_player_character(values.at("player_character"));
    bool player_character;
    is_player_character >> std::boolalpha >> player_character;
    Speech * death_speech = nullptr;
    if(values.at("death_speech") != "none") {
      death_speech = (Speech *) database->getSpeech(values.at("death_speech"));
    }
    Speech * talking_speech = nullptr;
    if(values.at("talking_speech") != "none") {
      talking_speech = (Speech *) database->getSpeech(values.at("talking_speech"));
    }
    std::list<Item *> * loots = new std::list<Item *>();
    std::istringstream is_loot(values.at("loot"));
    std::string loot;
    while(getline(is_loot, loot, '%')) {
      loots->push_back((Item *) database->getItem(loot));
    }
    int type = database->getTargetFromMacro(values.at("type"));
    long gold = stol(values.at("gold"));
    std::istringstream is_need_to_eat(values.at("need_to_eat"));
    bool need_to_eat;
    is_need_to_eat >> std::boolalpha >> need_to_eat;
    std::istringstream is_can_eat_food(values.at("can_eat_food"));
    bool can_eat_food;
    is_can_eat_food >> std::boolalpha >> can_eat_food;
    std::istringstream is_need_to_sleep(values.at("need_to_sleep"));
    bool need_to_sleep;
    is_need_to_sleep >> std::boolalpha >> need_to_sleep;
    std::list<Item *> * items = new std::list<Item *>();
    std::istringstream is_3(values.at("items"));
    std::string item;
    while(getline(is_3, item, '%')) {
      items->push_back((Item *) database->getItem(item));
    }
    std::list<Weapon *> * weapons = new std::list<Weapon *>();
    std::istringstream is_4(values.at("weapons"));
    std::string weapon;
    while(getline(is_4, weapon, '%')) {
      weapons->push_back((Weapon *) database->getWeapon(weapon));
    }
    std::list<Ammunition *> * ammunition = new std::list<Ammunition *>();
    std::istringstream is_5(values.at("ammunition"));
    std::string ammo;
    while(getline(is_5, ammo, '%')) {
      std::string ammo_name = ammo.substr(0, ammo.find('|'));
      const Ammunition * base_ammo = database->getAmmunition(ammo_name);
      Ammunition * new_ammo = new Ammunition();
      new_ammo->projectile = base_ammo->projectile;
      new_ammo->gold_value = base_ammo->gold_value;
      new_ammo->ammo_type = base_ammo->ammo_type;
      new_ammo->number = stoi(ammo.substr(ammo.find('|') + 1, ammo.length()));
      ammunition->push_back(new_ammo);
    }
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_6(values.at("effects"));
    std::string effect;
    while(getline(is_6, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    std::list<Skill *> * skills = new std::list<Skill *>();
    std::istringstream is_7(values.at("skills"));
    std::string skill;
    while(getline(is_7, skill, '%')) {
      skills->push_back((Skill *) database->getSkill(skill));
    }
    Character * character = new Character(name, player_character, death_speech, talking_speech, *loots, type, gold, need_to_eat, can_eat_food, need_to_sleep, *items, *weapons, *ammunition, *effects, *skills);
    database->addCharacter(character);
    delete loots;
    delete items;
    delete weapons;
    delete ammunition;
    delete effects;
    delete skills;
  }

  void EffectOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int type = database->getTargetFromMacro(values.at("type"));
    int duration_type = database->getTargetFromMacro(values.at("duration_type"));
    int duration = 0;
    if(duration_type == TEMPORARY) {
      duration = stoi(values.at("duration"));
    }
    int power = stoi(values.at("power"));
    int damages[DAMAGE_TYPE_NUMBER] = {0};
    float damage_reductions[DAMAGE_TYPE_NUMBER] = {0.};
    if(type == DAMAGE || type == DAMAGE_BUFF) {
      damages[SLASH_DAMAGE] = stoi(values.at("SLASH_DAMAGE"));
      damages[PUNCTURE_DAMAGE] = stoi(values.at("PUNCTURE_DAMAGE"));
      damages[IMPACT_DAMAGE] = stoi(values.at("IMPACT_DAMAGE"));
      damages[FIRE_DAMAGE] = stoi(values.at("FIRE_DAMAGE"));
      damages[LIGHTNING_DAMAGE] = stoi(values.at("LIGHTNING_DAMAGE"));
      damages[COLD_DAMAGE] = stoi(values.at("COLD_DAMAGE"));
      damages[POISON_DAMAGE] = stoi(values.at("POISON_DAMAGE"));
      damages[NEUTRAL_DAMAGE] = stoi(values.at("NEUTRAL_DAMAGE"));
      damages[TRUE_DAMAGE] = stoi(values.at("TRUE_DAMAGE"));
      damages[SOUL_DAMAGE] = stoi(values.at("SOUL_DAMAGE"));
    }
    if(type == DAMAGE_REDUCTION) {
      damage_reductions[SLASH_DAMAGE] = stof(values.at("SLASH_REDUCTION"));
      damage_reductions[PUNCTURE_DAMAGE] = stof(values.at("PUNCTURE_REDUCTION"));
      damage_reductions[IMPACT_DAMAGE] = stof(values.at("IMPACT_REDUCTION"));
      damage_reductions[FIRE_DAMAGE] = stof(values.at("FIRE_REDUCTION"));
      damage_reductions[LIGHTNING_DAMAGE] = stof(values.at("LIGHTNING_REDUCTION"));
      damage_reductions[COLD_DAMAGE] = stof(values.at("COLD_REDUCTION"));
      damage_reductions[POISON_DAMAGE] = stof(values.at("POISON_REDUCTION"));
      damage_reductions[NEUTRAL_DAMAGE] = 0.;
      damage_reductions[TRUE_DAMAGE] = 0.;
      damage_reductions[SOUL_DAMAGE] = 0.;
    }
    Effect * effect = new Effect(name, type, duration_type, power, duration, damages, damage_reductions);
    database->addEffect(effect);
  }

  void EventOpener(std::string fileName, Database * database) {}

  void ItemOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    std::istringstream is(values.at("equipable"));
    bool equipable;
    is >> std::boolalpha >> equipable;
    std::istringstream is_2(values.at("consumable"));
    bool consumable;
    is_2 >> std::boolalpha >> consumable;
    int type = database->getTargetFromMacro(values.at("type"));
    float weight = stof(values.at("weight"));
    int gold_value = stoi(values.at("gold_value"));
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_3(values.at("effects"));
    std::string effect;
    while(getline(is_3, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    float damage_reductions[DAMAGE_TYPE_NUMBER] = {0.};
    if(type != UNEQUIPABLE) {
      damage_reductions[SLASH_DAMAGE] = stof(values.at("SLASH_REDUCTION"));
      damage_reductions[PUNCTURE_DAMAGE] = stof(values.at("PUNCTURE_REDUCTION"));
      damage_reductions[IMPACT_DAMAGE] = stof(values.at("IMPACT_REDUCTION"));
      damage_reductions[FIRE_DAMAGE] = stof(values.at("FIRE_REDUCTION"));
      damage_reductions[LIGHTNING_DAMAGE] = stof(values.at("LIGHTNING_REDUCTION"));
      damage_reductions[COLD_DAMAGE] = stof(values.at("COLD_REDUCTION"));
      damage_reductions[POISON_DAMAGE] = stof(values.at("POISON_REDUCTION"));
      damage_reductions[NEUTRAL_DAMAGE] = 0.;
      damage_reductions[TRUE_DAMAGE] = 0.;
      damage_reductions[SOUL_DAMAGE] = 0.;
    }
    Item * item = new Item(name, equipable, consumable, type, weight, gold_value, *effects, damage_reductions);
    database->addItem(item);
    delete effects;
  }

  void MapOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int sizeX = stoi(values.at("width"));
    int sizeY = stoi(values.at("height"));
    std::istringstream is(values.at("outside"));
    bool outside;
    is >> std::boolalpha >> outside;

    Map * map = new Map(name, sizeX, sizeY, outside);

    std::fstream file;
    std::string line;
    file.open(fileName, std::ios::in);
    if(!file) {
      std::cout << "File not found: " + fileName << std::endl;
    }
    while(getline(file,line) && line != "!end");
    for(int y = sizeY - 1; y >= 0; y--) {
      getline(file,line);
      std::istringstream is(line);
      for(int x = 0; x < sizeX; x++) {
        std::string tile;
        getline(is, tile, ' ');
        map->setTile(y, x, (Tile *)database->getTile(values.at(tile)));
      }
    }

    map->calculateLights();
    file.close();
    database->addMap(map);
  }

  void ProjectileOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int projectile_type = database->getTargetFromMacro(values.at("projectile_type"));
    std::istringstream is(values.at("homing"));
    bool homing;
    is >> std::boolalpha >> homing;
    std::string skill_str = values.at("skill");
    Skill * skill = nullptr;
    if(skill_str != "none") {
      skill = (Skill *) database->getSkill(skill_str);
    }
    int speed = stoi(values.at("speed"));
    int area = stoi(values.at("area"));
    int falloff_range = stoi(values.at("falloff_range"));
    float waste_per_tile = stof(values.at("waste_per_tile"));
    float waste_per_tile_area = stof(values.at("waste_per_tile_area"));
    float waste_per_hit = stof(values.at("waste_per_hit"));
    int damages[DAMAGE_TYPE_NUMBER];
    damages[SLASH_DAMAGE] = stoi(values.at("SLASH_DAMAGE"));
    damages[PUNCTURE_DAMAGE] = stoi(values.at("PUNCTURE_DAMAGE"));
    damages[IMPACT_DAMAGE] = stoi(values.at("IMPACT_DAMAGE"));
    damages[FIRE_DAMAGE] = stoi(values.at("FIRE_DAMAGE"));
    damages[LIGHTNING_DAMAGE] = stoi(values.at("LIGHTNING_DAMAGE"));
    damages[COLD_DAMAGE] = stoi(values.at("COLD_DAMAGE"));
    damages[POISON_DAMAGE] = stoi(values.at("POISON_DAMAGE"));
    damages[NEUTRAL_DAMAGE] = stoi(values.at("NEUTRAL_DAMAGE"));
    damages[TRUE_DAMAGE] = stoi(values.at("TRUE_DAMAGE"));
    damages[SOUL_DAMAGE] = stoi(values.at("SOUL_DAMAGE"));
    Projectile * projectile = new Projectile(name, projectile_type, homing, skill, speed, area, falloff_range, waste_per_tile, waste_per_tile_area, waste_per_hit, damages);
    database->addProjectile(projectile);
  }

  void AmmunitionOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    Ammunition * ammo = new Ammunition();
    ammo->projectile = (Projectile *) database->getProjectile(values.at("projectile"));
    ammo->gold_value = stoi(values.at("gold_value"));
    ammo->ammo_type = database->getTargetFromMacro(values.at("ammo_type"));
    database->addAmmunition(ammo);
  }

  void QuestOpener(std::string fileName, Database * database) {}

  void SkillOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int target_type = database->getTargetFromMacro(values.at("target_type"));
    int overcharge_power_type = database->getTargetFromMacro(values.at("overcharge_power_type"));
    int overcharge_duration_type = database->getTargetFromMacro(values.at("overcharge_duration_type"));
    int overcharge_range_type = database->getTargetFromMacro(values.at("overcharge_range_type"));
    int range = stoi(values.at("range"));
    float priority = stof(values.at("priority"));
    std::list<PseudoSkill *> * skills = new std::list<PseudoSkill *>();
    std::istringstream is_skills(values.at("skills"));
    std::string pseudoSkill;
    while(getline(is_skills, pseudoSkill, '%')) {
      skills->push_back((PseudoSkill *) database->getPseudoSkill(pseudoSkill));
    }
    Skill * skill = new Skill(name, target_type, overcharge_power_type, overcharge_duration_type, overcharge_range_type, range, priority, *skills);
    database->addSkill(skill);
    delete skills;
  }

  void PseudoSkillOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int skill_type = database->getTargetFromMacro(values.at("skill_type"));
    int target_type = database->getTargetFromMacro(values.at("target_type"));
    int mana_cost = stoi(values.at("mana_cost"));
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_effects(values.at("effects"));
    std::string effect;
    PseudoSkill * pseudoSkill;
    while(getline(is_effects, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    switch(skill_type) {
      case SIMPLE_SKILL:
        pseudoSkill = new SimpleSkill(name, skill_type, target_type, mana_cost, *effects);
        break;
      case PROJECTILE_SKILL: {
        Projectile * projectile = (Projectile *) database->getProjectile(values.at("projectile"));
        pseudoSkill = new ProjectileSkill(name, skill_type, target_type, mana_cost, *effects);
        ((ProjectileSkill *) pseudoSkill)->setProjectile(projectile);
        break;
      }
      case TELEPORT_SKILL: {
        int apparition_type = database->getTargetFromMacro(values.at("apparition_type"));
        pseudoSkill = new TeleportSkill(name, skill_type, target_type, mana_cost, *effects);
        ((TeleportSkill *) pseudoSkill)->setApparitionType(apparition_type);
        break;
      }
      case TILE_SWAP_SKILL: {
        Tile * current_tile = (Tile *) database->getTile(values.at("current_tile"));
        Tile * new_tile = (Tile *) database->getTile(values.at("new_tile"));
        pseudoSkill = new TileSwapSkill(name, skill_type, target_type, mana_cost, *effects);
        ((TileSwapSkill *) pseudoSkill)->setCurrentTile(current_tile);
        ((TileSwapSkill *) pseudoSkill)->setNewTile(new_tile);
        break;
      }
      case SUMMON_SKILL: {
        Character * character = (Character *) database->getCharacter(values.at("character"));
        std::string ai_str = values.at("ai");
        Way * race = (Way *) database->getWay(values.at("race"));
        Way * origin = (Way *) database->getWay(values.at("origin"));
        Way * culture = (Way *) database->getWay(values.at("culture"));
        Way * religion = (Way *) database->getWay(values.at("religion"));
        Way * profession = (Way *) database->getWay(values.at("profession"));
        Attributes * attributes = (Attributes *) database->getAttributes(values.at("attributes"));
        std::list<Way *> * titles = new std::list<Way *>();
        std::istringstream is_titles(values.at("titles"));
        std::string title;
        while(getline(is_titles, title, '%') && title != "none") {
          titles->push_back((Way *) database->getWay(title));
        }
        int apparition_type = database->getTargetFromMacro(values.at("apparition_type"));
        int xp = stoi(values.at("xp"));
        pseudoSkill = new SummonSkill(name, skill_type, target_type, mana_cost, *effects);
        ((SummonSkill *) pseudoSkill)->setCharacter(character);
        ((SummonSkill *) pseudoSkill)->setAI(ai_str);
        ((SummonSkill *) pseudoSkill)->setRace(race);
        ((SummonSkill *) pseudoSkill)->setOrigin(origin);
        ((SummonSkill *) pseudoSkill)->setCulture(culture);
        ((SummonSkill *) pseudoSkill)->setReligion(religion);
        ((SummonSkill *) pseudoSkill)->setProfession(profession);
        ((SummonSkill *) pseudoSkill)->setAttributes(attributes);
        ((SummonSkill *) pseudoSkill)->setTitles(*titles);
        ((SummonSkill *) pseudoSkill)->setApparitionType(apparition_type);
        ((SummonSkill *) pseudoSkill)->setXp(xp);
        delete titles;
        break;
      }
      default:
        pseudoSkill = new SimpleSkill(name, skill_type, target_type, mana_cost, *effects);
    }
    database->addPseudoSkill(pseudoSkill);
    delete effects;
  }

  void SpeechOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int type = database->getTargetFromMacro(values.at("type"));
    std::istringstream is_empty(values.at("empty"));
    bool empty;
    is_empty >> std::boolalpha >> empty;
    std::istringstream is_constant(values.at("constant"));
    bool constant;
    is_constant >> std::boolalpha >> constant;
    std::map<const std::string, const std::string> * options = new std::map<const std::string, const std::string>();
    std::istringstream is_options(values.at("options"));
    std::string option;
    while(getline(is_options, option, '%')) {
      std::string pair1 = option.substr(0, option.find('|'));
      std::string pair2 = option.substr(option.find('|') + 1, option.length());
      options->insert(std::make_pair(pair1, pair2));
    }
    Speech * speech = new Speech(name, type, empty, constant, *options);
    database->addSpeech(speech);
    delete options;
  }

  void TileOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    std::istringstream is(values.at("untraversable"));
    bool untraversable;
    is >> std::boolalpha >> untraversable;
    int light = stoi(values.at("light"));
    Tile * tile = new Tile(name, untraversable, light);
    database->addTile(tile);
  }

  void WayOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int type = database->getTargetFromMacro(values.at("type"));
    int hpIncr = stoi(values.at("hpIncr"));
    int manaIncr = stoi(values.at("manaIncr"));
    int armorIncr = stoi(values.at("armorIncr"));
    int soulBurnIncr = stoi(values.at("soulBurnIncr"));
    int flowIncr = stoi(values.at("flowIncr"));
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_1(values.at("effects"));
    std::string effect;
    while(getline(is_1, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    std::list<Skill *> * skills = new std::list<Skill *>();
    std::istringstream is_2(values.at("skills"));
    std::string skill;
    while(getline(is_2, skill, '%')) {
      skills->push_back((Skill *) database->getSkill(skill));
    }
    Way * way = new Way(name, type, hpIncr, manaIncr, armorIncr, soulBurnIncr, flowIncr, *effects, *skills);
    database->addWay(way);
    delete effects;
    delete skills;
  }

  void WeaponOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    std::istringstream is(values.at("melee"));
    bool melee;
    is >> std::boolalpha >> melee;
    int range = stoi(values.at("range"));
    int type = database->getTargetFromMacro(values.at("type"));
    float weight = stof(values.at("weight"));
    int gold_value = stoi(values.at("gold_value"));
    std::istringstream is_2(values.at("use_ammo"));
    bool use_ammo;
    is_2 >> std::boolalpha >> use_ammo;
    int ammo_type = 0;
    int capacity = 0;
    if(use_ammo) {
      ammo_type = database->getTargetFromMacro(values.at("ammo_type"));
      capacity = database->getTargetFromMacro(values.at("capacity"));
    }
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_3(values.at("effects"));
    std::string effect;
    while(getline(is_3, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    int damages[DAMAGE_TYPE_NUMBER];
    damages[SLASH_DAMAGE] = stoi(values.at("SLASH_DAMAGE"));
    damages[PUNCTURE_DAMAGE] = stoi(values.at("PUNCTURE_DAMAGE"));
    damages[IMPACT_DAMAGE] = stoi(values.at("IMPACT_DAMAGE"));
    damages[FIRE_DAMAGE] = stoi(values.at("FIRE_DAMAGE"));
    damages[LIGHTNING_DAMAGE] = stoi(values.at("LIGHTNING_DAMAGE"));
    damages[COLD_DAMAGE] = stoi(values.at("COLD_DAMAGE"));
    damages[POISON_DAMAGE] = stoi(values.at("POISON_DAMAGE"));
    damages[NEUTRAL_DAMAGE] = stoi(values.at("NEUTRAL_DAMAGE"));
    damages[TRUE_DAMAGE] = stoi(values.at("TRUE_DAMAGE"));
    damages[SOUL_DAMAGE] = stoi(values.at("SOUL_DAMAGE"));
    Weapon * weapon = new Weapon(name, melee, range, type, weight, gold_value, use_ammo, ammo_type, capacity, *effects, damages);
    database->addWeapon(weapon);
    delete effects;
  }

  void FileOpener(std::string fileName, Database * database) {
    std::fstream file;
    char delimiter = '/';
    std::string dirname = fileName.substr(0, fileName.rfind(delimiter));
    std::string last_folder = dirname.substr(dirname.rfind(delimiter) + 1, dirname.length() - 1);
    if(last_folder == "attributes") {
      AttributesOpener(fileName, database);
    }
    else if(last_folder == "characters") {
      CharacterOpener(fileName, database);
    }
    else if(last_folder == "effects") {
      EffectOpener(fileName, database);
    }
    else if(last_folder == "events") {
      EventOpener(fileName, database);
    }
    else if(last_folder == "items") {
      ItemOpener(fileName, database);
    }
    else if(last_folder == "maps") {
      MapOpener(fileName, database);
    }
    else if(last_folder == "projectiles") {
      ProjectileOpener(fileName, database);
    }
    else if(last_folder == "ammunition") {
      AmmunitionOpener(fileName, database);
    }
    else if(last_folder == "quests") {
      QuestOpener(fileName, database);
    }
    else if(last_folder == "skills") {
      SkillOpener(fileName, database);
    }
    else if(last_folder == "pseudoskills") {
      PseudoSkillOpener(fileName, database);
    }
    else if(last_folder == "speechs") {
      SpeechOpener(fileName, database);
    }
    else if(last_folder == "tiles") {
      TileOpener(fileName, database);
    }
    else if(last_folder == "ways") {
      WayOpener(fileName, database);
    }
    else if(last_folder == "weapons") {
      WeaponOpener(fileName, database);
    }
  }

  Database * DatabaseOpener(std::string fileName) {
    Database * database = new Database();
    std::fstream file;
    file.open(fileName, std::ios::in);
    if(!file) {
      std::cout << "File not found: " + fileName << std::endl;
    }
    std::string line;
    while(getline(file,line)) {
      while(line != "" && std::isspace(line.at(0))) {
        line = line.substr(1, line.length());
      }
      if(line != "" && line.at(0) != '#') {
        while(std::isspace(line.at(line.length() - 1))) {
          line = line.substr(0, line.length() - 1);
        }
        FileOpener(line, database);
      }
    }
    file.close();
    return database;
  }
}
