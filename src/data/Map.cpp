#include <sstream>

#include "data/Adventure.h"
#include "data/Item.h"
#include "data/Weapon.h"

#include "communication/SpeechManager.h"

#include "data/Map.h"

std::list<Character *> Map::getCharacters() { return characters; }
std::list<Projectile *> Map::getProjectiles() { return projectiles; }
std::list<Loot *> Map::getLoots() { return loots; }
// TODO : return right tile
Tile * Map::getTile(long y, long x) { return tiles[y][x]; }
int Map::getLight(long y, long x) { return std::max(lights[y][x], light); }
void Map::applyDayLight(int light) { this->light = light; }

void Map::calculateLights() {
  // initialize
  long lightX[sizeX*sizeY];
  long lightY[sizeX*sizeY];
  long cpt = 0;
  for(long y = 0; y < sizeY; y++) {
    for(long x = 0; x < sizeX; x++) {
      lights[y][x] = tiles[y][x]->light;
      if (lights[y][x] > 0) {
        lightX[cpt] = x;
        lightY[cpt++] = y;
      }
    }
  }
  for(long i = 0; i < cpt; i++) {
    propagateLight(lightY[i],lightX[i]);
  }
}

void Map::propagateLight(long y, long x) {
  int light = lights[y][x] - 1;
  if(x > 1 && y > 1 && light > lights[y - 1][x - 1]) {
    lights[y - 1][x - 1] = light;
    propagateLight(y - 1, x - 1);
  }
  if(x > 1 && light > lights[y][x - 1]) {
    lights[y][x - 1] = light;
    propagateLight(y, x - 1);
  }
  if(x > 1 && y < sizeY - 1 && light > lights[y + 1][x - 1]) {
    lights[y + 1][x - 1] = light;
    propagateLight(y + 1, x - 1);
  }
  if(y > 1 && light > lights[y - 1][x]) {
    lights[y-1][x] = light;
    propagateLight(y - 1, x);
  }
  if(y < sizeY - 1 && light > lights[y + 1][x]) {
    lights[y + 1][x] = light;
    propagateLight(y + 1, x);
  }
  if(x < sizeX - 1 && y > 1 && light > lights[y - 1][x + 1]) {
    lights[y - 1][x + 1] = light;
    propagateLight(y - 1, x + 1);
  }
  if(x < sizeX - 1 && light > lights[y][x + 1]) {
    lights[y][x + 1] = light;
    propagateLight(y, x + 1);
  }
  if(x < sizeX - 1 && y < sizeY - 1 && light > lights[y + 1][x + 1]) {
    lights[y + 1][x + 1] = light;
    propagateLight(y + 1, x + 1);
  }
}

void Map::setTile(long y, long x, Tile * tile) { tiles[y][x] = tile; }

void Map::crumble(long y, long x) {
  for(auto character : characters) {
    if(character->getX() == x && character->getY() == y) {
      if(character->type == WALL) {
        removeCharacter(character);
        delete character;
      }
      else break;
    }
  }
}

void Map::addCharacter(Character * c) { characters.push_back(c); }
void Map::addProjectile(Projectile * p) { projectiles.push_back(p); }
void Map::addLoot(Loot * l) { loots.push_back(l); }

void Map::removeCharacter(Character * c) { characters.remove(c); }
void Map::killCharacter(Character * killer, Character * victim) {
  characters.remove(victim);
  SpeechManager::add(victim->death_speech);
  Loot * loot = new Loot();
  loot->weapons = std::list<Weapon *>();
  loot->items = std::list<Item *>();
  loot->ammunitions = std::list<Ammunition *>();
  loot->x = victim->getX();
  loot->y = victim->getY();
  loot->gold = victim->getGold();
  for(Weapon * w : victim->getWeapons()) {
    loot->weapons.push_back(w);
  }
  for(Item * i : victim->getItems()) {
    loot->items.push_back(i);
  }
  for(Ammunition * a : victim->getAmmunitions()) {
    loot->ammunitions.push_back(a);
  }
  loot->type = CORPSE;
  if(loot->gold == 0 && loot->weapons.empty() && loot->items.empty() && loot->ammunitions.empty()) {
    delete loot;
  } else {
    loots.push_back(loot);
  }
  killer->gainXP(victim->getXP() / 2);
  delete victim;
}

void Map::removeProjectile(Projectile * p) { projectiles.remove(p); }
void Map::destroyProjectile(Projectile * p) {
  projectiles.remove(p);
  delete p;
}

