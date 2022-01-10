#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <list>
#include <deque>
#include <typeinfo>

class Factorio_game;

#include "nlohmann/json.hpp"
#include "item.hpp"
#include "factory.hpp"
#include "recipe.hpp"
#include "technology.hpp"
#include "hash.hpp"
#include "order.hpp"



using json = nlohmann::json;

class Simulator {
public:

  Simulator(const std::string&, Factorio_game&);

  void buildItemBuildOrder();
  void process_order(Order, std::list<Order>&, std::list<Order>::iterator&);
  void build_items();
  void research_Technology(Technology*, std::list<Order>&, std::list<Order>::iterator);
  void unlock_missing_ingredient(Item*, std::list<Order>&, std::list<Order>::iterator);
  void sort_Orders_by_factories();
  void restore_original_state();
  void printBuildOrder(std::ostream&);
  void printItems(std::ostream&);
  void print_factories(std::ostream&);
  void print_technologies(std::ostream&);

  std::unordered_map<std::string, Item*>& items_blueprint;
  std::unordered_map<std::string, Factory*>& factories_blueprint;
  std::unordered_map<std::string, Recipe*>& recipes_blueprint;
  std::unordered_map<std::string, Technology*>& technologies_blueprint;
  std::unordered_set<Item, Hash> available_categories;
  json initial;
  std::vector<Factory> factories;
  std::vector<std::pair<Item*, int>> goal;
  std::list<Order>& buildOrder;
  std::vector<std::list<std::list<Order>::iterator>>& build_order_by_factories;

};
