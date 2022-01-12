#pragma once

#include <unordered_map>
#include <vector>
#include <list>

class Item;
class Factory;
class Recipe;
class Technology;
class Order;

class Factorio_game{
public:
  Factorio_game();
  ~Factorio_game();
  void restore_original_state();

  std::unordered_map<std::string, Item*> items_blueprint;
  std::unordered_map<std::string, Factory*> factories_blueprint;
  std::unordered_map<std::string, Recipe*> recipes_blueprint;
  std::unordered_map<std::string, Technology*> technologies_blueprint;
  std::list<Order> build_order;
  std::vector<std::list<std::list<Order>::iterator>> build_order_by_factories = std::vector<std::list<std::list<Order>::iterator>>(10);
};
