#ifndef _REGION_H_
#define _REGION_H_

#include <algorithm>
#include <list>
#include <array>
#include <set>
#include <map>
#include <vector>
#include <string>

#include "data/Character.h"
#include "data/BlocksChunk.h"

#include "data/furnitures/Furniture.h"
#include "data/furnitures/BasicFurniture.h"
#include "data/furnitures/ContainerFurniture.h"
#include "data/furnitures/CraftingFurniture.h"
#include "data/furnitures/LinkedFurniture.h"
#include "data/furnitures/SkillFurniture.h"
#include "data/furnitures/SwitchFurniture.h"

#include "util/MathUtil.h"

#include <iostream>

class Region {
  public:
    const MathUtil::Vector3i id;
    Region(MathUtil::Vector3i id, std::array<BlocksChunk *, 27> chunks):id(id),chunks(chunks) {}
    void addCharacter(Character * character);
    bool removeCharacter(Character * character);
    Block * getBlock(MathUtil::Vector3 coord);
    Block * getBlock(MathUtil::Vector3i coord);
    std::map<const MathUtil::Vector3i, Block *> getBlocks();
    float getMoveCost(Character * c, MathUtil::Vector3 ori, MathUtil::Vector3 dest);
    bool tryMove(Character * c, MathUtil::Vector3 dest);
    bool intersect(Character * character, MathUtil::Vector3 dest, Furniture * furniture);
    bool move(Character * c, float orientation, MathUtil::Vector3 dest, float ap, World * world);
    MathUtil::Vector3 getCoordsOnSlope(Character * character, MathUtil::Vector3 dest, int32_t orientation, float z);
    bool canSee(Character * watcher, Character * target);
    std::list<Character *> getCharacters();
    std::list<Character *> getCharacters(Character * character);
    std::list<Furniture *> getFurnitures();
    std::list<Furniture *> getFurnitures(Character * character);
    Furniture * getFurniture(MathUtil::Vector3i coord);
  private:
    std::array<BlocksChunk *, 27> chunks;
    std::list<Character *> characters;
};

#endif // _REGION_H_
