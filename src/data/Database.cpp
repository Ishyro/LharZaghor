#include <stdexcept>

#include "data/Character.h"
#include "data/Effect.h"
#include "data/Event.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Quest.h"
#include "data/skills/Skill.h"
#include "data/Speech.h"
#include "data/Block.h"


#include "data/items/Item.h"
#include "data/items/Gear.h"
#include "data/furnitures/Furniture.h"

#include "data/ways/Way.h"
#include "data/ways/Attributes.h"

#include "data/Database.h"

Database::Database() {
  macros = std::map<const std::string,const int32_t>();

  attributes = std::map<const std::string, const Attributes *>();
  blocks = std::map<const std::string, const Block *>();
  characters = std::map<const std::string, const Character *>();
  effects = std::map<const std::string, const Effect *>();
  events = std::map<const std::string, const Event *>();
  items = std::map<const std::string, const Item *>();
  itemsFiles = std::map<const std::string, const std::string>();
  furnitures = std::map<const std::string, const Furniture * >();
  furnituresFiles = std::map<const std::string, const std::string>();
  gears = std::map<const std::string, const Gear *>();
  maps = std::map<const std::string, const Map *>();
  projectiles = std::map<const std::string, const Projectile *>();
  quests = std::map<const std::string, const Quest * >();
  skills = std::map<const std::string, const Skill * >();
  pseudoSkills = std::map<const std::string, const PseudoSkill *>();
  speechs = std::map<const std::string, const Speech * >();
  ways = std::map<const std::string, const Way *>();
  waysFiles = std::map<const std::string, const std::string>();
  relations = std::map<const std::string, std::map<const std::string, int32_t>>();
  waysIncompatibilities = std::list<std::pair<const std::string, const std::string>>();
  paths = std::list<std::string>();

  // block_type
  macros.insert(std::make_pair("BLOCK_SOLID", BLOCK_SOLID));
  macros.insert(std::make_pair("BLOCK_LIQUID", BLOCK_LIQUID));
  macros.insert(std::make_pair("BLOCK_GAS", BLOCK_GAS));
  macros.insert(std::make_pair("BLOCK_STAIRS", BLOCK_STAIRS));
  macros.insert(std::make_pair("BLOCK_SLOPE", BLOCK_SLOPE));

  // lightening_type
  macros.insert(std::make_pair("LIGHTENING_NO", LIGHTENING_NO));
  macros.insert(std::make_pair("LIGHTENING_INDOORS", LIGHTENING_INDOORS));
  macros.insert(std::make_pair("LIGHTENING_OUTDOORS", LIGHTENING_OUTDOORS));
  macros.insert(std::make_pair("LIGHTENING_DARK", LIGHTENING_DARK));

  // effect_duration_type
  macros.insert(std::make_pair("DURATION_INSTANT", DURATION_INSTANT));
  macros.insert(std::make_pair("DURATION_INFINITE", DURATION_INFINITE));
  macros.insert(std::make_pair("DURATION_TEMPORARY", DURATION_TEMPORARY));

  // effect_type
  macros.insert(std::make_pair("EFFECT_HP", EFFECT_HP));
  macros.insert(std::make_pair("EFFECT_MANA", EFFECT_MANA));
  macros.insert(std::make_pair("EFFECT_HP_MAX", EFFECT_HP_MAX));
  macros.insert(std::make_pair("EFFECT_MANA_MAX", EFFECT_MANA_MAX));
  macros.insert(std::make_pair("EFFECT_HUNGER", EFFECT_HUNGER));
  macros.insert(std::make_pair("EFFECT_THIRST", EFFECT_THIRST));
  macros.insert(std::make_pair("EFFECT_STAMINA", EFFECT_STAMINA));
  macros.insert(std::make_pair("EFFECT_SANITY", EFFECT_SANITY));
  macros.insert(std::make_pair("EFFECT_SHIELD", EFFECT_SHIELD));
  macros.insert(std::make_pair("EFFECT_SHIELD_MULTIPLIER", EFFECT_SHIELD_MULTIPLIER));
  macros.insert(std::make_pair("EFFECT_SOULBURNTRESHOLD", EFFECT_SOULBURNTRESHOLD));
  macros.insert(std::make_pair("EFFECT_FLOW", EFFECT_FLOW));
  macros.insert(std::make_pair("EFFECT_VISION_RANGE", EFFECT_VISION_RANGE));
  macros.insert(std::make_pair("EFFECT_VISION_POWER", EFFECT_VISION_POWER));
  macros.insert(std::make_pair("EFFECT_DETECTION_RANGE", EFFECT_DETECTION_RANGE));
  macros.insert(std::make_pair("EFFECT_EXPERIENCE", EFFECT_EXPERIENCE));
  macros.insert(std::make_pair("EFFECT_DAMAGE", EFFECT_DAMAGE));
  macros.insert(std::make_pair("EFFECT_DAMAGE_BUFF", EFFECT_DAMAGE_BUFF));
  macros.insert(std::make_pair("EFFECT_DAMAGE_REDUCTION", EFFECT_DAMAGE_REDUCTION));
  macros.insert(std::make_pair("EFFECT_DAMAGE_MULTIPLIER", EFFECT_DAMAGE_MULTIPLIER));
  macros.insert(std::make_pair("EFFECT_STATUS_POWER", EFFECT_STATUS_POWER));
  macros.insert(std::make_pair("EFFECT_STATUS_REDUCTION", EFFECT_STATUS_REDUCTION));
  macros.insert(std::make_pair("EFFECT_CHANNELING", EFFECT_CHANNELING));
  macros.insert(std::make_pair("EFFECT_STUNNED", EFFECT_STUNNED));
  macros.insert(std::make_pair("EFFECT_SLEEPING", EFFECT_SLEEPING));
  macros.insert(std::make_pair("EFFECT_FLY", EFFECT_FLY));
  macros.insert(std::make_pair("EFFECT_LIGHT", EFFECT_LIGHT));
  macros.insert(std::make_pair("EFFECT_CLOAKED", EFFECT_CLOAKED));
  macros.insert(std::make_pair("EFFECT_INVISIBLE", EFFECT_INVISIBLE));
  macros.insert(std::make_pair("EFFECT_ETHERAL", EFFECT_ETHERAL));
  macros.insert(std::make_pair("EFFECT_INVULNERABLE", EFFECT_INVULNERABLE));
  macros.insert(std::make_pair("EFFECT_ACTION_TIME_MODIFIER", EFFECT_ACTION_TIME_MODIFIER));
  macros.insert(std::make_pair("EFFECT_HAND_ACTION_TIME_MODIFIER", EFFECT_HAND_ACTION_TIME_MODIFIER));
  macros.insert(std::make_pair("EFFECT_SKILL_TIME_MODIFIER", EFFECT_SKILL_TIME_MODIFIER));
  macros.insert(std::make_pair("EFFECT_MOVEMENT_TIME_MODIFIER", EFFECT_MOVEMENT_TIME_MODIFIER));

  // status_type
  macros.insert(std::make_pair("EFFECT_STATUS_BLEEDING", EFFECT_STATUS_BLEEDING));
  macros.insert(std::make_pair("EFFECT_STATUS_WEAKENED", EFFECT_STATUS_WEAKENED));
  macros.insert(std::make_pair("EFFECT_STATUS_CONFUSED", EFFECT_STATUS_CONFUSED));
  macros.insert(std::make_pair("EFFECT_STATUS_BURNING", EFFECT_STATUS_BURNING));
  macros.insert(std::make_pair("EFFECT_STATUS_FROZEN", EFFECT_STATUS_FROZEN));
  macros.insert(std::make_pair("EFFECT_STATUS_SHOCKED", EFFECT_STATUS_SHOCKED));
  macros.insert(std::make_pair("EFFECT_STATUS_POISONED", EFFECT_STATUS_POISONED));
  macros.insert(std::make_pair("EFFECT_STATUS_CORRODED", EFFECT_STATUS_CORRODED));
  macros.insert(std::make_pair("EFFECT_STATUS_BROKEN", EFFECT_STATUS_BROKEN));
  macros.insert(std::make_pair("EFFECT_STATUS_DISINTEGRATED", EFFECT_STATUS_DISINTEGRATED));

  macros.insert(std::make_pair("EFFECT_STATUS_IMMUNITY_BLEEDING", EFFECT_STATUS_IMMUNITY_BLEEDING));
  macros.insert(std::make_pair("EFFECT_STATUS_IMMUNITY_WEAKENED", EFFECT_STATUS_IMMUNITY_WEAKENED));
  macros.insert(std::make_pair("EFFECT_STATUS_IMMUNITY_CONFUSED", EFFECT_STATUS_IMMUNITY_CONFUSED));
  macros.insert(std::make_pair("EFFECT_STATUS_IMMUNITY_BURNING", EFFECT_STATUS_IMMUNITY_BURNING));
  macros.insert(std::make_pair("EFFECT_STATUS_IMMUNITY_FROZEN", EFFECT_STATUS_IMMUNITY_FROZEN));
  macros.insert(std::make_pair("EFFECT_STATUS_IMMUNITY_SHOCKED", EFFECT_STATUS_IMMUNITY_SHOCKED));
  macros.insert(std::make_pair("EFFECT_STATUS_IMMUNITY_POISONED", EFFECT_STATUS_IMMUNITY_POISONED));
  macros.insert(std::make_pair("EFFECT_STATUS_IMMUNITY_CORRODED", EFFECT_STATUS_IMMUNITY_CORRODED));
  macros.insert(std::make_pair("EFFECT_STATUS_IMMUNITY_BROKEN", EFFECT_STATUS_IMMUNITY_BROKEN));
  macros.insert(std::make_pair("EFFECT_STATUS_IMMUNITY_DISINTEGRATED", EFFECT_STATUS_IMMUNITY_DISINTEGRATED));

  // item_type
  macros.insert(std::make_pair("ITEM_WEAPON", ITEM_WEAPON));
  macros.insert(std::make_pair("ITEM_ARMOR", ITEM_ARMOR));
  macros.insert(std::make_pair("ITEM_BASIC", ITEM_BASIC));
  macros.insert(std::make_pair("ITEM_SERIAL", ITEM_SERIAL));
  macros.insert(std::make_pair("ITEM_AMMUNITION", ITEM_AMMUNITION));
  macros.insert(std::make_pair("ITEM_CONTAINER", ITEM_CONTAINER));

  // weapon_type
  macros.insert(std::make_pair("ITEM_LONG_SWORD", ITEM_LONG_SWORD));
  macros.insert(std::make_pair("ITEM_SHORT_SWORD", ITEM_SHORT_SWORD));
  macros.insert(std::make_pair("ITEM_CURVED_SWORD", ITEM_CURVED_SWORD));
  macros.insert(std::make_pair("ITEM_RAPIER", ITEM_RAPIER));
  macros.insert(std::make_pair("ITEM_SPEAR", ITEM_SPEAR));
  macros.insert(std::make_pair("ITEM_GLAIVE", ITEM_GLAIVE));
  macros.insert(std::make_pair("ITEM_AXE", ITEM_AXE));
  macros.insert(std::make_pair("ITEM_WARHAMMER", ITEM_WARHAMMER));
  macros.insert(std::make_pair("ITEM_DAGGER", ITEM_DAGGER));
  macros.insert(std::make_pair("ITEM_FIST", ITEM_FIST));
  macros.insert(std::make_pair("ITEM_BOW", ITEM_BOW));
  macros.insert(std::make_pair("ITEM_CROSSBOW", ITEM_CROSSBOW));
  macros.insert(std::make_pair("ITEM_MAGIC_STAFF", ITEM_MAGIC_STAFF));
  macros.insert(std::make_pair("ITEM_RIFLE", ITEM_RIFLE));
  macros.insert(std::make_pair("ITEM_PISTOL", ITEM_PISTOL));
  macros.insert(std::make_pair("ITEM_SLING", ITEM_SLING));

  // armor_type
  macros.insert(std::make_pair("ITEM_MANTLE", ITEM_MANTLE));
  macros.insert(std::make_pair("ITEM_HELMET", ITEM_HELMET));
  macros.insert(std::make_pair("ITEM_GAUNTLETS", ITEM_GAUNTLETS));
  macros.insert(std::make_pair("ITEM_CUIRASS", ITEM_CUIRASS));
  macros.insert(std::make_pair("ITEM_BOOTS", ITEM_BOOTS));
  macros.insert(std::make_pair("ITEM_LANTERN", ITEM_LANTERN));
  macros.insert(std::make_pair("ITEM_AMULET", ITEM_AMULET));
  macros.insert(std::make_pair("ITEM_RING", ITEM_RING));

  // other_items_type
  macros.insert(std::make_pair("ITEM_FOOD", ITEM_FOOD));
  macros.insert(std::make_pair("ITEM_MATERIAL", ITEM_MATERIAL));
  macros.insert(std::make_pair("ITEM_POTION", ITEM_POTION));
  macros.insert(std::make_pair("ITEM_BAG", ITEM_BAG));
  macros.insert(std::make_pair("ITEM_BELT", ITEM_BELT));
  macros.insert(std::make_pair("ITEM_POUCH", ITEM_POUCH));
  macros.insert(std::make_pair("ITEM_MAGIC_BOX", ITEM_MAGIC_BOX));

  // furniture_type
  macros.insert(std::make_pair("FURNITURE_BASIC", FURNITURE_BASIC)); 
  macros.insert(std::make_pair("FURNITURE_CONTAINER", FURNITURE_CONTAINER)); 
  macros.insert(std::make_pair("FURNITURE_CRAFTING", FURNITURE_CRAFTING)); 
  macros.insert(std::make_pair("FURNITURE_LINKED", FURNITURE_LINKED)); 
  macros.insert(std::make_pair("FURNITURE_SWITCH", FURNITURE_SWITCH)); 
  macros.insert(std::make_pair("FURNITURE_SKILL", FURNITURE_SKILL)); 

  // event_type
  macros.insert(std::make_pair("EVENT_CALAMITY", EVENT_CALAMITY));
  macros.insert(std::make_pair("EVENT_BLESSING", EVENT_BLESSING));

  // character_apparition_type
  macros.insert(std::make_pair("APPARITION_SOFT", APPARITION_SOFT));
  macros.insert(std::make_pair("APPARITION_HARD", APPARITION_HARD));

  // movement_type
  macros.insert(std::make_pair("MOVEMENT_FLYING", MOVEMENT_FLYING));
  macros.insert(std::make_pair("MOVEMENT_WALKING", MOVEMENT_WALKING));
  macros.insert(std::make_pair("MOVEMENT_TELEPORT", MOVEMENT_TELEPORT));

  // target_type
  macros.insert(std::make_pair("TARGET_NONE", TARGET_NONE));
  macros.insert(std::make_pair("TARGET_SELF", TARGET_SELF));
  macros.insert(std::make_pair("TARGET_CHARACTER", TARGET_CHARACTER));
  macros.insert(std::make_pair("TARGET_COORDINATES", TARGET_COORDINATES));
  macros.insert(std::make_pair("TARGET_BLOCK", TARGET_BLOCK));
  macros.insert(std::make_pair("TARGET_PERMISSIVE", TARGET_PERMISSIVE));
  macros.insert(std::make_pair("TARGET_MULTIPLE", TARGET_MULTIPLE));

  // skill_type
  macros.insert(std::make_pair("SKILL_CHANNELED", SKILL_CHANNELED));
  macros.insert(std::make_pair("SKILL_MAP_LINKER", SKILL_MAP_LINKER));
  macros.insert(std::make_pair("SKILL_MIND_CONTROL", SKILL_MIND_CONTROL));
  macros.insert(std::make_pair("SKILL_PROJECTILE", SKILL_PROJECTILE));
  macros.insert(std::make_pair("SKILL_RESURRECTION", SKILL_RESURRECTION));
  macros.insert(std::make_pair("SKILL_SIMPLE", SKILL_SIMPLE));
  macros.insert(std::make_pair("SKILL_SUMMON", SKILL_SUMMON));
  macros.insert(std::make_pair("SKILL_TEAM_CHANGER", SKILL_TEAM_CHANGER));
  macros.insert(std::make_pair("SKILL_TELEPORT", SKILL_TELEPORT));
  macros.insert(std::make_pair("SKILL_TILE_SWAP", SKILL_TILE_SWAP));

  // skill_overcharge_type
  macros.insert(std::make_pair("OVERCHARGE_NONE", OVERCHARGE_NONE));
  macros.insert(std::make_pair("OVERCHARGE_LINEAR", OVERCHARGE_LINEAR));
  macros.insert(std::make_pair("OVERCHARGE_SQUARE", OVERCHARGE_SQUARE));
  macros.insert(std::make_pair("OVERCHARGE_EXPONENTIAL", OVERCHARGE_EXPONENTIAL));

  // map_link type
  macros.insert(std::make_pair("MAPLINK_THROUGH", MAPLINK_THROUGH));
  macros.insert(std::make_pair("MAPLINK_BOUNCE", MAPLINK_BOUNCE));
  macros.insert(std::make_pair("MAPLINK_BACK", MAPLINK_BACK));

  // projectile_type
  macros.insert(std::make_pair("PROJECTILE_FIRE_FORM", PROJECTILE_FIRE_FORM));
  macros.insert(std::make_pair("PROJECTILE_LIGHTNING_FORM", PROJECTILE_LIGHTNING_FORM));
  macros.insert(std::make_pair("PROJECTILE_AIR", PROJECTILE_AIR));
  macros.insert(std::make_pair("PROJECTILE_ARROW", PROJECTILE_ARROW));
  macros.insert(std::make_pair("PROJECTILE_BULLET", PROJECTILE_BULLET));
  macros.insert(std::make_pair("PROJECTILE_SPECIAL", PROJECTILE_SPECIAL));

  // race_type
  macros.insert(std::make_pair("RACE_NONE", RACE_NONE));
  macros.insert(std::make_pair("RACE_NATURE", RACE_NATURE));
  macros.insert(std::make_pair("RACE_HOMO", RACE_HOMO));
  macros.insert(std::make_pair("RACE_OMONCULUS", RACE_OMONCULUS));
  macros.insert(std::make_pair("RACE_MECANICUS", RACE_MECANICUS));
  macros.insert(std::make_pair("RACE_UNDEAD", RACE_UNDEAD));
  macros.insert(std::make_pair("RACE_WALL", RACE_WALL));
  macros.insert(std::make_pair("RACE_GOD", RACE_GOD));

  // damage_type
  macros.insert(std::make_pair("DAMAGE_TYPE_NUMBER", DAMAGE_TYPE_NUMBER));

  macros.insert(std::make_pair("DAMAGE_SLASH", DAMAGE_SLASH));
  macros.insert(std::make_pair("DAMAGE_PUNCTURE", DAMAGE_PUNCTURE));
  macros.insert(std::make_pair("DAMAGE_BLUNT", DAMAGE_BLUNT));
  macros.insert(std::make_pair("DAMAGE_FIRE", DAMAGE_FIRE));
  macros.insert(std::make_pair("DAMAGE_LIGHTNING", DAMAGE_LIGHTNING));
  macros.insert(std::make_pair("DAMAGE_FROST", DAMAGE_FROST));
  macros.insert(std::make_pair("DAMAGE_POISON", DAMAGE_POISON));
  macros.insert(std::make_pair("DAMAGE_ACID", DAMAGE_ACID));
  macros.insert(std::make_pair("DAMAGE_MIND", DAMAGE_MIND));
  macros.insert(std::make_pair("DAMAGE_SOLAR", DAMAGE_SOLAR));
  macros.insert(std::make_pair("DAMAGE_AETHER", DAMAGE_AETHER));

  // loot_type
  macros.insert(std::make_pair("LOOT_CORPSE", LOOT_CORPSE));
  macros.insert(std::make_pair("LOOT_TREASURE", LOOT_TREASURE));

  // quest_step_type
  macros.insert(std::make_pair("QUEST_SLAY", QUEST_SLAY));
  macros.insert(std::make_pair("QUEST_OBTAIN", QUEST_OBTAIN));
  macros.insert(std::make_pair("QUEST_DISCOVER", QUEST_DISCOVER));
  macros.insert(std::make_pair("QUEST_TALK", QUEST_TALK));

  // way_type
  macros.insert(std::make_pair("WAY_CLASS", WAY_CLASS));
  macros.insert(std::make_pair("WAY_RACE", WAY_RACE));
  macros.insert(std::make_pair("WAY_ORIGIN", WAY_ORIGIN));
  macros.insert(std::make_pair("WAY_CULTURE", WAY_CULTURE));
  macros.insert(std::make_pair("WAY_RELIGION", WAY_RELIGION));
  macros.insert(std::make_pair("WAY_PROFESSION", WAY_PROFESSION));
  macros.insert(std::make_pair("WAY_RACE_MODIFIER", WAY_RACE_MODIFIER));
  macros.insert(std::make_pair("WAY_TITLE", WAY_TITLE));

  // speech_type
  macros.insert(std::make_pair("SPEECH_TEXT", SPEECH_TEXT));
  macros.insert(std::make_pair("SPEECH_VOCAL", SPEECH_VOCAL));
  macros.insert(std::make_pair("SPEECH_LOG", SPEECH_LOG));
  macros.insert(std::make_pair("SPEECH_WARNING", SPEECH_WARNING));
  macros.insert(std::make_pair("SPEECH_ERROR", SPEECH_ERROR));

  // teams relations
  macros.insert(std::make_pair("TEAM_SAME", TEAM_SAME));
  macros.insert(std::make_pair("TEAM_ALLY", TEAM_ALLY));
  macros.insert(std::make_pair("TEAM_NEUTRAL", TEAM_NEUTRAL));
  macros.insert(std::make_pair("TEAM_ENEMY", TEAM_ENEMY));

  // window_type
  macros.insert(std::make_pair("WINDOW_FULLSCREEN", WINDOW_FULLSCREEN));
  macros.insert(std::make_pair("WINDOW_WINDOW", WINDOW_WINDOW));
  macros.insert(std::make_pair("WINDOW_BORDERLESS", WINDOW_BORDERLESS));
}

