#include "events.hpp"
#include "item.hpp"
#include "order.hpp"
#include "factory.hpp"
#include "hash.hpp"

class Factorio_game;

#include "nlohmann/json.hpp"
#include <list>
#include <queue>

using json = nlohmann::json;

class Event_generator{
public:

  Event_generator(Factorio_game&, json);
  ~Event_generator();

  void find_work(Factory*);
  void generate_events();
  void print_events();


  //std::unordered_map<Crafting_category, std::vector<Factory*>, Hash> factories_by_crafting_category;
  std::unordered_map<std::string, Factory*>& factories_blueprint;
  std::deque<Factory*> starved_factories;
  std::unordered_map<int, Factory*> factories;
  int next_factory_index = 0;
  std::unordered_map<std::string, Item*>& items_blueprint;
  std::vector<std::list<Order>>& build_order;
  std::priority_queue<Stop_factory_event, std::vector<Stop_factory_event>, std::greater<Stop_factory_event>> future_events;
  std::list<Event*> events;
  long long int time = 0;
  json output = json::array();

};
