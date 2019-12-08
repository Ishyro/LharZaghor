#ifndef _MAP_H_
#define _MAP_H_

#include <list>
#include <string>

#include "src/data/Character.h"
#include "src/data/Projectile.h"
#include "src/data/Item.h"
#include "src/data/Weapon.h"
#include "src/data/Tile.h"

typedef struct Loot {
  const int type;
  const long x;
  const long y;
  const long gold;
  const std::list<Weapon *> weapons;
  const std::list<Item *> items;
} Loot;

namespace map {
  static long id_cpt = 0;
}

class Map {
  public:
    const std::string name;
    const long id = ++map::id_cpt;
    const long sizeX;
    const long sizeY;
    Map(std::string name) {
      // TODO READER
    }
    std::list<Character *> getCharacters();
    std::list<Projectile *> getProjectiles();
    std::list<Loot *> getLoots();
    Tile * getTile(long x, long y);
    void crumble(long x, long y);
    void addCharacter(Character * c);
    void addProjectile(Projectile * p);
    void addLoot(Loot * l);
    void removeCharacter(Character * c);
    void killCharacter(Character * c);
    void removeProjectile(Projectile * p);
    void destroyProjectile(Projectile * p);
    void removeLoot(Loot * l);
    void destroyLoot(Loot * l);
    void takeLoot(Character * c, Loot * l);
  private:
    std::list<Character *> characters;
    std::list<Projectile *> projectiles;
    std::list<Loot *> loots;
    Tile *** tiles;
};

typedef struct MapLink {
    int x1;
    int x2;
    int y1;
    int y2;
    Map * map1;
    Map * map2;
} MapLink;

#endif // _MAP_H_
