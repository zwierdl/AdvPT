#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <list>
#include <deque>
#include <typeinfo>

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

  Simulator(const std::string);

  void buildItemBuildOrder();
  void process_order(Order, std::list<Order>&, std::list<Order>::iterator&);
  void build_items();
  void research_Technology(Technology*, std::list<Order>&, std::list<Order>::iterator);
  void unlock_missing_ingredient(Item*, std::list<Order>&, std::list<Order>::iterator);
  void restore_original_state();
  //void concartinate_identical_items();
  //void generate_events();
  //void calculate_energies();
  //void addTechnologies();
  //void apply_on_all_items(void(*function)(Item*));
  //void calculate_energy(Item*);

  //void process_technology(std::list::iterator, Technology*, std::list<Order>&){};
  void printBuildOrder(std::ostream&);
  void printItems(std::ostream&);
  void print_factories(std::ostream&);
  //void addTechnologies();
  std::unordered_map<std::string, Item*> items_blueprint;
  std::unordered_map<std::string, Recipe*> recipes_blueprint;
  std::unordered_map<std::string, Factory*> factories_blueprint;
  std::unordered_map<std::string, Technology*> technologies_blueprint;
  //std::unordered_multimap<std::string, std::pair<Recipe, int>> recipes_by_item;
  std::unordered_set<Item, Hash> available_categories;

  json initial;
  std::vector<Factory> factories;
  std::vector<std::pair<Item*, int>> goal;
  std::vector<std::list<Order>> buildOrder;
  //std::vector<std::list<Order>> buildOrder2;
  //std::deque<std::tuple<Item, int, Item&>> basicBuildOrder;
};
