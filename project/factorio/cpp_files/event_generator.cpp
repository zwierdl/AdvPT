#include "event_generator.hpp"
#include "technology.hpp"

Event_generator::Event_generator(std::vector<std::list<Order>>& build_order, json& challenge, std::unordered_map<std::string, Factory*>& factories_blueprint, std::unordered_map<std::string, Item*>& items_blueprint):
build_order(build_order),
items(items_blueprint),
factories_blueprint(factories_blueprint){
  for (json::iterator i = challenge["initial-factories"].begin(); i != challenge["initial-factories"].end(); ++i){
    Factory* new_factory = new Factory((*i)["factory-type"], (*i)["factory-id"], (*i)["factory-name"], factories_blueprint);
    factories.push_back(new_factory);

    /*for (Crafting_category& category :new_factory->crafting_categories){
      factories_by_crafting_category[category].push_back(new_factory);
    }*/
  }
}

void Event_generator::generate_events(){
  for (Factory* factory_p : factories){
    find_work(factory_p);
  }

  while(!future_events.empty()){
    Stop_factory_event event = future_events.top();
    future_events.pop();
    if (factories[event.factory_id]->current_job.purpose != nullptr){
      --factories[event.factory_id]->current_job.purpose->ingredients_still_needed;
    }
    output += event;
    time = event.timestamp;

    if (factories[event.factory_id]->current_job.item->type == "factory"){
        Factory* new_factory = new Factory(factories[event.factory_id]->current_job.item->name, factories.size(), "dummy-factory-name", factories_blueprint);
        factories.push_back(new_factory);
        output += Build_factory_event(time, new_factory->id, new_factory->name, new_factory->factory_name);
        find_work(new_factory);
    }

    find_work(factories[event.factory_id]);
    for (int i = 0; i < starved_factories.size(); ++i){
      find_work(starved_factories.front());
      starved_factories.pop_front();
    }
    /*for (Factory* factory_p : starved_factories){
      find_work(factory_p);
      starved_factories.pop_front();
    }*/
  }
}



void Event_generator::find_work(Factory* factory_p){
  for (std::list<Order>& list : build_order){
    for (std::list<Order>::iterator iterator = list.begin(); iterator != list.end(); ++iterator){
      Order order = *iterator;
      if (order.ingredients_still_needed == 0){
        if (order.item->type == "technology"){
          output += Research_event(time, order.item->name);
          Technology* technology_p = static_cast<Technology*>(order.item);
          technology_p->researched == true;
          for (Recipe* effect : technology_p->effects){
            effect->enabled = true;
          }
          for (std::list<Order>& list2 : build_order ) {
            for (std::list<Order>::iterator iterator2 = list2.begin(); iterator2 != list2.end(); ++iterator2){
              Order& advanced_order = *iterator2;
              if (typeid(advanced_order.item) == typeid(Technology*)){
                Technology* advanced_technology_p = static_cast<Technology*>(order.item);
                for (Technology* prerequisite : advanced_technology_p->prerequisites){
                  if (technology_p == prerequisite){
                    --advanced_order.ingredients_still_needed;
                    break;
                  }
                }
              }
            }
          }

        }
        if (order.item->type == "item" || "factory"){ //noch falsch
          if (order.recipe->enabled){
            bool recipe_can_be_executed = false;
            for (Crafting_category& category : factory_p->crafting_categories){
              if (category == order.recipe->crafting_category){
                recipe_can_be_executed = true;
                break;
              }
            }
            if (recipe_can_be_executed){  //hier muessen noch die factories destroyed werden
              factory_p->current_job = order;
              list.erase(iterator);
              output += Start_factory_event(time, factory_p->id, order.recipe->name);
              long long int time_finished = time + static_cast<int>((static_cast<double>(order.quantity/order.recipe->products[0].second + 1) * order.recipe->energy) / factory_p->crafting_speed) +1; //stimmt noch nicht products[0] koennte falsch sein
              future_events.push(Stop_factory_event(time_finished, factory_p->id));
              return;
            }
          }
        }
      }
    }
  }
  starved_factories.push_back(factory_p);
  //output += Stop_factory_event(time, factory_p->id); hier ist noch ein Fehler!!!
  //factory_p->do_default_job()
}

void Event_generator::print_events(){
  std::cout << std::setw(2) << output << std::endl;
}