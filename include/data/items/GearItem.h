#ifndef _GEAR_ITEM_H_
#define _GEAR_ITEM_H_

#include <list>
#include <string>

#include "data/items/Item.h"

#include "Values.h"

class GearItem : public Item {
  public:
    const int32_t swap_time;
    GearItem(
      std::string name,
      int64_t id,
      int32_t type,
      int32_t type2,
      int32_t tier,
      int32_t max_tier,
      float weight,
      int32_t sizeX,
      int32_t sizeY,
      int32_t gold_value,
      bool droppable,
      bool usable,
      bool consumable,
      int32_t use_time,
      std::list<Effect *> effects,
      int32_t swap_time
    ):
      Item(
        name,
        id,
        type,
        type2,
        tier,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        effects
      ),
      swap_time(swap_time)
    {}
    GearItem(GearItem * item, int32_t tier):
      Item(item, tier),
      swap_time(item->swap_time)
    {}
    float getWeight();
    virtual std::string to_string() = 0;
};

#endif // _GEAR_ITEM_H_