void Map::removeLoot(Loot * l) { loots.remove(l); }

void Map::destroyLoot(Loot * l) {
  loots.remove(l);
  delete l;
}

void Map::takeLoot(Character * c) {
  for(Loot * l : loots) {
    if(l->x == c->getX() && l->y == c->getY()) {
      loots.remove(l);
      for(auto i : l->items) {
        c->addItem(i);
      }
      for(auto w : l->weapons) {
        c->addWeapon(w);
      }
      for(auto a : l->ammunitions) {
        c->addAmmunition(a);
      }
      delete l;
    }
  }
}

void Map::move(Character *c, int orientation, Adventure * adventure) {
  bool is_legal = false;
  long destX;
  long destY;
  c->setOrientation(orientation);
  switch(orientation) {
    case NORTH:
      if(c->getY() < sizeY - 1 && !tiles[c->getY() + 1][c->getX()]->untraversable) {
        is_legal = true;
        destX = c->getX();
        destY = c->getY() + 1;
      }
      break;
    case NORTH_EAST:
      if(c->getY() < sizeY - 1 && c->getX() < sizeX - 1 && !tiles[c->getY() + 1][c->getX() + 1]->untraversable) {
        is_legal = true;
        destX = c->getX() + 1;
        destY = c->getY() + 1;
      }
      break;
    case EAST:
      if(c->getX() < sizeX - 1 && !tiles[c->getY()][c->getX() + 1]->untraversable) {
        is_legal = true;
        destX = c->getX() + 1;
        destY = c->getY();
      }
      break;
    case SOUTH_EAST:
      if(c->getY() > 0 && c->getX() < sizeX - 1 && !tiles[c->getY() - 1][c->getX() + 1]->untraversable) {
        is_legal = true;
        destX = c->getX() + 1;
        destY = c->getY() - 1;
      }
      break;
    case SOUTH:
      if(c->getY() > 0 && !tiles[c->getY() - 1][c->getX()]->untraversable) {
        is_legal = true;
        destX = c->getX();
        destY = c->getY() - 1;
      }
      break;
    case SOUTH_WEST:
      if(c->getY() > 0 && c->getX() > 0 && !tiles[c->getY() - 1][c->getX() - 1]->untraversable) {
        is_legal = true;
        destX = c->getX() - 1;
        destY = c->getY() - 1;
      }
      break;
    case WEST:
      if(c->getX() > 0 && !tiles[c->getY()][c->getX() - 1]->untraversable) {
        is_legal = true;
        destX = c->getX() - 1;
        destY = c->getY();
      }
      break;
    case NORTH_WEST:
      if(c->getY() < sizeY - 1 && c->getX() > 0 && !tiles[c->getY() + 1][c->getX() - 1]->untraversable) {
        is_legal = true;
        destX = c->getX() - 1;
        destY = c->getY() + 1;
      }
      break;
  }
  if(is_legal) {
    Character * to_kill = nullptr;
    for(Character * other : characters) {
      if (other->getX() == destX && other->getY() == destY) {
        if(adventure->getDatabase()->getRelation(c->getTeam(), other->getTeam()) == ENEMY) {
          c->attack(other);
          if(!other->isAlive()) {
            to_kill = other;
          } else {
            return;
          }
        } else {
          return; // there is a character, can't move on this position
        }
      }
    }
    if(to_kill != nullptr) {
      characters.remove(to_kill);
      killCharacter(c, to_kill);
      return;
    }
    c->move(orientation);
  }
}

void Map::actProjectile(Projectile * p, Adventure * adventure) {
  for(int i = 0; i <= p->getSpeed(); i++) {
    p->move();
    if(p->isAtDest()) {
      p->setLost(true);
    }
    if(p->getArea() > 1) {
      p->attack_multiple_targets(characters, adventure);
      for(Character * c : characters) {
        if(!c->isAlive()) {
          killCharacter(p->getOwner(), c);
        }
      }
    } else {
      for(Character * c : characters) {
        if(c->getX() == p->getX() && c->getY() == p->getY()) {
          p->attack_single_target(c, adventure);
          if(!c->isAlive()) {
            killCharacter(p->getOwner(), c);
          }
        }
      }
    }
    if(p->noDamage()) {
      projectiles.remove(p);
      delete p;
      break;
    }
  }
}

void Map::actAllProjectiles(Adventure * adventure) {
  for(Projectile * p : projectiles) {
    actProjectile(p, adventure);
  }
}

