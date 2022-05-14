#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include <string>
#include <cmath>

#include "Values.h"

typedef struct Ammunition {
  Projectile * projectile;
  int number;
  int gold_value;
  int ammo_type;
} Ammunition;

typedef struct ProjectileDisplay {
  std::string name;
  int id;
  int projectile_type;
  int x;
  int y;
  int orientation;
  int damages[DAMAGE_TYPE_NUMBER];
  int speed;
  int area;
  float waste_per_tile;
  float waste_per_tile_area;
  float waste_per_hit;
} ProjectileDisplay;

namespace projectile {
  static long id_cpt = 0;
}

class Projectile {
  public:
    const long id = ++projectile::id_cpt;
    const std::string name;
    const int projectile_type;
    const bool homing;
    Projectile(
      const std::string name,
      int projectile_type,
      bool homing,
      Skill * skill,
      int speed,
      int area,
      int falloff_range,
      float waste_per_tile,
      float waste_per_tile_area,
      float waste_per_hit,
      int damages[DAMAGE_TYPE_NUMBER]
    ):
      name(name),
      projectile_type(projectile_type),
      homing(homing),
      skill(skill),
      speed(speed),
      area(area),
      overcharge(overcharge),
      falloff_range(falloff_range),
      waste_per_tile(waste_per_tile),
      waste_per_tile_area(waste_per_tile_area),
      waste_per_hit(waste_per_hit)
    {
      lost = false;
      current_map_id = 0;
      x = 0;
      y = 0;
      dest_x = 0;
      dest_y = 0;
      target = nullptr;
      owner = nullptr;
      orientation = NO_ORIENTATION;
      overcharge = 0;
      current_travel = 0;
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
        this->current_damages[i] = damages[i];
      }
    }
    Projectile(
      const Projectile * projectile,
      int realDamages[DAMAGE_TYPE_NUMBER],
      int current_map_id,
      int x,
      int y,
      int dest_x,
      int dest_y,
      Character * target,
      Character * owner,
      int orientation,
      int overcharge
    ):
      name(projectile->name),
      projectile_type(projectile->projectile_type),
      homing(projectile->homing),
      current_map_id(current_map_id),
      x(x),
      y(y),
      dest_x(dest_x),
      dest_y(dest_y),
      skill(projectile->skill),
      target(target),
      owner(owner),
      orientation(orientation),
      speed(projectile->speed),
      area(projectile->area),
      overcharge(overcharge),
      falloff_range(projectile->falloff_range),
      current_travel(projectile->current_travel),
      waste_per_tile(projectile->waste_per_tile),
      waste_per_tile_area(projectile->waste_per_tile_area),
      waste_per_hit(projectile->waste_per_hit)
    {
      lost = false;
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = realDamages[i];
        this->current_damages[i] = realDamages[i];
      }
    }
    Projectile(
      const std::string name,
      int projectile_type,
      bool homing,
      int current_map_id,
      int x,
      int y,
      int dest_x,
      int dest_y,
      Skill * skill,
      Character * target,
      Character * owner,
      int orientation,
      int speed,
      int area,
      int overcharge,
      int falloff_range,
      float waste_per_tile,
      float waste_per_tile_area,
      float waste_per_hit,
      int damages[DAMAGE_TYPE_NUMBER]
    ):
      name(name),
      projectile_type(projectile_type),
      homing(homing),
      current_map_id(current_map_id),
      x(x),
      y(y),
      dest_x(dest_x),
      dest_y(dest_y),
      skill(skill),
      target(target),
      owner(owner),
      orientation(orientation),
      speed(speed),
      area(area),
      overcharge(overcharge),
      falloff_range(falloff_range),
      waste_per_tile(waste_per_tile),
      waste_per_tile_area(waste_per_tile_area),
      waste_per_hit(waste_per_hit)
    {
      lost = false;
      current_travel = 0;
      for(int i = 0; i < DAMAGE_TYPE_NUMBER; i++) {
        this->damages[i] = damages[i];
        this->current_damages[i] = damages[i];
      }
    }
    int getX();
    int getY();
    int getDestX();
    int getDestY();
    int getOrientation();
    int getRawDamage();
    int getDamageFromType(int damage_type);
    int getSpeed();
    int getArea();
    Character * getTarget();
    Character * getOwner();
    bool isAtDest();
    bool noDamage();
    void setX(int setX);
    void setY(int setY);
    void setDestX(int destX);
    void setDestY(int destY);
    void setTarget(Character * target);
    void setOwner(Character * owner);
    void move();
    void setLost(bool state);
    void attack_single_target(Character * target, Adventure * adventure);
    void attack_multiple_targets(std::list<Character *> characters, Adventure * adventure);
    std::string to_string(long offsetY, long offsetX);
    std::string full_to_string();
    static std::string ammo_to_string(Ammunition * ammo);
    static ProjectileDisplay * from_string(std::string to_read);
    static Projectile * full_from_string(std::string to_read);
    static Ammunition * ammo_from_string(std::string to_read);
    bool operator == (const Projectile& p) const { return id == p.id; }
    bool operator != (const Projectile& p) const { return !operator==(p); }
  private:
    int current_map_id;
    int x;
    int y;
    int target_type;
    int dest_x;
    int dest_y;
    bool lost;
    Skill * skill;
    Character * target;
    Character * owner;
    int orientation;
    int speed;
    int area;
    int overcharge;
    int falloff_range;
    int current_travel;
    float waste_per_tile;
    float waste_per_tile_area;
    float waste_per_hit;
    int damages[DAMAGE_TYPE_NUMBER];
    int current_damages[DAMAGE_TYPE_NUMBER];
    void nextOrientation();
};

#endif // _PROJECTILE_H_
