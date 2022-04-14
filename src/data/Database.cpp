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

#include "data/Database.h"

Database::Database() {
  macros = std::map<const std::string,const int>();

  attributes = std::map<const std::string,const Attributes * >();
  characters = std::map<const std::string,const Character * >();
  effects = std::map<const std::string,const Effect * >();
  events = std::map<const std::string,const Event * >();
  items = std::map<const std::string,const Item * >();
  maps = std::map<const std::string,const Map * >();
  projectiles = std::map<const std::string,const Projectile * >();
  ammunitions = std::map<const std::string,const Ammunition * >();
  quests = std::map<const std::string,const Quest * >();
  skills = std::map<const std::string,const Skill * >();
  speechs = std::map<const std::string,const Speech * >();
  tiles = std::map<const std::string,const Tile * >();
  ways = std::map<const std::string,const Way * >();
  weapons = std::map<const std::string,const Weapon * >();

  macros.insert(std::make_pair("INSTANT",INSTANT));
  macros.insert(std::make_pair("INFINITE",INFINITE));
  macros.insert(std::make_pair("TEMPORARY",TEMPORARY));

  // effect_type

  macros.insert(std::make_pair("HP",HP));
  macros.insert(std::make_pair("MANA",MANA));
  macros.insert(std::make_pair("HP_MAX",HP_MAX));
  macros.insert(std::make_pair("MANA_MAX",MANA_MAX));
  macros.insert(std::make_pair("ARMOR",ARMOR));
  macros.insert(std::make_pair("SOULBURNTRESHOLD",SOULBURNTRESHOLD));
  macros.insert(std::make_pair("FLOW",FLOW));
  macros.insert(std::make_pair("VISION_RANGE",VISION_RANGE));
  macros.insert(std::make_pair("VISION_POWER",VISION_POWER));
  macros.insert(std::make_pair("DETECTION_RANGE",DETECTION_RANGE));
  macros.insert(std::make_pair("DAMAGE",DAMAGE));
  macros.insert(std::make_pair("DAMAGE_BUFF",DAMAGE_BUFF));
  macros.insert(std::make_pair("DAMAGE_REDUCTION",DAMAGE_REDUCTION));
  macros.insert(std::make_pair("CHANNELING",CHANNELING));
  macros.insert(std::make_pair("STUNNED",STUNNED));
  macros.insert(std::make_pair("SLEEPING",SLEEPING));
  macros.insert(std::make_pair("CLOAKED",CLOAKED));
  macros.insert(std::make_pair("INVISIBLE",INVISIBLE));

  // melee_type

  macros.insert(std::make_pair("LONG_SWORD",LONG_SWORD));
  macros.insert(std::make_pair("SHORT_SWORD",SHORT_SWORD));
  macros.insert(std::make_pair("CURVED_SWORD",CURVED_SWORD));
  macros.insert(std::make_pair("RAPIER",RAPIER));
  macros.insert(std::make_pair("SPEAR",SPEAR));
  macros.insert(std::make_pair("GLAIVE",GLAIVE));
  macros.insert(std::make_pair("AXE",AXE));
  macros.insert(std::make_pair("WARHAMMER",WARHAMMER));
  macros.insert(std::make_pair("DAGGER",DAGGER));

  // melee_range

  macros.insert(std::make_pair("SHORT",SHORT));
  macros.insert(std::make_pair("STANDARD",STANDARD));
  macros.insert(std::make_pair("LONG",LONG));

  // range_type

  macros.insert(std::make_pair("BOW",BOW));
  macros.insert(std::make_pair("CROSSBOW",CROSSBOW));
  macros.insert(std::make_pair("MAGIC_STAFF",MAGIC_STAFF));
  macros.insert(std::make_pair("RIFLE",RIFLE));
  macros.insert(std::make_pair("PISTOL",PISTOL));
  macros.insert(std::make_pair("SLING",SLING));

  // weight

  macros.insert(std::make_pair("LIGHT",LIGHT));
  macros.insert(std::make_pair("MEDIUM",MEDIUM));
  macros.insert(std::make_pair("HEAVY",HEAVY));

  // item_type

  macros.insert(std::make_pair("UNEQUIPABLE",UNEQUIPABLE));
  macros.insert(std::make_pair("HEAD",HEAD));
  macros.insert(std::make_pair("ARMS",ARMS));
  macros.insert(std::make_pair("BODY",BODY));
  macros.insert(std::make_pair("LEGS",LEGS));
  macros.insert(std::make_pair("FULL_BODY",FULL_BODY));
  macros.insert(std::make_pair("RING",RING));
  macros.insert(std::make_pair("AMULET",AMULET));

  macros.insert(std::make_pair("LEFT_RING",LEFT_RING));
  macros.insert(std::make_pair("RIGHT_RING",RIGHT_RING));

  // event_type

  macros.insert(std::make_pair("CALAMITY",CALAMITY));
  macros.insert(std::make_pair("BLESSING",BLESSING));

  // character_apparition_type

  macros.insert(std::make_pair("SOFT",SOFT));
  macros.insert(std::make_pair("HARD",HARD));

  // skill_target

  macros.insert(std::make_pair("SELF",SELF));
  macros.insert(std::make_pair("TOUCH",TOUCH));
  macros.insert(std::make_pair("TARGET",TARGET));
  macros.insert(std::make_pair("TILE",TILE));

  // skill_special

  macros.insert(std::make_pair("TIMESTOP",TIMESTOP));
  macros.insert(std::make_pair("TILESWAP",TILESWAP));
  macros.insert(std::make_pair("TELEPORT",TELEPORT));
  macros.insert(std::make_pair("MAPLINKER",MAPLINKER));
  macros.insert(std::make_pair("TEAMCHANGER",TEAMCHANGER));
  macros.insert(std::make_pair("MINDCONTROL",MINDCONTROL));
  macros.insert(std::make_pair("PROJECTILE",PROJECTILE));
  macros.insert(std::make_pair("SUMMON",SUMMON));
  macros.insert(std::make_pair("RESURRECTION",RESURRECTION));

  // skill_overcharge_type

  macros.insert(std::make_pair("NO_OVERCHARGE",NO_OVERCHARGE));
  macros.insert(std::make_pair("LINEAR",LINEAR));
  macros.insert(std::make_pair("SQUARE",SQUARE));
  macros.insert(std::make_pair("EXPONENTIAL",EXPONENTIAL));

  // orientations

  macros.insert(std::make_pair("NORTH",NORTH));
  macros.insert(std::make_pair("NORTH_EAST",NORTH_EAST));
  macros.insert(std::make_pair("EAST",EAST));
  macros.insert(std::make_pair("SOUTH_EAST",SOUTH_EAST));
  macros.insert(std::make_pair("SOUTH",SOUTH));
  macros.insert(std::make_pair("SOUTH_WEST",SOUTH_WEST));
  macros.insert(std::make_pair("WEST",WEST));
  macros.insert(std::make_pair("NORTH_WEST",NORTH_WEST));
  macros.insert(std::make_pair("NO_ORIENTATION",NO_ORIENTATION));

  // projectile_type

  macros.insert(std::make_pair("FIRE_FORM",FIRE_FORM));
  macros.insert(std::make_pair("THUNDER_FORM",THUNDER_FORM));
  macros.insert(std::make_pair("AIR",AIR));
  macros.insert(std::make_pair("ARROW",ARROW));
  macros.insert(std::make_pair("BULLET",BULLET));
  macros.insert(std::make_pair("SPECIAL",SPECIAL));

  // projectile_target_type

  macros.insert(std::make_pair("SINGLE_CHARACTER",SINGLE_CHARACTER));
  macros.insert(std::make_pair("SINGLE_TILE",SINGLE_TILE));
  macros.insert(std::make_pair("MULTIPLE_CHARACTERS",MULTIPLE_CHARACTERS));
  macros.insert(std::make_pair("MULTIPLE_TILES",MULTIPLE_TILES));

  // character_type

  macros.insert(std::make_pair("HUMAN",HUMAN));
  macros.insert(std::make_pair("UNDEAD",UNDEAD));
  macros.insert(std::make_pair("NATURE",NATURE));
  macros.insert(std::make_pair("OMONCULUS",OMONCULUS));
  macros.insert(std::make_pair("WALL",WALL));

  // damage_type

  macros.insert(std::make_pair("DAMAGE_TYPE_NUMBER",DAMAGE_TYPE_NUMBER));

  macros.insert(std::make_pair("SLASH",SLASH));
  macros.insert(std::make_pair("PUNCTURE",PUNCTURE));
  macros.insert(std::make_pair("IMPACT",IMPACT));
  macros.insert(std::make_pair("FIRE",FIRE));
  macros.insert(std::make_pair("THUNDER",THUNDER));
  macros.insert(std::make_pair("COLD",COLD));
  macros.insert(std::make_pair("POISON",POISON));
  macros.insert(std::make_pair("NEUTRAL",NEUTRAL));
  macros.insert(std::make_pair("TRUE",TRUE));
  macros.insert(std::make_pair("SOUL",SOUL));

  // loot_type

  macros.insert(std::make_pair("CORPSE",CORPSE));
  macros.insert(std::make_pair("TREASURE",TREASURE));

  // quest_step_type

  macros.insert(std::make_pair("SLAY",SLAY));
  macros.insert(std::make_pair("OBTAIN_ITEM",OBTAIN_ITEM));
  macros.insert(std::make_pair("OBTAIN_WEAPON",OBTAIN_WEAPON));
  macros.insert(std::make_pair("DISCOVER",DISCOVER));
  macros.insert(std::make_pair("TALK",TALK));

  // way_type

  macros.insert(std::make_pair("RACE",RACE));
  macros.insert(std::make_pair("ORIGIN",ORIGIN));
  macros.insert(std::make_pair("CULTURE",CULTURE));
  macros.insert(std::make_pair("RELIGION",RELIGION));
  macros.insert(std::make_pair("PROFESSION",PROFESSION));

  // speech_type

  macros.insert(std::make_pair("TEXT",TEXT));
  macros.insert(std::make_pair("SPEACH",SPEACH));
  macros.insert(std::make_pair("WARNING",WARNING));
  macros.insert(std::make_pair("ERROR",ERROR));
}

