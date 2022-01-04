#pragma once

class Crafting_category;
#include "recipe.hpp"
#include "item.hpp"


class Order{
public:
  Order(Item*, double, Recipe*);
  Order(Item*, double, Recipe*, Order*);
  Order();

  Item* item;
  double quantity;
  Recipe* recipe;
  int ingredients_still_needed = 0;
  Order* purpose = nullptr;
};
