#pragma once

class Crafting_category;
#include "recipe.hpp"
#include "item.hpp"


class Order{
public:
  Order(Item*, int, Recipe*);
  Order(Item*, int, Recipe*, Order*);
  Order();

  Item* item;
  int quantity;
  Recipe* recipe;
  int ingredients_still_needed = 0;
  Order* purpose = nullptr;
};