const int Database::getTargetFromMacro(const std::string macro) { return macros.find(macro) != macros.end() ? macros.at(macro) : stoi(macro); }

const Attributes * Database::getAttributes(const std::string attributes) { return this->attributes.at(attributes); }
const Character * Database::getCharacter(const std::string character) { return characters.at(character); }
const Effect * Database::getEffect(const std::string effect) { return effects.at(effect); }
const Event * Database::getEvent(const std::string event) { return events.at(event); }
const Item * Database::getItem(const std::string item) { return items.at(item); }
const Map * Database::getMap(const std::string map) { return maps.at(map); }
const Projectile * Database::getProjectile(const std::string projectile) { return projectiles.at(projectile); }
const Ammunition * Database::getAmmunition(const std::string ammunition) { return ammunitions.at(ammunition); }
const Quest * Database::getQuest(const std::string quest) { return quests.at(quest); }
const Skill * Database::getSkill(const std::string skill) { return skills.at(skill); }
const Speech * Database::getSpeech(const std::string speech) { return speechs.at(speech); }
const Tile * Database::getTile(const std::string tile) { return tiles.at(tile); }
const Way * Database::getWay(const std::string way) { return ways.at(way); }
const Weapon * Database::getWeapon(const std::string weapon) { return weapons.at(weapon); }

