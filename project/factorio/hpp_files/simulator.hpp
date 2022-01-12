#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <list>
#include <deque>
#include <typeinfo>
#include <queue>

//class Factorio_game;

#include "nlohmann/json.hpp"
#include "item.hpp"
#include "factory.hpp"
#include "recipe.hpp"
#include "technology.hpp"
#include "hash.hpp"
#include "order.hpp"
#include "factorio_game.hpp"
#include "events.hpp"
//#include "event_generator.hpp"



using json = nlohmann::json;

class Simulator {
public:

  Simulator(const std::string&);

  //void buildItembuild_order();
  void process_order(Order, std::list<Order>&, std::list<Order>::iterator&);
  void build_items(std::list<Order>&);
  void research_Technology(Technology*, std::list<Order>&, std::list<Order>::iterator);
  void unlock_missing_ingredient(Item*, std::list<Order>&, std::list<Order>::iterator);
  void sort_Orders_by_factories();
  //void initialize_factories_to_build_in_advance();

  void restore_original_state();

  void find_work(Factory*);
  int generate_events();
  void print_events();

  void printbuild_order(std::ostream&);
  void printItems(std::ostream&);
  void print_factories(std::ostream&);
  void print_technologies(std::ostream&);
  void optimize();

  Factorio_game factorio;
  std::unordered_map<std::string, Item*>& items_blueprint;
  std::unordered_map<std::string, Factory*>& factories_blueprint;
  std::unordered_map<std::string, Recipe*>& recipes_blueprint;
  std::unordered_map<std::string, Technology*>& technologies_blueprint;
  std::unordered_set<Item, Hash> available_categories;
  json initial;
  std::vector<Factory> initial_factories;
  std::unordered_map<int, Factory*> factories;
  int next_factory_index = 0;
  std::vector<std::pair<Item*, int>> goal;
  std::list<Order>& build_order;
  std::vector<std::list<std::list<Order>::iterator>>& build_order_by_factories;
  //std::list<Order> build_order_current_best;
  std::vector<std::pair<Item*, std::list<Order>::iterator>> items_and_insert_iterators = std::vector<std::pair<Item*, std::list<Order>::iterator>>(9);
  //Event_generator event_generator;
  std::deque<Factory*> starved_factories;
  std::priority_queue<Stop_factory_event, std::vector<Stop_factory_event>, std::greater<Stop_factory_event>> future_events;
  std::list<Event*> events;
  long long int time = 0;
  json output = json::array();

  std::list<Order> factories_to_build_in_advance;

};
