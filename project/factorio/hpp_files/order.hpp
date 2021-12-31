#pragma once

class Crafting_category;

#include "item.hpp"


class Order{
public:
  Order(Item*, int, Item*);
  Item* item;
  int quantity;
  Item* purpose;
};