void Database::addAttributes(const Attributes * attributes) { this->attributes.insert(std::make_pair(attributes->name, attributes)); }
void Database::addCharacter(const Character * character) { characters.insert(std::make_pair(character->name, character)); }
void Database::addEffect(const Effect * effect) { effects.insert(std::make_pair(effect->name, effect)); }
void Database::addEvent(const Event * event) { events.insert(std::make_pair(event->name, event)); }
void Database::addItem(const Item * item) { items.insert(std::make_pair(item->name, item)); }
void Database::addMap(const Map * map) { maps.insert(std::make_pair(map->name, map)); }
void Database::addProjectile(const Projectile * projectile) { projectiles.insert(std::make_pair(projectile->name, projectile)); }
void Database::addAmmunition(const Ammunition * ammunition) { ammunitions.insert(std::make_pair(ammunition->projectile->name, ammunition)); }
void Database::addQuest(const Quest * quest) { quests.insert(std::make_pair(quest->name, quest)); }
void Database::addSkill(const Skill * skill) { skills.insert(std::make_pair(skill->name, skill)); }
void Database::addSpeech(const Speech * speech) { speechs.insert(std::make_pair(speech->name, speech)); }
void Database::addTile(const Tile * tile) { tiles.insert(std::make_pair(tile->name, tile)); }
void Database::addWay(const Way * way) { ways.insert(std::make_pair(way->name, way)); }
void Database::addWeapon(const Weapon * weapon) { weapons.insert(std::make_pair(weapon->name, weapon)); }
