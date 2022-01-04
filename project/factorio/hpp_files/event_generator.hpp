#include "events.hpp"
#include "item.hpp"
#include "order.hpp"
#include "factory.hpp"
#include "hash.hpp"

#include "nlohmann/json.hpp"
#include <list>
#include <queue>

using json = nlohmann::json;

class Event_generator{
public:

  Event_generator(std::vector<std::list<Order>>&, json&, std::unordered_map<std::string, Factory*>&, std::unordered_map<std::string, Item*>&);

  void find_work(Factory*);
  void generate_events();
  void print_events();


  //std::unordered_map<Crafting_category, std::vector<Factory*>, Hash> factories_by_crafting_category;
  std::unordered_map<std::string, Factory*>& factories_blueprint;
  std::deque<Factory*> starved_factories;
  std::vector<Factory*> factories;
  std::unordered_map<std::string, Item*>& items;
  std::vector<std::list<Order>>& build_order;
  std::priority_queue<Stop_factory_event, std::deque<Stop_factory_event>, std::greater<Stop_factory_event>> future_events;
  std::list<Event*> events;
  long long int time = 0;
  json output = json::array();

};
