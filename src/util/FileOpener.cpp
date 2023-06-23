#include "util/FileOpener.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <regex>

// time
#include<ctime>

#include "ai/AI.h"
#include "ai/PlayerAI.h"
#include "ai/DiurnalPassiveAI.h"
#include "ai/NocturnalPassiveAI.h"
#include "ai/DiurnalAgressiveAI.h"
#include "ai/NocturnalAgressiveAI.h"
#include "ai/EtheralCasterAI.h"
#include "ai/GuardAI.h"
#include "ai/RoamerAI.h"

#include "data/Adventure.h"
#include "data/Attributes.h"
#include "data/Character.h"
#include "data/Effect.h"
#include "data/Event.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Quest.h"
#include "data/Settings.h"
#include "data/skills/Skill.h"
#include "data/Speech.h"
#include "data/Tile.h"
#include "data/Way.h"
#include "data/Race.h"
#include "data/World.h"
#include "data/Database.h"

#include "data/items/Item.h"
#include "data/items/ArmorItem.h"
#include "data/items/WeaponItem.h"
#include "data/items/SerialItem.h"
#include "data/items/AmmunitionItem.h"
#include "data/items/Gear.h"

#include "data/skills/PseudoSkill.h"
#include "data/skills/Skill.h"

#include "data/skills/ChanneledSkill.h"
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
#include "data/ClientSettings.h"

#include "util/String.h"

namespace FileOpener {

