#include "event_generator.hpp"
#include "technology.hpp"
#include "factorio_game.hpp"
#include <cmath>


Event_generator::Event_generator(Factorio_game& factorio, json challenge):
build_order(factorio.build_order),
items_blueprint(factorio.items_blueprint),
factories_blueprint(factorio.factories_blueprint){
  for (json::iterator i = challenge["initial-factories"].begin(); i != challenge["initial-factories"].end(); ++i){
    Factory* new_factory = new Factory((*i)["factory-type"], (*i)["factory-id"], (*i)["factory-name"], factories_blueprint);

    //factories.push_back(new_factory);
    factories[(*i)["factory-id"]] = new_factory;
    next_factory_index++;

  }
  for (json::iterator i = challenge["initial-items"].begin(); i != challenge["initial-items"].end(); ++i){
    items_blueprint[(*i)["name"]]->stock = (*i)["amount"];
    if (factories_blueprint.contains((*i)["name"])){
      for (int j = 0; j < (*i)["amount"]; ++j){
        Factory* new_factory = new Factory((*i)["name"], next_factory_index, "dummy-name", factories_blueprint);
        //factories.push_back();
        factories[next_factory_index++] = new_factory;
      }
    }
  }
}

void Event_generator::generate_events(){
  for (auto& i : factories){
    Factory* factory = i.second;
    find_work(factory);
  }

  while(!future_events.empty()){
    Stop_factory_event event = future_events.top();
    future_events.pop();
    if (factories[event.factory_id]->destroyed){
      continue;
    }
    if (factories[event.factory_id]->current_job.purpose != nullptr){
      --factories[event.factory_id]->current_job.purpose->ingredients_still_needed;
    }
    output += event;
    //std::cout << event << std::endl;
    time = event.timestamp;

    if (factories[event.factory_id]->current_job.item->type == "factory"){
      for (int i = 0; i < factories[event.factory_id]->current_job.quantity; ++i){
        Factory* new_factory = new Factory(factories[event.factory_id]->current_job.item->name, next_factory_index, "dummy-factory-name", factories_blueprint);
        //factories.push_back(new_factory);
        factories[next_factory_index++] = new_factory;
        output += Build_factory_event(time, new_factory->id, new_factory->name, new_factory->factory_name);
//std::cerr << Build_factory_event(time, new_factory->id, new_factory->name, new_factory->factory_name) <<std::endl;
        find_work(new_factory);
      }
    }

    find_work(factories[event.factory_id]);
    for (int i = 0; i < starved_factories.size(); ++i){
      if (!starved_factories.front()->destroyed){
        starved_factories.front()->starved = false;
        find_work(starved_factories.front());
      }
      starved_factories.pop_front();
    }
  }

  output += Victory_event(time);

/*for (Factory* factory : factories){
std::cerr << factory << std::endl;
}*/
}



void Event_generator::find_work(Factory* factory_p){
  for (std::list<Order>& list : build_order){
    for (std::list<Order>::iterator iterator = list.begin(); iterator != list.end(); ++iterator){
      Order order = *iterator;
      if (order.ingredients_still_needed == 0){
        if (order.item->type == "technology"){
          output += Research_event(time, order.item->name);
//std::cerr << *order.item << std::endl;
//std::cerr << Research_event(time, order.item->name);
          Technology* technology_p = static_cast<Technology*>(order.item);
          technology_p->researched = true;
          for (Recipe* effect : technology_p->effects){
            effect->enabled = true;
          }
          for (std::list<Order>& list2 : build_order ) {
            for (std::list<Order>::iterator iterator2 = list2.begin(); iterator2 != list2.end(); ++iterator2){
              Order& advanced_order = *iterator2;
              if (advanced_order.item->type == "technology"){
                Technology* advanced_technology_p = static_cast<Technology*>(advanced_order.item);
                for (Technology* prerequisite : advanced_technology_p->prerequisites){
                  if (technology_p == prerequisite){
                    --advanced_order.ingredients_still_needed;
                    break;
                  }
                }
              }
            }
          }

          list.erase(iterator);
          find_work(factory_p);
          return;

        }
        else if (order.item->type == "item" || "factory"){ //noch falsch
          if (order.recipe.first->enabled){
            bool recipe_can_be_executed = false;
            for (Crafting_category& category : factory_p->crafting_categories){
              if (category == order.recipe.first->crafting_category){
                recipe_can_be_executed = true;
                break;
              }
            }
            if (recipe_can_be_executed){  //hier muessen noch die factories destroyed werden
              for (std::pair<Item*, int>& ingredient : order.recipe.first->ingredients){
                //if (factories_blueprint.contains(ingredient.first->name)){
                if (ingredient.first->type == "factory"){
                  for (int i = 0; i < ingredient.second * std::ceil(order.quantity/order.recipe.second); ++i){
                    for (auto& j : factories){
                      Factory* ingredient_factory = j.second;
                      if (ingredient.first->name == ingredient_factory->name && ingredient_factory != factory_p && !ingredient_factory->destroyed){
                        ingredient_factory->destroyed = true;
                        if (ingredient_factory->starved){
                          for (std::deque<Factory*>::iterator starved_factory = starved_factories.begin(); starved_factory != starved_factories.end(); ++starved_factory){
                            if ((*starved_factory)->id == ingredient_factory->id){
                              starved_factories.erase(starved_factory);
                              break;
                            }
                          }
                        }
                        else {
                          ingredient_factory->shrink_job(time);
                          build_order[0].push_front(ingredient_factory->current_job);
                          output += Stop_factory_event(time, ingredient_factory->id);
//std::cerr << Stop_factory_event(time, ingredient_factory->id) << std::endl;
                        }
                        output += Destroy_factory_event(time, ingredient_factory->id);//hier muss noch die Factory aus dem Vector geloescht werden und deleted werden

//std::cerr << Destroy_factory_event(time, ingredient_factory->id) << std::endl;
                        break;

                      }
                    }

                  }
                }
              }
              factory_p->current_job = order;
              factory_p->time_job_started = time;
              list.erase(iterator);
              output += Start_factory_event(time, factory_p->id, order.recipe.first->name);
//std::cerr << Start_factory_event(time, factory_p->id, order.recipe->name) << std::endl;
              long long int time_finished = time + static_cast<int>(std::ceil(std::ceil(order.quantity/order.recipe.second)  * order.recipe.first->energy / factory_p->crafting_speed)) ;
              future_events.push(Stop_factory_event(time_finished, factory_p->id));
              return;
            }
          }
        }
      }
    }
  }
  factory_p->starved = true;
  starved_factories.push_back(factory_p);
//output += Stop_factory_event(time, factory_p->id);
//factory_p->do_default_job()
}

void Event_generator::print_events(){
  std::cout << std::setw(2) << output << std::endl;
}
