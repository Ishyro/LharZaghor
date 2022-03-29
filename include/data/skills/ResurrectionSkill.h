#ifndef _RESURRECTIONSKILL_H_
#define _RESURRECTIONSKILL_H_

#include "data/skills/PseudoSkill.h"

class ResurrectionSkill : public PseudoSkill {
  public:
    void activate(Character * owner, Character * target, Adventure * adventure, long overcharge, long map_id = 0L, long x = 0L, long y = 0L);
};

#endif // _RESURRECTIONSKILL_H_
