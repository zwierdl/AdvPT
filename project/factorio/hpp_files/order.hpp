#pragma once

#include <utility>

//class Crafting_category;
class Item;
class Recipe;


class Order{
public:
  Order(Item*, double, std::pair<Recipe*, int>);
  Order(Item*, double, std::pair<Recipe*, int>, Order*);
  Order();


  Item* item;
  double quantity;
  std::pair<Recipe*, int> recipe = std::pair<Recipe*, int>(nullptr, 0);
  int ingredients_still_needed = 0;
  Order* purpose = nullptr;
};