const int32_t Database::getTargetFromMacro(const std::string macro) { return macros.find(macro) != macros.end() ? macros.at(macro) : stoi(macro); }

const Attributes * Database::getAttributes(const std::string attributes) { return this->attributes.at(attributes); }
const std::string Database::getAttributesFile(const std::string attributes) { return attributesFiles.at(attributes); }
const Block * Database::getBlock(const std::string block) { return blocks.at(block); }
std::map<const std::string, const Block *> Database::getAvaillableBlocks() { return blocks; }
const Character * Database::getCharacter(const std::string character) { return characters.at(character); }
const Effect * Database::getEffect(const std::string effect) { return effects.at(effect); }
const Event * Database::getEvent(const std::string event) { return events.at(event); }
const Item * Database::getItem(const std::string item) { return items.at(item); }
const std::string Database::getItemFile(const std::string item) { return itemsFiles.at(item); }
const Furniture * Database::getFurniture(const std::string furniture) { return furnitures.at(furniture); }
const std::string Database::getFurnitureFile(const std::string furniture) { return furnituresFiles.at(furniture); }
const Gear * Database::getGear(const std::string gear) { return gears.at(gear); }
const Map * Database::getMap(const std::string map) { return maps.at(map); }
const Projectile * Database::getProjectile(const std::string projectile) { return projectiles.at(projectile); }
const Quest * Database::getQuest(const std::string quest) { return quests.at(quest); }
const Skill * Database::getSkill(const std::string skill) { return skills.at(skill); }
const PseudoSkill * Database::getPseudoSkill(const std::string pseudoSkill) { return pseudoSkills.at(pseudoSkill); }
const Speech * Database::getSpeech(const std::string speech) { return speechs.at(speech); }
const Way * Database::getWay(const std::string way) { return ways.at(way); }
const std::string Database::getWayFile(const std::string way) { return waysFiles.at(way); }
const int32_t Database::getRelation(const std::string team1, const std::string team2) {
  if(team1 == team2) {
    return TEAM_SAME;
  }
  try {
    return relations.at(team1).at(team2);
  } catch (const std::out_of_range &e) {
    return TEAM_NEUTRAL;
  }
}