std::string Map::to_string(Character * player, Adventure * adventure) {
  std::string msg = name + "@";
  msg += std::to_string(offsetX) + "@";
  msg += std::to_string(offsetY) + "@";
  msg += std::to_string(sizeX) + "@";
  msg += std::to_string(sizeY) + "@";
  msg += std::to_string(outside) + "@";
  for(int y = 0; y < sizeY; y++) {
    for(int x = 0; x < sizeX; x++) {
      msg += std::to_string(x) + ";" + std::to_string(y) + ";" + getTile(y, x)->name + ";" + std::to_string(getTile(y, x)->untraversable) + ";" + std::to_string(getLight(y, x)) + "|";
    }
  }
  msg += "@";
  for(Character * character : characters) {
    msg += character->to_string(offsetY, offsetX) + std::to_string(adventure->getDatabase()->getRelation(character->getTeam(), player->getTeam())) + "|";
  }
  msg += "@";
  for(Projectile * projectile : projectiles) {
    msg += projectile->to_string(offsetY, offsetX) + "|";
  }
  msg += "@";
  for(Loot * loot : loots) {
    msg += std::to_string(loot->type) + ";" + std::to_string(loot->x - offsetX) + ";" + std::to_string(loot->y - offsetY) + "|";
  }
  msg += "@";
  return msg;
}

MapDisplay * Map::from_string(std::string to_read) {
  std::string msg = to_read;
  MapDisplay * display = new MapDisplay();
  display->name = msg.substr(0, msg.find('@'));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  display->offsetX = stol(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  display->offsetY = stol(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  display->sizeX = stol(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  display->sizeY = stol(msg.substr(0, msg.find('@')));
  msg = msg.substr(msg.find('@') + 1, msg.length());
  std::string outside_str = msg.substr(0, msg.find('@'));
  display->outside = (outside_str == "1");
  msg = msg.substr(msg.find('@') + 1, msg.length());
  display->tiles = std::vector<std::vector<Tile *>>(display->sizeY);
  for(long i = 0; i < display->sizeY; i++) {
    display->tiles[i] = std::vector<Tile *>(display->sizeX);
  }
  std::istringstream tiles(msg.substr(0, msg.find('@')));
  std::string tile;
  while(getline(tiles, tile, '|') && tile != "") {
    long x = stol(tile.substr(0, tile.find(';')));
    tile = tile.substr(tile.find(';') + 1, tile.length());
    long y = stol(tile.substr(0, tile.find(';')));
    tile = tile.substr(tile.find(';') + 1, tile.length());
    std::string name = tile.substr(0, tile.find(';'));
    tile = tile.substr(tile.find(';') + 1, tile.length());
    std::string untraversable_str = tile.substr(0, tile.find(';'));
    bool untraversable = (untraversable_str == "1");
    tile = tile.substr(tile.find(';') + 1, tile.length());
    int light = stoi(tile.substr(0, tile.find(';')));
    tile = tile.substr(tile.find(';') + 1, tile.length());
    display->tiles[y][x] = new Tile(name, untraversable, light);
  }
  msg = msg.substr(msg.find('@') + 1, msg.length());
  std::istringstream characters(msg.substr(0, msg.find('@')));
  std::string character;
  while(getline(characters, character, '|') && character != "") {
    CharacterDisplay * characterDisplay = Character::from_string(character);
    characterDisplay->teamRelation = stoi(character.substr(character.rfind(';') + 1, character.length()));
    display->characters.push_back(characterDisplay);
  }
  msg = msg.substr(msg.find('@') + 1, msg.length());
  std::istringstream projectiles(msg.substr(0, msg.find('@')));
  std::string projectile;
  while(getline(projectiles, projectile, '|') && projectile != "") {
    display->projectiles.push_back(Projectile::from_string(projectile));
  }
  msg = msg.substr(msg.find('@') + 1, msg.length());
  std::istringstream loots(msg.substr(0, msg.find('@')));
  std::string loot_str;
  while(getline(loots, loot_str, '|') && loot_str != "") {
    Loot * loot = new Loot();
    loot->type = stoi(tile.substr(0, tile.find(';')));
    tile = tile.substr(tile.find(';') + 1, tile.length());
    loot->x = stol(tile.substr(0, tile.find(';')));
    tile = tile.substr(tile.find(';') + 1, tile.length());
    loot->y = stol(tile.substr(0, tile.find(';')));
    tile = tile.substr(tile.find(';') + 1, tile.length());
    display->loots.push_back(loot);
  }
  msg = msg.substr(msg.find('@') + 1, msg.length());
  return display;
}