  std::map<const std::string,std::string> getValuesFromFile(std::string fileName) {
    std::map<const std::string, std::string> result = std::map<const std::string,std::string>();
    std::fstream file;
    std::string os_fileName = std::regex_replace(fileName, std::regex("/"), PATH_DELIMITER);
    file.open(os_fileName, std::ios::in);
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
        while(key.length() > 0 && std::isspace(key.at(key.length() - 1))) {
          key = key.substr(0, key.length() - 1);
        }
        if(line.length() - 1 > line.find(delimiter)) {
          std::string value = line.substr(line.find(delimiter) + 1, line.length() - 1);
          while(value.length() > 0 && std::isspace(value.at(0))) {
            value = value.substr(1, value.length() - 1);
          }
          while(value.length() > 0 && std::isspace(value.at(value.length() - 1))) {
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

  void getCoordinates(std::string to_read, long & map_id, int & y, int & x, World * world) {
    std::stringstream * ss = new std::stringstream(to_read);
    map_id = world->getMap(String::extract(ss))->id;
    x = String::extract_int(ss);
    y = String::extract_int(ss);
    delete ss;
  }

  Adventure * AdventureOpener(std::string fileName, bool isServer) {
    std::string delimiter = ".";
    std::string dataFile = std::regex_replace(fileName, std::regex(".commands"), ".data");
    Database * database = DatabaseOpener(dataFile, isServer);
    if(isServer) {
      SettingsOpener("data" + PATH_DELIMITER + "settings_server.data", database);
    }
    std::fstream file;
    std::string os_fileName = std::regex_replace(fileName, std::regex("/"), PATH_DELIMITER);
    file.open(os_fileName, std::ios::in);
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
        executeCommand(keyword, command, world, quests, events, spawns, startingAttributes, startingWays, database, isServer);
      }
    }
    file.close();
    // we need to escape '\' 2 times
    Adventure * adventure = new Adventure(name, std::regex_replace(fileName, std::regex("\\\\"), "/"), spawns->size(), database, world, *quests, *events, *spawns, *startingAttributes, *startingWays);
    delete quests;
    delete events;
    delete spawns;
    delete startingAttributes;
    delete startingWays;
    return adventure;
  }

  void executeCommand(std::string keyword, std::string command, World * world, std::list<Quest *> * quests, std::list<Event *> * events, std::list<Spawn *> * spawns, std::list<Attributes *> * startingAttributes, std::list<Way *> * startingWays, Database * database, bool isServer) {
    std::stringstream * ss = new std::stringstream(command);
    if(keyword == "Character" && isServer) {
      std::string name = String::extract(ss);
      std::string coord = String::extract(ss);
      int x, y;
      long map_id;
      getCoordinates(coord, map_id, y, x, world);
      std::string team = String::extract(ss);
      std::string ai_str = String::extract(ss);
      Attributes * attributes = (Attributes *) database->getAttributes(String::extract(ss));
      Attributes * second_attributes = (Attributes *) database->getAttributes(String::extract(ss));
      Gear * gear = (Gear *) database->getGear(String::extract(ss));
      
      Race * race = (Race *) database->getWay(String::extract(ss));
      std::stringstream * ss_race_modifiers = new std::stringstream(String::extract(ss));
      std::list<Race *> * race_modifiers = new std::list<Race *>();
      while(ss_race_modifiers->rdbuf()->in_avail() != 0) {
        race_modifiers->push_back((Race *) database->getWay(String::extract(ss_race_modifiers)));
      }
      delete ss_race_modifiers;
      Way * origin = (Way *) database->getWay(String::extract(ss));
      Way * culture = (Way *) database->getWay(String::extract(ss));
      Way * religion = (Way *) database->getWay(String::extract(ss));
      Way * profession = (Way *) database->getWay(String::extract(ss));
      std::stringstream * ss_titles = new std::stringstream(String::extract(ss));
      std::list<Way *> * titles = new std::list<Way *>();
      while(ss_titles->rdbuf()->in_avail() != 0) {
        titles->push_back((Way *) database->getWay(String::extract(ss_titles)));
      }
      delete ss_titles;
      long xp = String::extract_long(ss);
      int gold = String::extract_int(ss);
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
      else if (ai_str == "GuardAI") {
        ai = new GuardAI(x, y);
      }
      else if (ai_str == "RoamerAI") {
        ai = new RoamerAI(x, y);
      }
      Character * c = new Character(
        database->getCharacter(name),
        name,
        xp,
        gold,
        x,
        y,
        90.F,
        map_id,
        team,
        ai,
        attributes,
        second_attributes,
        gear,
        race,
        *race_modifiers,
        origin,
        culture,
        religion,
        profession,
        *titles
      );
      world->getMap(map_id)->addCharacter(c);
      delete race_modifiers;
      delete titles;
    }
    else if(keyword == "Event" && isServer) {
      Event * event = new Event(database->getEvent(String::extract(ss)));
      events->push_back(event);
    }
    else if(keyword == "Map") {
      std::string map_name = String::extract(ss);
      Map * map = new Map(database->getMap(map_name.substr(0, map_name.find('#'))), map_name);
      world->addMap(map);
    }
    else if(keyword == "MapLink") {
      long id1, id2;
      MapLink * link = new MapLink();
      std::string coord = String::extract(ss);
      getCoordinates(coord, id1, link->y1, link->x1, world);
      coord = String::extract(ss);
      getCoordinates(coord, id2, link->y2, link->x2, world);
      link->type = database->getTargetFromMacro(String::extract(ss));
      Map * map1 = world->getMap(id1);
      Map * map2 = world->getMap(id2);
      link->map1 = map1;
      link->map2 = map2;
      world->addMapLink(link);
      world->addMapLink(link);
    }
    else if(keyword == "Loot" && isServer) {

    }
    else if(keyword == "Quest") {
      Quest * quest = new Quest(database->getQuest(String::extract(ss)));
      quests->push_back(quest);
    }
    else if(keyword == "Settings") {
      std::string setting = String::extract(ss);
      std::string value_str = String::extract(ss);
      if(setting == "LIGHT_MAX_POWER") {
        Settings::setLightMaxPower(stoi(value_str));
      }
      else if(setting == "LIGHT_DURATION") {
        Settings::setLighDuration(stoi(value_str));
      }
      else if(setting == "YEAR_DURATION") {
        Settings::setYearDuration(stoi(value_str));
      }
      else if(setting == "MONTH_DURATION") {
        Settings::setMonthDuration(stoi(value_str));
      }
      else if(setting == "WEEK_DURATION") {
        Settings::setWeekDuration(stoi(value_str));
      }
      else if(setting == "HOUR_DURATION") {
        Settings::setHourDuration(stoi(value_str));
      }
      else if(setting == "MINUTE_DURATION") {
        Settings::setMinuteDuration(stoi(value_str));
      }
      else if(setting == "STARTING_YEAR") {
        Settings::setStartingYear(stoi(value_str));
      }
      else if(setting == "STARTING_MONTH") {
        Settings::setStartingMonth(stoi(value_str));
      }
      else if(setting == "STARTING_WEEK") {
        Settings::setStartingWeek(stoi(value_str));
      }
      else if(setting == "STARTING_DAY") {
        Settings::setStartingDay(stoi(value_str));
      }
      else if(setting == "STARING_HOUR") {
        Settings::setStartingHour(stoi(value_str));
      }
    }
    else if(keyword == "Spawn" && isServer) {
      Spawn * spawn = new Spawn();
      std::string coord = String::extract(ss);
      getCoordinates(coord, spawn->map_id, spawn->y, spawn->x, world);
      spawns->push_back(spawn);
    }
    else if(keyword == "StartingAttributes") {
      startingAttributes->push_back( (Attributes *) database->getAttributes(String::extract(ss)));
    }
    else if(keyword == "StartingWay") {
      startingWays->push_back( (Way *) database->getWay(String::extract(ss)));
    }
    else if(keyword == "TeamRelation") {
      int relation = database->getTargetFromMacro(String::extract(ss));
      std::string team1 = String::extract(ss);
      std::string team2 = String::extract(ss);
      database->addRelation(team1, team2, relation);
    }
    else if(keyword == "Translation" && !isServer) {
      database->addTranslationPath(String::extract(ss));
    }
    else if(keyword == "WayImcompatibility") {
      std::string way1 = String::extract(ss);
      std::string way2 = String::extract(ss);
      database->addWayImcompatibility(way1, way2);
    }
    delete ss;
  }

  void AttributesOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    Attributes * archetype = nullptr;
    std::string archetype_str = values.at("archetype");
    if(archetype_str != "none") {
      archetype = (Attributes *) database->getAttributes(archetype_str);
    }
    int baseHp = stoi(values.at("baseHp"));
    int baseMana = stoi(values.at("baseMana"));
    int baseArmorMult = stoi(values.at("baseArmorMult"));
    int baseDamageMult = stoi(values.at("baseDamageMult"));
    int baseSoulBurn = stoi(values.at("baseSoulBurn"));
    int baseFlow = stoi(values.at("baseFlow"));
    int baseVisionRange = stoi(values.at("baseVisionRange"));
    int baseVisionPower = stoi(values.at("baseVisionPower"));
    int baseDetectionRange = stoi(values.at("baseDetectionRange"));
    int hpIncr = stoi(values.at("hpIncr"));
    int manaIncr = stoi(values.at("manaIncr"));
    int armorMultIncr = stoi(values.at("armorMultIncr"));
    int damageMultIncr = stoi(values.at("damageMultIncr"));
    int soulBurnIncr = stoi(values.at("soulBurnIncr"));
    int flowIncr = stoi(values.at("flowIncr"));
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
    Attributes * attributes = new Attributes(
      name,
      archetype,
      baseHp,
      baseMana,
      baseArmorMult,
      baseDamageMult,
      baseSoulBurn,
      baseFlow,
      baseVisionRange,
      baseVisionPower,
      baseDetectionRange,
      hpIncr,
      manaIncr,
      armorMultIncr,
      damageMultIncr,
      soulBurnIncr,
      flowIncr,
      *effects,
      *skills
    );
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
    std::istringstream is_merchant(values.at("merchant"));
    bool merchant;
    is_merchant >> std::boolalpha >> merchant;
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
    std::list<Item *> * sellable_items = new std::list<Item *>();
    std::string item;
    std::istringstream is_sellable_items(values.at("sellable_items"));
    while(getline(is_sellable_items, item, '%')) {
      sellable_items->push_back((Item *) database->getItem(item));
    }
    std::list<Effect *> * sellable_effects = new std::list<Effect *>();
    std::istringstream is_sellable_effects(values.at("sellable_effects"));
    while(getline(is_sellable_effects, effect, '%')) {
      sellable_effects->push_back((Effect *) database->getEffect(effect));
    }
    std::list<Skill *> * sellable_skills = new std::list<Skill *>();
    std::istringstream is_sellable_skills(values.at("sellable_skills"));
    while(getline(is_sellable_skills, skill, '%')) {
      sellable_skills->push_back((Skill *) database->getSkill(skill));
    }
    Character * character = new Character(
      name,
      player_character,
      death_speech,
      talking_speech,
      merchant,
      *effects,
      *skills,
      *sellable_items,
      *sellable_effects,
      *sellable_skills
    );
    database->addCharacter(character);
    delete effects;
    delete skills;
    delete sellable_items;
    delete sellable_effects;
    delete sellable_skills;
  }

