#pragma once
#include <list>
#include <utility>

//class Crafting_category;
class Item;
class Recipe;


class Order{
public:
  Order(Item*, double, std::pair<Recipe*, int>);
  Order(Item*, double, std::pair<Recipe*, int>, std::list<Order>::iterator, bool establish_factory = false);
  Order();


  Item* item;
  double quantity;
  std::pair<Recipe*, int> recipe = std::pair<Recipe*, int>(nullptr, 0);
  int ingredients_still_needed = 0;
  std::list<Order>::iterator purpose;
  //Order* purpose = nullptr;
  bool establish_factory = false;
};
