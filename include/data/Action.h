#ifndef _ACTION_H_
#define _ACTION_H_

#include <list>
#include <string>
#include <cmath>
#include <algorithm>

#include "Values.h"

class Action {
  public:
    const int type;
    const int orientation;
    const Skill * skill;
    const Character * target;
    const int target_x;
    const int target_y;
    const MapLink * link;
    const std::string object;
    const int overcharge_power;
    const int overcharge_duration;
    const int overcharge_range;
    Action(
      const int type,
      Character * user,
      const int orientation,
      const Skill * skill,
      const Character * target,
      const int target_x,
      const int target_y,
      const MapLink * link,
      const std::string object,
      const int overcharge_power,
      const int overcharge_duration,
      const int overcharge_range
    ):
      type(type),
      user(user),
      orientation(orientation),
      skill(skill),
      target(target),
      target_x(target_x),
      target_y(target_y),
      link(link),
      object(object),
      overcharge_power(overcharge_power),
      overcharge_duration(overcharge_duration),
      overcharge_range(overcharge_range)
    {}
    void execute(Adventure * adventure);
    Character * getUser();
  private:
    Character * user;
};

#endif // _ACTION_H_