  void EffectOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int level = stoi(values.at("level"));
    std::string attributes = values.at("attributes");
    int type = database->getTargetFromMacro(values.at("type"));
    int duration_type = database->getTargetFromMacro(values.at("duration_type"));
    int duration = 0;
    if(duration_type == DURATION_TEMPORARY) {
      duration = stoi(values.at("duration"));
    }
    int power = stoi(values.at("power"));
    int damages[DAMAGE_TYPE_NUMBER] = {0};
    float damage_reductions[DAMAGE_TYPE_NUMBER] = {0.};
    if(type == EFFECT_DAMAGE || type == EFFECT_DAMAGE_BUFF) {
      damages[DAMAGE_SLASH] = stoi(values.at("DAMAGE_SLASH"));
      damages[DAMAGE_PUNCTURE] = stoi(values.at("DAMAGE_PUNCTURE"));
      damages[DAMAGE_IMPACT] = stoi(values.at("DAMAGE_IMPACT"));
      damages[DAMAGE_FIRE] = stoi(values.at("DAMAGE_FIRE"));
      damages[DAMAGE_LIGHTNING] = stoi(values.at("DAMAGE_LIGHTNING"));
      damages[DAMAGE_FROST] = stoi(values.at("DAMAGE_FROST"));
      damages[DAMAGE_POISON] = stoi(values.at("DAMAGE_POISON"));
      damages[DAMAGE_ACID] = stoi(values.at("DAMAGE_ACID"));
      damages[DAMAGE_MIND] = stoi(values.at("DAMAGE_MIND"));
      damages[DAMAGE_TRUE] = stoi(values.at("DAMAGE_TRUE"));
      damages[DAMAGE_SOUL] = stoi(values.at("DAMAGE_SOUL"));
    }
    if(type == EFFECT_DAMAGE_REDUCTION) {
      damage_reductions[DAMAGE_SLASH] = stof(values.at("SLASH_REDUCTION"));
      damage_reductions[DAMAGE_PUNCTURE] = stof(values.at("PUNCTURE_REDUCTION"));
      damage_reductions[DAMAGE_IMPACT] = stof(values.at("IMPACT_REDUCTION"));
      damage_reductions[DAMAGE_FIRE] = stof(values.at("FIRE_REDUCTION"));
      damage_reductions[DAMAGE_LIGHTNING] = stof(values.at("LIGHTNING_REDUCTION"));
      damage_reductions[DAMAGE_FROST] = stof(values.at("COLD_REDUCTION"));
      damage_reductions[DAMAGE_POISON] = stof(values.at("POISON_REDUCTION"));
      damage_reductions[DAMAGE_ACID] = 0.;
      damage_reductions[DAMAGE_MIND] = stof(values.at("MIND_REDUCTION"));
      damage_reductions[DAMAGE_TRUE] = 0.;
      damage_reductions[DAMAGE_SOUL] = 0.;
    }
    Effect * effect = new Effect(name, 0, level, attributes, type, duration_type, power, duration, damages, damage_reductions);
    database->addEffect(effect);
  }

  void EventOpener(std::string fileName, Database * database) {}

  void GearOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    std::string head_str = values.at("head");
    ArmorItem * head = head_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(head_str.substr(0, head_str.find('|'))), stoi(head_str.substr(head_str.find('|') + 1, head_str.length())), 1
    ) : nullptr;
    std::string arms_str = values.at("arms");
    ArmorItem * arms = arms_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(arms_str.substr(0, arms_str.find('|'))), stoi(arms_str.substr(arms_str.find('|') + 1, arms_str.length())), 1
    ) : nullptr;
    std::string legs_str = values.at("legs");
    ArmorItem * legs = legs_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(legs_str.substr(0, legs_str.find('|'))), stoi(legs_str.substr(legs_str.find('|') + 1, legs_str.length())), 1
    ) : nullptr;
    std::string body_str = values.at("body");
    ArmorItem * body = body_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(body_str.substr(0, body_str.find('|'))), stoi(body_str.substr(body_str.find('|') + 1, body_str.length())), 1
    ) : nullptr;
    std::string lantern_str = values.at("lantern");
    ArmorItem * lantern = lantern_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(lantern_str.substr(0, lantern_str.find('|'))), stoi(lantern_str.substr(lantern_str.find('|') + 1, lantern_str.length())), 1
    ) : nullptr;
    std::string left_ring_str = values.at("left_ring");
    ArmorItem * left_ring = left_ring_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(left_ring_str.substr(0, left_ring_str.find('|'))), stoi(left_ring_str.substr(left_ring_str.find('|') + 1, left_ring_str.length())), 1
    ) : nullptr;
    std::string right_ring_str = values.at("right_ring");
    ArmorItem * right_ring = right_ring_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(right_ring_str.substr(0, right_ring_str.find('|'))), stoi(right_ring_str.substr(right_ring_str.find('|') + 1, right_ring_str.length())), 1
    ) : nullptr;
    std::string amulet_str = values.at("amulet");
    ArmorItem * amulet = amulet_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(amulet_str.substr(0, amulet_str.find('|'))), stoi(amulet_str.substr(amulet_str.find('|') + 1, amulet_str.length())), 1
    ) : nullptr;
    std::string weapon_str = values.at("weapon");
    WeaponItem * current_weapon = weapon_str != "none" ? (WeaponItem *) Item::init(
      database->getItem(weapon_str.substr(0, weapon_str.find('|'))), stoi(weapon_str.substr(weapon_str.find('|') + 1, weapon_str.length())), 1
    ) : nullptr;
    std::list<Item *> * items = new std::list<Item *>();
    std::istringstream is_items(values.at("items"));
    std::string item;
    while(getline(is_items, item, '%')) {
      std::string item_name = item.substr(0, item.find('|'));
      item = item.substr(item.find('|') + 1, item.length());
      int tier = stoi(item.substr(0, item.find('|')));
      int number = stoi(item.substr(item.find('|') + 1, item.length()));
      items->push_back(Item::init(database->getItem(item_name), tier, number));
    }
    Gear * gear = new Gear(
      name,
      head,
      arms,
      legs,
      body,
      lantern,
      left_ring,
      right_ring,
      amulet,
      current_weapon,
      *items
    );
    database->addGear(gear);
    delete items;
  }

  void ItemOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int type = database->getTargetFromMacro(values.at("type"));
    int type2 = database->getTargetFromMacro(values.at("type2"));
    int max_tier = stoi(values.at("tier"));
    float weight = stof(values.at("weight"));
    int gold_value = stoi(values.at("gold_value"));
    std::istringstream is_droppable(values.at("droppable"));
    bool droppable;
    is_droppable >> std::boolalpha >> droppable;
    std::istringstream is_usable(values.at("usable"));
    bool usable;
    is_usable >> std::boolalpha >> usable;
    std::istringstream is_consumable(values.at("consumable"));
    bool consumable;
    is_consumable >> std::boolalpha >> consumable;
    int use_time = 0;
    if(usable) {
      use_time = stoi(values.at("use_time"));
    }
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_3(values.at("effects"));
    std::string effect;
    while(getline(is_3, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    Item * item;
    if(type == ITEM_ARMOR) {
      int armor = stoi(values.at("armor"));
      int swap_time = stoi(values.at("swap_time"));
      float damage_reductions[DAMAGE_TYPE_NUMBER] = {0.};
      damage_reductions[DAMAGE_SLASH] = stof(values.at("SLASH_REDUCTION"));
      damage_reductions[DAMAGE_PUNCTURE] = stof(values.at("PUNCTURE_REDUCTION"));
      damage_reductions[DAMAGE_IMPACT] = stof(values.at("IMPACT_REDUCTION"));
      damage_reductions[DAMAGE_FIRE] = stof(values.at("FIRE_REDUCTION"));
      damage_reductions[DAMAGE_LIGHTNING] = stof(values.at("LIGHTNING_REDUCTION"));
      damage_reductions[DAMAGE_FROST] = stof(values.at("FROST_REDUCTION"));
      damage_reductions[DAMAGE_POISON] = stof(values.at("POISON_REDUCTION"));
      damage_reductions[DAMAGE_ACID] = 0.;
      damage_reductions[DAMAGE_MIND] = stof(values.at("MIND_REDUCTION"));
      damage_reductions[DAMAGE_TRUE] = 0.;
      damage_reductions[DAMAGE_SOUL] = 0.;
      item = new ArmorItem(
        name,
        0,
        type,
        type2,
        0,
        max_tier,
        weight,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        armor,
        swap_time,
        damage_reductions
      );
    }
    if(type == ITEM_WEAPON) {
      int armor = stoi(values.at("armor"));
      int swap_time = stoi(values.at("swap_time"));
      float range = stof(values.at("range"));
      int strike_time = stoi(values.at("strike_time"));
      std::istringstream is_use_projectile(values.at("use_projectile"));
      bool use_projectile;
      is_use_projectile >> std::boolalpha >> use_projectile;
      bool use_ammo = false;
      int ammo_type = 0;
      int capacity = 0;
      int reload_time = 0;
      if(use_projectile) {
        ammo_type = database->getTargetFromMacro(values.at("ammo_type"));
        std::istringstream is_use_ammo(values.at("use_ammo"));
        is_use_ammo >> std::boolalpha >> use_ammo;
        if(use_ammo) {
          capacity = stoi(values.at("capacity"));
          reload_time = stoi(values.at("reload_time"));
        }
      }
      int damages[DAMAGE_TYPE_NUMBER] = {0};
      damages[DAMAGE_SLASH] = stoi(values.at("DAMAGE_SLASH"));
      damages[DAMAGE_PUNCTURE] = stoi(values.at("DAMAGE_PUNCTURE"));
      damages[DAMAGE_IMPACT] = stoi(values.at("DAMAGE_IMPACT"));
      damages[DAMAGE_FIRE] = stoi(values.at("DAMAGE_FIRE"));
      damages[DAMAGE_LIGHTNING] = stoi(values.at("DAMAGE_LIGHTNING"));
      damages[DAMAGE_FROST] = stoi(values.at("DAMAGE_FROST"));
      damages[DAMAGE_POISON] = stoi(values.at("DAMAGE_POISON"));
      damages[DAMAGE_ACID] = stoi(values.at("DAMAGE_ACID"));
      damages[DAMAGE_MIND] = stoi(values.at("DAMAGE_MIND"));
      damages[DAMAGE_TRUE] = stoi(values.at("DAMAGE_TRUE"));
      damages[DAMAGE_SOUL] = stoi(values.at("DAMAGE_SOUL"));
      item = new WeaponItem(
        name,
        0,
        type,
        type2,
        0,
        max_tier,
        weight,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        armor,
        swap_time,
        range,
        strike_time,
        use_projectile,
        use_ammo,
        ammo_type,
        capacity,
        reload_time,
        nullptr,
        damages
      );
    }
    if(type == ITEM_MISCELLANEOUS || type == ITEM_CONSUMABLE) {
      item = new SerialItem(
        name,
        0,
        type,
        type2,
        0,
        max_tier,
        weight,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        0
      );
    }
    if(type == ITEM_AMMUNITION) {
      Projectile * projectile = (Projectile *) database->getProjectile(values.at("projectile"));
      item = new AmmunitionItem(
        name,
        0,
        type,
        type2,
        0,
        max_tier,
        weight,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        0,
        projectile
      );
    }
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
    std::string os_fileName = std::regex_replace(fileName, std::regex("/"), PATH_DELIMITER);
    file.open(os_fileName, std::ios::in);
    if(!file) {
      std::cout << "File not found: " + fileName << std::endl;
    }
    // skip lines until we reach the map itself
    while(getline(file,line) && line != "!end");
    for(int y = sizeY - 1; y >= 0; y--) {
      getline(file,line);
      std::istringstream is(line);
      for(int x = 0; x < sizeX; x++) {
        std::string tile;
        getline(is, tile, ' ');
        map->setTile(y, x, (Tile *) database->getTile(values.at(tile)));
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
    float size = stof(values.at("size"));
    std::istringstream is(values.at("homing"));
    bool homing;
    is >> std::boolalpha >> homing;
    std::string skill_str = values.at("skill");
    Skill * skill = nullptr;
    if(skill_str != "none") {
      skill = (Skill *) database->getSkill(skill_str);
    }
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_effects(values.at("effects"));
    std::string effect;
    while(getline(is_effects, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    float speed = stof(values.at("speed"));
    float area = stof(values.at("area"));
    int falloff_timer = stoi(values.at("falloff_timer"));
    float waste_per_tick = stof(values.at("waste_per_tick"));
    float waste_per_area = stof(values.at("waste_per_area"));
    float waste_per_hit = stof(values.at("waste_per_hit"));
    int damages[DAMAGE_TYPE_NUMBER];
    damages[DAMAGE_SLASH] = stoi(values.at("DAMAGE_SLASH"));
    damages[DAMAGE_PUNCTURE] = stoi(values.at("DAMAGE_PUNCTURE"));
    damages[DAMAGE_IMPACT] = stoi(values.at("DAMAGE_IMPACT"));
    damages[DAMAGE_FIRE] = stoi(values.at("DAMAGE_FIRE"));
    damages[DAMAGE_LIGHTNING] = stoi(values.at("DAMAGE_LIGHTNING"));
    damages[DAMAGE_FROST] = stoi(values.at("DAMAGE_FROST"));
    damages[DAMAGE_POISON] = stoi(values.at("DAMAGE_POISON"));
    damages[DAMAGE_ACID] = stoi(values.at("DAMAGE_ACID"));
    damages[DAMAGE_MIND] = stoi(values.at("DAMAGE_ACID"));
    damages[DAMAGE_TRUE] = stoi(values.at("DAMAGE_TRUE"));
    damages[DAMAGE_SOUL] = stoi(values.at("DAMAGE_SOUL"));
    Projectile * projectile = new Projectile(name, projectile_type, size, homing, skill, *effects, speed, area, falloff_timer, waste_per_tick, waste_per_area, waste_per_hit, damages);
    database->addProjectile(projectile);
    delete effects;
  }

  void QuestOpener(std::string fileName, Database * database) {}

  void SettingsOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    Settings::setLightMaxPower(stoi(values.at("LIGHT_MAX_POWER")));
    Settings::setLighDuration(stoi(values.at("LIGHT_DURATION")));
    Settings::setYearDuration(stoi(values.at("YEAR_DURATION")));
    Settings::setMonthDuration(stoi(values.at("MONTH_DURATION")));
    Settings::setWeekDuration(stoi(values.at("WEEK_DURATION")));
    Settings::setHourDuration(stoi(values.at("HOUR_DURATION")));
    Settings::setMinuteDuration(stoi(values.at("MINUTE_DURATION")));
    Settings::setStartingYear(stoi(values.at("STARTING_YEAR")));
    Settings::setStartingMonth(stoi(values.at("STARTING_MONTH")));
    Settings::setStartingWeek(stoi(values.at("STARTING_WEEK")));
    Settings::setStartingDay(stoi(values.at("STARTING_DAY")));
    Settings::setStartingHour(stoi(values.at("STARING_HOUR")));
    Settings::setMaxNumberOfDaysAwake(stoi(values.at("MAX_NUMBER_DAYS_AWAKE")));
    Settings::setMaxNumberOfDaysFasting(stoi(values.at("MAX_NUMBER_DAYS_FASTING")));
    Settings::setStaminaRecoveryRatio(stoi(values.at("STAMINA_RECOVERY_RATIO")));
    Settings::setSatietyRecoveryRatio(stoi(values.at("SATIETY_RECOVERY_RATIO")));
    Settings::setStaminaOverextendRatio(stoi(values.at("STAMINA_OVEREXTEND_RATIO")));
    Settings::setSatietyOverextendRatio(stoi(values.at("SATIETY_OVEREXTEND_RATIO")));
    Settings::setBuyingPriceModifier(stof(values.at("BUYING_PRICE_MODIFIER")));
    Settings::setPort(stoi(values.at("PORT")));
    std::string seed = values.at("SEED");
    seed == "rand" ? Settings::setSeed(time(0)) : Settings::setSeed(stoi(seed));
  }

  void ClientSettingsOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    ClientSettings::setPort(stoi(values.at("PORT")));
    ClientSettings::setLang(values.at("LANG"));
    ClientSettings::setResolution(values.at("RESOLUTION"));
    ClientSettings::setWindowType(database->getTargetFromMacro(values.at("WINDOW_TYPE")));
  }

  void SkillOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int level = stoi(values.at("level"));
    std::string attributes = values.at("attributes");
    int target_type = database->getTargetFromMacro(values.at("target_type"));
    std::istringstream is(values.at("is_instant"));
    bool is_INSTANT_DURATION;
    is >> std::boolalpha >> is_INSTANT_DURATION;
    int overcharge_power_type = database->getTargetFromMacro(values.at("overcharge_power_type"));
    int overcharge_duration_type = database->getTargetFromMacro(values.at("overcharge_duration_type"));
    int overcharge_range_type = database->getTargetFromMacro(values.at("overcharge_range_type"));
    int range = stoi(values.at("range"));
    int time = stoi(values.at("time"));
    std::list<PseudoSkill *> * skills = new std::list<PseudoSkill *>();
    std::istringstream is_skills(values.at("skills"));
    std::string pseudoSkill;
    while(getline(is_skills, pseudoSkill, '%')) {
      skills->push_back((PseudoSkill *) database->getPseudoSkill(pseudoSkill));
    }
    Skill * skill = new Skill(name, level, attributes, target_type, is_INSTANT_DURATION, overcharge_power_type, overcharge_duration_type, overcharge_range_type, range, time, *skills);
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
      case SKILL_SIMPLE:
        pseudoSkill = new SimpleSkill(name, skill_type, target_type, mana_cost, *effects);
        break;
      case SKILL_PROJECTILE: {
        Projectile * projectile = (Projectile *) database->getProjectile(values.at("projectile"));
        pseudoSkill = new ProjectileSkill(name, skill_type, target_type, mana_cost, *effects, projectile);
        break;
      }
      case SKILL_TELEPORT: {
        int apparition_type = database->getTargetFromMacro(values.at("apparition_type"));
        int movement_type = database->getTargetFromMacro(values.at("movement_type"));
        pseudoSkill = new TeleportSkill(name, skill_type, target_type, mana_cost, *effects, apparition_type, movement_type);
        break;
      }
      case SKILL_TILE_SWAP: {
        Tile * current_tile = (Tile *) database->getTile(values.at("current_tile"));
        Tile * new_tile = (Tile *) database->getTile(values.at("new_tile"));
        pseudoSkill = new TileSwapSkill(name, skill_type, target_type, mana_cost, *effects, current_tile, new_tile);
        break;
      }
      case SKILL_SUMMON: {
        Character * character = (Character *) database->getCharacter(values.at("character"));
        std::string ai_str = values.at("ai");
        std::string team = values.at("team");
        if(team == "same") {
          team = "";
        }
        Race * race = (Race *) database->getWay(values.at("race"));
        std::list<Race *> * race_modifiers = new std::list<Race *>();
        std::istringstream is_race_modifiers(values.at("race_modifiers"));
        std::string modifier;
        while(getline(is_race_modifiers, modifier, '%') && modifier != "none") {
          race_modifiers->push_back((Race *) database->getWay(modifier));
        }
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
        pseudoSkill = new SummonSkill(
          name,
          skill_type,
          target_type,
          mana_cost,
          *effects,
          character,
          ai_str,
          team,
          race,
          *race_modifiers,
          origin,
          culture,
          religion,
          profession,
          attributes,
          *titles,
          apparition_type,
          xp
        );
        delete race_modifiers;
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

  std::string TileOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    std::istringstream is_untraversable(values.at("untraversable"));
    bool untraversable;
    is_untraversable >> std::boolalpha >> untraversable;
    std::istringstream is_opaque(values.at("opaque"));
    bool opaque;
    is_opaque >> std::boolalpha >> opaque;
    std::istringstream is_solid(values.at("solid"));
    bool solid;
    is_solid >> std::boolalpha >> solid;
    int light = stoi(values.at("light"));
    float ap_cost = stof(values.at("ap_cost"));
    Tile * tile = new Tile(name, untraversable, opaque, solid, light, ap_cost);
    database->addTile(tile);
    return name;
  }

  void WayOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int type = database->getTargetFromMacro(values.at("type"));
    int baseHp = stoi(values.at("baseHp"));
    int baseMana = stoi(values.at("baseMana"));
    int baseArmorMult = stoi(values.at("baseArmorMult"));
    int baseDamageMult = stoi(values.at("baseDamageMult"));
    int baseSoulBurn = stoi(values.at("baseSoulBurn"));
    int baseFlow = stoi(values.at("baseFlow"));
    int baseVisionRange = stoi(values.at("baseVisionRange"));
    int baseVisionPower = stoi(values.at("baseVisionPower"));
    int baseDetectionRange = stoi(values.at("baseDetectionRange"));
    int hpIncr = stoi(values.at("hpIncr"));
    int manaIncr = stoi(values.at("manaIncr"));
    int armorMultIncr = stoi(values.at("armorMultIncr"));
    int damageMultIncr = stoi(values.at("damageMultIncr"));
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
    if(type == WAY_RACE) {
      int race_type = database->getTargetFromMacro(values.at("race_type"));
      int baseArmor = stoi(values.at("baseArmor"));
      float size = stof(values.at("size"));
      std::istringstream is_need_to_eat(values.at("need_to_eat"));
      bool need_to_eat;
      is_need_to_eat >> std::boolalpha >> need_to_eat;
      std::istringstream is_can_eat_food(values.at("can_eat_food"));
      bool can_eat_food;
      is_can_eat_food >> std::boolalpha >> can_eat_food;
      std::istringstream is_need_to_sleep(values.at("need_to_sleep"));
      bool need_to_sleep;
      is_need_to_sleep >> std::boolalpha >> need_to_sleep;
      std::istringstream is_has_soulspark(values.at("has_soulspark"));
      bool has_soulspark;
      is_has_soulspark >> std::boolalpha >> has_soulspark;
      float action_time_modifier = stof(values.at("action_time_modifier"));
      float strike_time_modifier = stof(values.at("strike_time_modifier"));
      float skill_time_modifier = stof(values.at("skill_time_modifier"));
      float movement_time_modifier = stof(values.at("movement_time_modifier"));
      std::list<Item *> * loots = new std::list<Item *>();
      std::istringstream is_loot(values.at("loot"));
      std::string loot;
      while(getline(is_loot, loot, '%')) {
        loots->push_back((Item *) database->getItem(loot));
      }
      Way * way = new Race(
        name,
        type,
        baseHp,
        baseMana,
        baseArmorMult,
        baseDamageMult,
        baseSoulBurn,
        baseFlow,
        baseVisionRange,
        baseVisionPower,
        baseDetectionRange,
        hpIncr,
        manaIncr,
        armorMultIncr,
        damageMultIncr,
        soulBurnIncr,
        flowIncr,
        *effects,
        *skills,
        race_type,
        size,
        baseArmor,
        need_to_eat,
        can_eat_food,
        need_to_sleep,
        has_soulspark,
        action_time_modifier,
        strike_time_modifier,
        skill_time_modifier,
        movement_time_modifier,
        *loots
      );
      database->addWay(way);
      delete loots;
      delete effects;
      delete skills;
    }
    else {
      Way * way = new Way(
        name,
        type,
        baseHp,
        baseMana,
        baseArmorMult,
        baseDamageMult,
        baseSoulBurn,
        baseFlow,
        baseVisionRange,
        baseVisionPower,
        baseDetectionRange,
        hpIncr,
        manaIncr,
        armorMultIncr,
        damageMultIncr,
        soulBurnIncr,
        flowIncr,
        *effects,
        *skills
      );
      database->addWay(way);
      delete effects;
      delete skills;
    }
  }

  void FileOpener(std::string fileName, Database * database, bool isServer) {
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
    else if(last_folder == "gears") {
      GearOpener(fileName, database);
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
      std::string tile_name = TileOpener(fileName, database);
      if(!isServer) {
        database->addTileRes(tile_name, std::regex_replace(fileName, std::regex(".data"), ".tres"));
      }
    }
    else if((last_folder == "ways") ||
            (last_folder == "races") ||
            (last_folder == "origins") ||
            (last_folder == "cultures") ||
            (last_folder == "religions") ||
            (last_folder == "professions") ||
            (last_folder == "titles")) {
      WayOpener(fileName, database);
    }
  }

  Database * DatabaseOpener(std::string fileName, bool isServer) {
    Database * database = new Database();
    std::fstream file;
    std::string os_fileName = std::regex_replace(fileName, std::regex("/"), PATH_DELIMITER);
    file.open(os_fileName, std::ios::in);
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
        FileOpener(line, database, isServer);
      }
    }
    file.close();
    return database;
  }
}