std::list<std::string> Database::getKeysPaths() { return paths; }

std::list<std::pair<const std::string, const std::string>> Database::getWaysIncompatibilities() { return waysIncompatibilities; }

void Database::addAttributes(const Attributes * attributes) { this->attributes.insert(std::make_pair(attributes->name, attributes)); }
void Database::addAttributesFile(const std::string attributes, const std::string path) { attributesFiles.insert(std::make_pair(attributes, path)); }
void Database::addBlock(const Block * block) { blocks.insert(std::make_pair(block->name, block)); }
void Database::addCharacter(const Character * character) { characters.insert(std::make_pair(character->name, character)); }
void Database::addEffect(const Effect * effect) { effects.insert(std::make_pair(effect->name, effect)); }
void Database::addEvent(const Event * event) { events.insert(std::make_pair(event->name, event)); }
void Database::addItem(const Item * item) { items.insert(std::make_pair(item->name, item)); }
void Database::addItemFile(const std::string item, const std::string path) { itemsFiles.insert(std::make_pair(item, path)); }
void Database::addFurniture(const Furniture * furniture) { furnitures.insert(std::make_pair(furniture->name, furniture)); }
void Database::addFurnitureFile(const std::string furniture, const std::string path) { furnituresFiles.insert(std::make_pair(furniture, path)); }
void Database::addGear(const Gear * gear) { gears.insert(std::make_pair(gear->name, gear)); }
void Database::addMap(const Map * map) { maps.insert(std::make_pair(map->name, map)); }
void Database::addProjectile(const Projectile * projectile) { projectiles.insert(std::make_pair(projectile->name, projectile)); }
void Database::addQuest(const Quest * quest) { quests.insert(std::make_pair(quest->name, quest)); }
void Database::addSkill(const Skill * skill) { skills.insert(std::make_pair(skill->name, skill)); }
void Database::addPseudoSkill(const PseudoSkill * pseudoSkill) { pseudoSkills.insert(std::make_pair(pseudoSkill->name, pseudoSkill)); }
void Database::addSpeech(const Speech * speech) { speechs.insert(std::make_pair(speech->name, speech)); }
void Database::addWay(const Way * way) { ways.insert(std::make_pair(way->name, way)); }
void Database::addWayFile(const std::string way, const std::string path) { waysFiles.insert(std::make_pair(way, path)); }
void Database::addRelation(const std::string team1, const std::string team2, int32_t relation) {
  if(relations.find(team1) == relations.end()) {
    std::map<const std::string, int32_t> map = std::map<const std::string, int32_t>();
    relations.insert(std::make_pair(team1, map));
  }
  relations.at(team1).insert(std::make_pair(team2, relation));
  if(relations.find(team2) == relations.end()) {
    std::map<const std::string, int32_t> map = std::map<const std::string, int32_t>();
    relations.insert(std::make_pair(team2, map));
  }
  relations.at(team2).insert(std::make_pair(team1, relation));
}

void Database::addWayImcompatibility(const std::string way1, const std::string way2) {
  waysIncompatibilities.push_back(std::make_pair(way1, way2));
}
void Database::addKeyPath(const std::string path) { paths.push_back(path); };
