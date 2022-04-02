#include "data/Map.h"

std::list<Character *> Map::getCharacters() { return characters; }
std::list<Projectile *> Map::getProjectiles() { return projectiles; }
std::list<Loot *> Map::getLoots() { return loots; }
// TODO : return right tile
Tile * Map::getTile(long x, long y) { return tiles[x][y]; }
int Map::getLight(long x, long y) { return lights[x][y] + light; }
void Map::applyDayLight(int light) { this->light = light; }

void Map::calculateLights() {
  // initialize
  long lightX[sizeX*sizeY];
  long lightY[sizeX*sizeY];
  long cpt = 0;
  for(long x = 0; x < sizeX; x++) {
    for(long y = 0; y < sizeY; y++) {
      lights[x][y] = tiles[x][y]->light;
      if (lights[x][y] > 0) {
        lightX[cpt] = x;
        lightY[cpt++] = y;
      }
    }
  }
  for(long i = 0; i < cpt; i++) {
    propagateLight(lightX[i],lightY[i]);
  }
}

void Map::propagateLight(long x, long y) {
  int light = lights[x][y] - 1;
  if(x > 1 && y > 1 && light > lights[x - 1][y - 1]) {
    lights[x - 1][y - 1] = light;
    propagateLight(x - 1,y - 1);
  }
  if(x > 1 && light > lights[x - 1][y]) {
    lights[x - 1][y] = light;
    propagateLight(x - 1,y);
  }
  if(x > 1 && y < sizeY - 2 && light > lights[x - 1][y + 1]) {
    lights[x - 1][y + 1] = light;
    propagateLight(x - 1,y + 1);
  }
  if(y > 1 && light > lights[x][y - 1]) {
    lights[x][y-1] = light;
    propagateLight(x,y - 1);
  }
  if(y < sizeY - 2 && light > lights[x][y + 1]) {
    lights[x][y + 1] = light;
    propagateLight(x,y + 1);
  }
  if(x < sizeX - 2 && y > 1 && light > lights[x + 1][y - 1]) {
    lights[x + 1][y - 1] = light;
    propagateLight(x + 1,y - 1);
  }
  if(x < sizeX - 2 && light > lights[x + 1][y]) {
    lights[x + 1][y] = light;
    propagateLight(x +1,y);
  }
  if(x < sizeX - 2 && y < sizeY - 2 && light > lights[x + 1][y + 1]) {
    lights[x + 1][y + 1] = light;
    propagateLight(x + 1,y + 1);
  }
}

void Map::setTile(long x, long y, Tile * tile) { tiles[x][y] = tile; }

void Map::crumble(long x, long y) {
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
void Map::killCharacter(Character * victim, Character * killer) {
  characters.remove(victim);
  SpeechManager::add(victim->death_speech);
  Loot * loot = (Loot *) malloc(sizeof(Loot));
  loot->x = victim->getX();
  loot->y = victim->getY();
  loot->gold = victim->getGold();
  for(auto w : victim->getWeapons()) {
    loot->weapons.push_back(w);
  }
  for(auto i : victim->getItems()) {
    loot->items.push_back(i);
  }
  loot->type = CORPSE;
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

void Map::tryHit(Projectile * p, Adventure * adventure) {
  int p_x = p->getX();
  int p_y = p->getY();
  for(auto c : characters) {
    if(c->getX() == p_x && c->getY() == p_y) {
      switch(p->target_type) {
        case SINGLE_TILE:
          p->attack_single_target(c, adventure);
          p->setLost(true);
          break;
        case SINGLE_CHARACTER:
          p->attack_single_target(c, adventure);
          break;
        case MULTIPLE_TILES:
        case MULTIPLE_CHARACTERS:
          p->attack_multiple_targets(characters, adventure);
          break;
        default:
          break;
      }
      break;
    }
  }
}

void Map::move(Character *c, int orientation) {
  switch(orientation) {
    case NORTH:
      if(c->getY() < sizeY - 1 && !tiles[c->getX()][c->getY() + 1]->untraversable) {
        c->move(orientation);
      }
      break;
    case NORTH_EAST:
      if(c->getY() < sizeY - 1 && c->getX() < sizeX - 1 && !tiles[c->getX() + 1][c->getY() + 1]->untraversable) {
        c->move(orientation);
      }
      break;
    case EAST:
      if(c->getX() < sizeX - 1 && !tiles[c->getX() + 1][c->getY()]->untraversable) {
        c->move(orientation);
      }
      break;
    case SOUTH_EAST:
      if(c->getY() > 0 && c->getX() < sizeX - 1 && !tiles[c->getX() + 1][c->getY() - 1]->untraversable) {
        c->move(orientation);
      }
      break;
    case SOUTH:
      if(c->getY() > 0 && !tiles[c->getX()][c->getY() - 1]->untraversable) {
        c->move(orientation);
      }
      break;
    case SOUTH_WEST:
      if(c->getY() > 0 && c->getX() > 0 && !tiles[c->getX() - 1][c->getY() - 1]->untraversable) {
        c->move(orientation);
      }
      break;
    case WEST:
      if(c->getX() > 0 && !tiles[c->getX() - 1][c->getY()]->untraversable) {
        c->move(orientation);
      }
      break;
    case NORTH_WEST:
      if(c->getY() < sizeY - 1, c->getX() > 0 && !tiles[c->getX() - 1][c->getY() + 1]->untraversable) {
        c->move(orientation);
      }
      break;
  }
}
