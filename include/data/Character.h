#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <list>
#include <string>
#include <cmath>
#include <algorithm>

#include "data/Attributes.h"

#include "Values.h"

namespace character {
  static long id_cpt = 0;
}

typedef struct CharacterDisplay {
  std::string name;
  int id;
  int hp;
  int maxHp;
  int mana;
  int maxMana;
  int soulBurn;
  int soulBurnTreshold;
  int flow;
  bool player_character;
  int type;
  long x;
  long y;
  int orientation;
  std::string team;
  int armor;
  float damage_reductions[DAMAGE_TYPE_NUMBER];
  int damages[DAMAGE_TYPE_NUMBER];
  int teamRelation;
} CharacterDisplay;

class Character {
  public:
    const long id = ++character::id_cpt;
    const std::string name;
    const bool player_character;
    const Speech * death_speech;
    const std::list<const Speech *> talking_speechs;
    const int type;
    const std::string attributes;
    // not instancied character constructor
    Character(
      std::string name,
      bool player_character,
      const Speech * death_speech,
      const std::list<const Speech *> talking_speechs,
      int type,
      long gold,
      long xp,
      int level,
      std::list<Item *> items,
      std::list<Weapon *> weapons,
      std::list<Ammunition *> ammunition,
      std::list<Effect *> effects,
      std::list<Skill *> skills
    ):
      name(name),
      player_character(player_character),
      death_speech(death_speech),
      talking_speechs(talking_speechs),
      type(type),
      gold(gold),
      xp(xp),
      level(level),
      items(items),
      weapons(weapons),
      ammunition(ammunition),
      effects(effects),
      skills(skills)
    {}
    Character(
      const Character * from_database,
      std::string name,
      int x,
      int y,
      int orientation,
      int current_map_id,
      std::string team,
      AI * ai,
      Attributes * attributes,
      Way * race,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession
    ):
      name(name),
      player_character(from_database->player_character),
      death_speech(from_database->death_speech),
      talking_speechs(from_database->talking_speechs),
      type(from_database->type),
      gold(from_database->gold),
      xp(from_database->xp),
      level(from_database->level),
      x(x),
      y(y),
      orientation(orientation),
      current_map_id(current_map_id),
      team(team),
      ai(ai),
      items(from_database->items),
      weapons(from_database->weapons),
      ammunition(from_database->ammunition),
      effects(from_database->effects),
      skills(from_database->skills),
      attributes(attributes->name),
      race(race),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession)
    {
      applyAttributes(attributes, true);
    }
    Character(
      std::string name,
      bool player_character,
      int type,
      long x,
      long y,
      int orientation,
      int current_map_id,
      long gold,
      long xp,
      int level,
      std::string team,
      Gear * gear,
      std::list<Item *> items,
      std::list<Weapon *> weapons,
      std::list<Ammunition *> ammunition,
      std::list<Effect *> effects,
      std::list<Skill *> skills,
      Attributes * attributes,
      Way * race,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession
    ):
      name(name),
      player_character(player_character),
      type(type),
      x(x),
      y(y),
      orientation(orientation),
      current_map_id(current_map_id),
      gold(gold),
      xp(xp),
      level(level),
      ai(nullptr),
      team(team),
      gear(gear),
      items(items),
      weapons(weapons),
      ammunition(ammunition),
      effects(effects),
      skills(skills),
      attributes(attributes->name),
      race(race),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession)
    {
      applyAttributes(attributes, false);
    }
    void applyAttributes(const Attributes * attributes, bool applyGear);
    bool isAlive();
    bool isSoulBurning();
    int getX();
    int getY();
    int getOrientation();
    int getHp();
    int getMaxHp();
    int getMana();
    int getMaxMana();
    int getArmor();
    int getSoulBurnTreshold();
    int getCurrentSoulBurn();
    int getFlow();
    int getVisionRange();
    int getVisionPower();
    int getDetectionRange();
    long getCurrentMapId();
    long getGold();
    long getXP();
    long getLevel();

    AI * getAI();
    std::string getTeam();

    Gear * getGear();
    std::list<Item *> getItems();
    std::list<Weapon *> getWeapons();
    std::list<Ammunition *> getAmmunitions();
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();

    void setOrientation(int orientation);
    void move(int orientation);
    void move(int y, int x);
    void move(int y, int x, int orientation);
    void hpHeal(int hp);
    void incrMaxHp();
    void setHp(int hp);
    void manaHeal(int mana);
    void incrMaxMana();
    void setMana(int mana);
    void incrArmor();
    void incrSoulBurnTreshold();
    void setCurrentSoulBurn(int soulBurn);
    void incrFlow();
    void incrVisionRange();
    void incrVisionPower();
    void incrDetectionRange();
    void setCurrentMapId(long map_id);

    void applySoulBurn();
    void gainGold(long gold);
    void payMana(int cost);
    void gainXP(long xp);
    void gainLevel();

    void setAI(AI * ai);
    void setTeam(std::string team);

    void equip(Item * to_equip);
    void equip(Weapon * to_equip);
    void unequip(int type);
    void unequipWeapon();

    void addEffect(Effect * e);
    void addSkill(Skill * s);
    void removeEffect(Effect * e);
    void removeSkill(Skill * s);

    void setWay(Way * way);

    void addItem(Item * i);
    void addWeapon(Weapon * w);
    void addAmmunition(Ammunition * a);
    void removeItem(Item * i);
    void removeWeapon(Weapon * w);
    void removeAmmunition(Ammunition * a);

    int isChanneling();
    int isStunned();
    int isCloaked();
    int isInvisible();
    int isSleeping();
    int cloakPower();
    bool isInWeakState();

    void useSkill(Skill * skill, Character * target, Adventure * adventure, long overcharge);
    int getDamageFromType(int damage_type);
    float getDamageReductionFromType(int damage_type);
    Projectile * shoot(const Character * target, int y, int x);
    void reload(Ammunition * ammo);
    void attack(Character * target);
    void receiveAttack(int damages[DAMAGE_TYPE_NUMBER], int orientation);
    void receiveCriticalAttack(int damages[DAMAGE_TYPE_NUMBER]);
    std::string to_string(long offsetY, long offsetX);
    std::string full_to_string(Adventure * adventure);
    static CharacterDisplay * from_string(std::string to_read);
    static Character * full_from_string(std::string to_read);
    bool operator == (const Character& c) const { return id == c.id; }
    bool operator != (const Character& c) const { return !operator==(c); }

  private:
    int x;
    int y;
    int orientation;
    int current_map_id;
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    int armor;
    int soulBurnTreshold;
    int currentSoulBurn;
    int flow;
    int visionRange;
    int visionPower;
    int detectionRange;

    long gold;
    long xp;
    int level;

    AI * ai;
    std::string team;

    Gear * gear;
    std::list<Item *> items;
    std::list<Weapon *> weapons;
    std::list<Ammunition *> ammunition;
    std::list<Effect *> effects;
    std::list<Skill *> skills;

    Way * race;
    Way * origin;
    Way * culture;
    Way * religion;
    Way * profession;
};

#endif // _CHARACTER_H_
