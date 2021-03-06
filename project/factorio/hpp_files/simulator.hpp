#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <list>
//#include <deque>
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
//#include "factorio_game.hpp"
#include "events.hpp"
//#include "event_generator.hpp"



using json = nlohmann::json;

class Simulator {
public:

  Simulator(const std::string&, int);
  ~Simulator() noexcept;

  void process_order(Order, std::list<Order>&, std::list<Order>::iterator&);
  void build_items();
  void research_Technology(Technology*, std::list<Order>&, std::list<Order>::iterator);
  void unlock_missing_ingredient(Item*, std::list<Order>&, std::list<Order>::iterator);
  void set_Indices();
  void sort_Orders_by_factories();

  void restore_original_state();

  bool find_work(Factory*, bool print_to_json = true);
  int generate_events(bool print_to_json = true);
  void print_events();

  void printbuild_order(std::ostream&);
  void printItems(std::ostream&);
  void print_factories(std::ostream&);
  void print_technologies(std::ostream&);
  void optimize();


  const int first_amount;
  std::unordered_map<std::string, Item*> items_blueprint;
  std::unordered_map<std::string, Factory*> factories_blueprint;
  std::unordered_map<std::string, Recipe*> recipes_blueprint;
  std::unordered_map<std::string, Technology*> technologies_blueprint;
  std::unordered_set<Item, Hash> available_categories;
  json initial;
  std::vector<Factory> initial_factories;
  std::unordered_map<int, Factory*> factories;
  int next_factory_index = 0;
  std::vector<std::pair<Item*, int>> goal;
  std::list<Order> build_order;
  //std::vector<std::list<std::list<Order>::iterator>> build_order_by_factories = std::vector<std::list<std::list<Order>::iterator>>(10);
  std::vector<std::priority_queue<std::list<Order>::iterator, std::vector<std::list<Order>::iterator>, MyCompare>> build_order_by_factories = std::vector<std::priority_queue<std::list<Order>::iterator, std::vector<std::list<Order>::iterator>, MyCompare>>(10);
  std::vector<std::pair<Item*, std::list<Order>::iterator>> items_and_insert_iterators = std::vector<std::pair<Item*, std::list<Order>::iterator>>(9);
  std::deque<Factory*> starved_factories;
  std::priority_queue<Stop_factory_event, std::vector<Stop_factory_event>, std::greater<Stop_factory_event>> future_events;
  int time = 0;
  json output = json::array();
  std::list<Order> factories_to_build_in_advance;
  int time_of_last_research_event = -1;
  std::unordered_set<Factory*> factories_all_jobs_done;
  std::vector<std::vector<std::pair<Item*, std::list<Order>::iterator>>> power_set;

};
