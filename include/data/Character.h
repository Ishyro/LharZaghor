#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <list>
#include <string>
#include <map>
#include <set>
#include <array>
#include <cmath>
#include <algorithm>

#include "data/Projectile.h"

#include "data/items/Item.h"

#include "data/ways/Attributes.h"

#include "util/MapUtil.h"

#include "Values.h"

namespace character {
  static long id_cpt = 0;
}

typedef struct CharacterDisplay {
  std::string name;
  long id;
  int hp;
  int maxHp;
  int mana;
  int maxMana;
  int shield;
  int maxShield;
  float stamina;
  float satiety;
  int soulBurn;
  int soulBurnTreshold;
  int flow;
  bool player_character;
  int type;
  float x;
  float y;
  float z;
  float size;
  float height;
  float orientation;
  std::string team;
  int xp;
  int level;
  Speech * talking_speech;
  float damage_reductions[DAMAGE_TYPE_NUMBER];
  int damages[DAMAGE_TYPE_NUMBER];
  int teamRelation;
} CharacterDisplay;

class Character {
  public:
    const long id;
    const std::string name;
    const bool player_character;
    // not instancied character constructor
    Character(
      std::string name,
      bool player_character,
      Speech * death_speech,
      Speech * talking_speech,
      bool merchant,
      std::list<Effect *> effects,
      std::list<Skill *> skills,
      std::list<Item *> sellable_items,
      std::list<Effect *> sellable_effects,
      std::list<Skill *> sellable_skills
    ):
      name(name),
      id(0),
      player_character(player_character),
      death_speech(death_speech),
      talking_speech(talking_speech),
      merchant(merchant),
      effects(effects),
      skills(skills),
      sellable_items(sellable_items),
      sellable_effects(sellable_effects),
      sellable_skills(sellable_skills)
    {}
    Character(
      const Character * from_database,
      std::string name,
      long xp,
      int gold,
      int x,
      int y,
      int z,
      float orientation,
      Region * region,
      std::string team,
      AI * ai,
      Attributes * main_class,
      Attributes * second_class,
      Attributes * spec_class,
      Gear * gear,
      Race * race,
      std::list<Race *> race_modifiers,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession,
      std::list<Way *> titles
    ):
      name(name),
      id(++character::id_cpt),
      player_character(from_database->player_character),
      death_speech(from_database->death_speech),
      talking_speech(from_database->talking_speech),
      gold(gold),
      xp(xp),
      level(1),
      coord(MapUtil::makeVector3(x + 0.5F, y + 0.5F, z)),
      orientation(orientation),
      region(region),
      merchant(from_database->merchant),
      team(team),
      ai(ai),
      effects(std::list<Effect *>()),
      skills(from_database->skills),
      sellable_items(std::list<Item *>()),
      sellable_effects(from_database->sellable_effects),
      sellable_skills(from_database->sellable_skills),
      main_class(main_class),
      second_class(second_class),
      spec_class(spec_class),
      race(race),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession),
      titles(std::list<Way *>())
    {
      for(Item * item : from_database->sellable_items) {
        sellable_items.push_back(Item::init(item, 1, 1));
      }
      for(Race * modifier : race_modifiers) {
        setWay( (Way *) modifier);
      }
      for(Way * title : titles) {
        setWay(title);
      }
      initializeCharacter(gear);
      initEffects(from_database->effects);
    }
    Character(
      int maxHp,
      int maxMana,
      int maxShield,
      int hp,
      int mana,
      int shield,
      int damage_multiplier,
      int soulBurnTreshold,
      int flow,
      int visionRange,
      int visionPower,
      int detectionRange,
      int currentSoulBurn,
      float stamina,
      float satiety,
      float savedHpRegen,
      float savedManaRegen,
      int channeledMana,
      std::string name,
      long id,
      bool player_character,
      Speech * death_speech,
      Speech * talking_speech,
      MapUtil::Vector3 coord,
      float size,
      float height,
      float orientation,
      Region * region,
      bool merchant,
      long gold,
      long xp,
      int level,
      std::string team,
      Gear * gear,
      std::list<Effect *> effects,
      std::list<Skill *> skills,
      std::list<Item *> sellable_items,
      std::list<Effect *> sellable_effects,
      std::list<Skill *> sellable_skills,
      Attributes * main_class,
      Attributes * second_class,
      Attributes * spec_class,
      Race * race,
      std::list<Race *> race_modifiers,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession,
      std::list<Way *> titles
    ):
      maxHp(maxHp),
      maxMana(maxMana),
      maxShield(maxShield),
      hp(hp),
      mana(mana),
      shield(shield),
      damage_multiplier(damage_multiplier),
      soulBurnTreshold(soulBurnTreshold),
      flow(flow),
      visionRange(visionRange),
      visionPower(visionPower),
      detectionRange(detectionRange),
      currentSoulBurn(currentSoulBurn),
      stamina(stamina),
      satiety(satiety),
      savedHpRegen(savedHpRegen),
      savedManaRegen(savedManaRegen),
      channeledMana(channeledMana),
      name(name),
      id(id),
      player_character(player_character),
      death_speech(death_speech),
      talking_speech(talking_speech),
      coord(coord),
      size(size),
      height(height),
      orientation(orientation),
      region(region),
      merchant(merchant),
      gold(gold),
      xp(xp),
      level(level),
      ai(nullptr),
      team(team),
      gear(gear),
      effects(effects),
      skills(skills),
      sellable_items(sellable_items),
      sellable_effects(sellable_effects),
      sellable_skills(sellable_skills),
      main_class(main_class),
      second_class(second_class),
      spec_class(spec_class),
      race(race),
      race_modifiers(race_modifiers),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession),
      titles(titles)
    {}

    ~Character();
    
    bool isMarkedDead();
    void markDead(bool dead);
    bool isAlive();
    bool isSoulBurning();
    MapUtil::Vector3 getCoord();
    float getOrientation();
    float getSize();
    float getHeight();
    int getHp();
    int getMaxHp();
    int getMana();
    int getChanneledMana();
    int getMaxMana();
    int getShield();
    int getMaxShield();
    float getStamina();
    float getSatiety();
    int getSoulBurnThreshold();
    int getCurrentSoulBurn();
    int getFlow();
    long getRawPower();
    int getVisionRange();
    int getVisionPower();
    int getDetectionRange();
    Region * getRegion();
    Action * getCurrentAction();
    long getGold();
    long getXP();
    int getLevel();
    float getDamageMultiplier();
    int getPowerScore();

    bool needToSend();
    void setNeedToSend(bool need_to_send);

    AI * getAI();
    std::string getTeam();
    Speech * getDeathSpeech();
    Speech * getTalkingSpeech();
    int getType();

    Gear * getGear();
    float getActionTimeModifier();
    float getHandActionTimeModifier();
    float getSkillTimeModifier();
    float getMovementTimeModifier();
    float getStrikeTime(int slot);
    float getReloadTime(int slot);
    float getSwapTime(int slot1, int slot2);
    float getUseTime(Item * item);
    int getLight();
    std::list<Item *> getLoot();
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
    std::map<Skill *, std::array<int, DAMAGE_TYPE_NUMBER>> getDamageSkills();

    std::list<Item *> getSellableItems();
    std::list<Effect *> getSellableEffects();
    std::list<Skill *> getSellableSkills();
    std::list<Way *> getSellableTitles();

    Attributes * getMainClass();
    Attributes * getSecondClass();
    Attributes * getSpecClass();
    Race * getRace();
    std::list<Race *> getRaceModifiers();
    Way * getOrigin();
    Way * getCulture();
    Way * getReligion();
    Way * getProfession();
    std::list<Way *> getTitles();

    void setOrientation(float orientation);
    void setSize(float size);
    void move(MapUtil::Vector3 coord, float orientation, World * world);
    void hpHeal(int hp);
    void incrMaxHp();
    void setHp(int hp);
    void manaHeal(int mana);
    void incrMaxMana();
    void setMana(int mana);
    void shieldRestore(int shield);
    void setShield(int shield);
    void incrMaxShield();
    void addStamina(float stamina);
    void addSatiety(float satiety);
    void removeStamina(float stamina);
    void removeSatiety(float satiety);
    void setStamina(float stamina);
    void setSatiety(float satiety);
    void incrDamageMultiplier();
    void incrSoulBurnTreshold();
    void setCurrentSoulBurn(int soulBurn);
    void incrFlow();
    void incrVisionRange();
    void incrVisionPower();
    void incrDetectionRange();
    void setRegion(Region * region);
    void setCurrentAction(Action * action);

    void applySoulBurn();
    void applyManaWaste();
    void channel(int cost);
    void applyTiredness();
    void applyHunger();
    void applyEffects();
    void rest();
    void gainGold(long gold);
    void loseGold(long gold);
    void payMana(int cost);
    void gainXP(long xp);
    void gainLevel();
    void newSkillsAndEffects();
    void selectSecondAttributes();

    void setAI(AI * ai);
    void setTeam(std::string team);
    void setDeathSpeech(std::string option, Database * database);
    void setTalkingSpeech(std::string option, Database * database);

    void add(Item * item, int slot, int x, int y);
    Item * remove(int slot, int x, int y);
    float swap(int slot1, int slot2);

    void addEffect(Effect * e);
    void addSkill(Skill * s);
    bool hasSkill(Skill * s);
    void removeEffect(Effect * e);
    void removeSkill(Skill * s);

    void setWay(Way * way);

    void addItem(Item * i, int x, int y, int slot);
    void removeItem(int x, int y, int slot);
    void useItem(int x, int y, int slot);

    bool isFlying();
    bool isChanneling();
    bool isStunned();
    bool isCloaked();
    bool isInvisible();
    bool isEtheral();
    bool isInvulnerable();
    bool isSleeping();
    int cloakPower();
    bool isInWeakState();

    void useSkill(Skill * skill, Target * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_range);
    int getDamageFromType(int damage_type, int slot);
    float getDamageReductionFromType(int damage_type);
    Projectile * shoot(Target * target, Adventure * adventure, int slot);
    void reload(ItemSlot * ammo, int slot_weapon);
    void attack(Character * target, Adventure * adventure, int type);
    void mainAttack(Character * target, Adventure * adventure, int type);
    void subAttack(Character * target, Adventure * adventure, int type);
    ItemSlot * canReload(int slot);
    void receiveAttack(int damages[DAMAGE_TYPE_NUMBER], float orientation, int type);
    void receiveCriticalAttack(int damages[DAMAGE_TYPE_NUMBER], int type);
    int tryAttack(std::array<int, DAMAGE_TYPE_NUMBER> damages, int type);
    void trade(Character * buyer, int object_type, std::string object_name, float price_modifier);
    std::set<std::string> getTags();
    std::string to_string();
    std::string full_to_string(Adventure * adventure);
    static CharacterDisplay * from_string(std::string to_read);
    static Character * full_from_string(std::string to_read, Adventure * adventure);
    bool operator == (const Character& c) const { return id == c.id; }
    bool operator != (const Character& c) const { return !operator==(c); }

  private:
    void initializeCharacter(Gear * gear);
    void initSkillsAndEffects();
    void initEffects(std::list<Effect *> effects);
    MapUtil::Vector3 coord;
    float size;
    float height;
    float orientation;
    Region * region;
    Action * current_action;
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    float stamina;
    float satiety;
    float savedHpRegen;
    float savedManaRegen;
    int channeledMana;
    int shield;
    int maxShield;
    int damage_multiplier;
    int soulBurnTreshold;
    int currentSoulBurn;
    int flow;
    int visionRange;
    int visionPower;
    int detectionRange;
    bool need_to_send = false;
    bool dead = false;

    long gold;
    long xp;
    int level;

    AI * ai;
    std::string team;
    Speech * death_speech;
    Speech * talking_speech;

    Gear * gear;
    std::list<Effect *> effects;
    std::list<Skill *> skills;

    bool merchant;
    std::list<Item *> sellable_items;
    std::list<Effect *> sellable_effects;
    std::list<Skill *> sellable_skills;

    Attributes * main_class;
    Attributes * second_class;
    Attributes * spec_class;
    Race * race;
    std::list<Race *> race_modifiers;
    Way * origin;
    Way * culture;
    Way * religion;
    Way * profession;
    std::list<Way *> titles;
};

#endif // _CHARACTER_H_
