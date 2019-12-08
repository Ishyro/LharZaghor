#include "src/data/Gear.h"

std::list<Item *> Gear::equip(Item * new_item) {
  std::list<Item *> unequip;
  switch(new_item->type) {
    case HEAD:
      unequip.push_front(head);
      head = new_item;
      break;
    case ARMS:
      unequip.push_front(arms);
      arms = new_item;
      break;
    case BODY:
      unequip.push_front(body);
      body = new_item;
      break;
    case LEGS:
      unequip.push_front(legs);
      legs = new_item;
      break;
    case FULL_BODY:
      unequip.push_front(head);
      unequip.push_front(body);
      unequip.push_front(arms);
      unequip.push_front(legs);
      head = nullptr;
      arms = nullptr;
      legs = nullptr;
      body = new_item;
      break;
    case RING:
      if (left_ring == nullptr) {
        left_ring = new_item;
      }
      else if (right_ring == nullptr) {
        right_ring = new_item;
      }
      else {
        unequip.push_front(left_ring);
        left_ring = new_item;
      }
      break;
    case AMULET:
      unequip.push_front(amulet);
      amulet = new_item;
      break;
    case UNEQUIPABLE:
      return unequip;
  }
  return unequip;
}

Weapon * Gear::equip(Weapon * new_weapon) {
  Weapon * temp = weapon;
  weapon = new_weapon;
  return temp;
}

Item * Gear::unequip(int type) {
  Item * old_item;
  switch(type) {
    case HEAD:
      old_item = head;
      head = nullptr;
      break;
    case ARMS:
      old_item = arms;
      arms = nullptr;
      break;
    case BODY:
      old_item = body;
      body = nullptr;
      break;
    case LEGS:
      old_item = legs;
      legs = nullptr;
      break;
    case FULL_BODY:
      old_item = body;
      body = nullptr;
      break;
    case RING:
      if (right_ring == nullptr) {
        old_item = left_ring;
        left_ring = nullptr;
      }
      else {
        old_item = right_ring;
        right_ring = nullptr;
      }
      break;
    case AMULET:
      old_item = amulet;
      amulet = nullptr;
      break;
  }
  return old_item;
}

Weapon * Gear::unequipWeapon() {
  Weapon * temp = weapon;
  weapon = nullptr;
  return temp;
}

Item * Gear::getHead() {return head;}
Item * Gear::getArms() {return arms;}
Item * Gear::getLegs() {return legs;}
Item * Gear::getBody() {return body;}
Item * Gear::getLeft_ring() {return left_ring;}
Item * Gear::getRight_ring() {return right_ring;}
Item * Gear::getAmulet() {return amulet;}
Weapon * Gear::getWeapon() {return weapon;}
