#include "event_generator.hpp"
#include "technology.hpp"
#include "factorio_game.hpp"
#include <cmath>


Event_generator::Event_generator(Factorio_game& factorio, json challenge):
build_order(factorio.build_order),
build_order_by_factories(factorio.build_order_by_factories),
items_blueprint(factorio.items_blueprint),
factories_blueprint(factorio.factories_blueprint){
  for (json::iterator i = challenge["initial-factories"].begin(); i != challenge["initial-factories"].end(); ++i){
    Factory* new_factory = new Factory((*i)["factory-type"], (*i)["factory-id"], (*i)["factory-name"], factories_blueprint);
    factories[(*i)["factory-id"]] = new_factory;
    next_factory_index++;

  }
  for (json::iterator i = challenge["initial-items"].begin(); i != challenge["initial-items"].end(); ++i){
    items_blueprint[(*i)["name"]]->stock = (*i)["amount"];
    if (factories_blueprint.contains((*i)["name"])){
      for (int j = 0; j < (*i)["amount"]; ++j){
        Factory* new_factory = new Factory((*i)["name"], next_factory_index, "dummy-name", factories_blueprint);
        factories[next_factory_index++] = new_factory;
      }
    }
  }
}


Event_generator::~Event_generator(){
  for (auto& i : factories){
    delete i.second;
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
    Factory* factory = factories[event.factory_id];
    if (factories[event.factory_id]->destroyed){
      factories.erase(event.factory_id);
      delete factory;
      continue;
    }
    if (factories[event.factory_id]->current_job.purpose != nullptr){
      --factories[event.factory_id]->current_job.purpose->ingredients_still_needed;
    }
    time = event.timestamp;
    Order current_job = factories[event.factory_id]->current_job;
    int overshoot = current_job.recipe.second * (time - factory->time_job_started) / (factory->crafting_speed * current_job.recipe.first->energy) - current_job.quantity; //keine ahnung obs hier rundungsfehler gibt
    current_job.item->stock += overshoot;
    output += event;
//std::cout << event << std::endl;

    if (factories[event.factory_id]->current_job.item->type == "factory" && factories[event.factory_id]->current_job.item->name != "electric-furnace"){
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
      find_work(starved_factories.front());
      starved_factories.pop_front();
    }
  }

  output += Victory_event(time);

/*for (Factory* factory : factories){
std::cerr << factory << std::endl;
}*/
}


void Event_generator::find_work(Factory* factory){
  if (factory->build_order_index == -1){

  }
  if (!build_order_by_factories[0].empty()){
    if((*build_order_by_factories[0].front()).ingredients_still_needed == 0){
      Order& order = *build_order_by_factories[0].front();
      output += Research_event(time, order.item->name);
      Technology* technology = static_cast<Technology*>(order.item);
      technology->researched = true;
      for (Recipe* effect : technology->effects){
        effect->enabled = true;
      }
      for (std::list<std::list<Order>::iterator>::iterator iterator = build_order_by_factories[0].begin(); iterator != build_order_by_factories[0].end(); ++iterator){
        Technology* advanced_technology = static_cast<Technology*>((**iterator).item);
        for (Technology* prerequisite : advanced_technology->prerequisites){
          if (prerequisite == technology){
            --(**iterator).ingredients_still_needed;
          }
        }
      }
      build_order.erase(build_order_by_factories[0].front());
      build_order_by_factories[0].pop_front();
      find_work(factory);
      return;
    }
  }

  for (std::list<std::list<Order>::iterator>::iterator iterator = build_order_by_factories[factory->build_order_index].begin(); iterator != build_order_by_factories[factory->build_order_index].end(); ++iterator){
    Order& order = **iterator;
    if (order.ingredients_still_needed == 0){
      if (order.item->stock > order.quantity){
        --order.purpose->ingredients_still_needed;
        order.item->stock -= order.quantity;
        build_order.erase(*iterator);
        build_order_by_factories[factory->build_order_index].erase(iterator);
        find_work(factory);
        return;

      }
      if (order.recipe.first->enabled){
        bool recipe_can_be_executed = false;
        for (Crafting_category& category : factory->crafting_categories){
          if (category == order.recipe.first->crafting_category){
            recipe_can_be_executed = true;
            break;
          }
        }
        if (recipe_can_be_executed){  //hier muessen noch die factories destroyed werden
          for (std::pair<Item*, int>& ingredient : order.recipe.first->ingredients){
            //if (factories_blueprint.contains(ingredient.first->name)){
            if (ingredient.first->type == "factory" && ingredient.first->name != "electric-furnace"){
              for (int i = 0; i < ingredient.second * std::ceil(order.quantity/order.recipe.second); ++i){
                for (auto& j : factories){
                  Factory* ingredient_factory = j.second;
                  int factory_id;
                  if (ingredient.first->name == ingredient_factory->name && ingredient_factory != factory && !ingredient_factory->destroyed){
                    ingredient_factory->destroyed = true;
                    int ingredient_factory_id = ingredient_factory->id;
                    if (ingredient_factory->starved){
                      for (std::deque<Factory*>::iterator starved_factory = starved_factories.begin(); starved_factory != starved_factories.end(); ++starved_factory){
                        if ((*starved_factory)->id == ingredient_factory->id){
                          starved_factories.erase(starved_factory);
                          factories.erase(ingredient_factory_id);
                          delete ingredient_factory;
                          break;
                        }
                      }
                    }
                    else {
                      ingredient_factory->shrink_job(time);
                      build_order.push_front(ingredient_factory->current_job);
                      build_order_by_factories[ingredient_factory->build_order_index].push_front(build_order.begin());
                      output += Stop_factory_event(time, ingredient_factory->id);
//std::cerr << Stop_factory_event(time, ingredient_factory->id) << std::endl;
                    }
                    output += Destroy_factory_event(time, ingredient_factory_id);//hier muss noch die Factory aus dem Vector geloescht werden und deleted werden

//std::cerr << Destroy_factory_event(time, ingredient_factory->id) << std::endl;
                    break;

                  }
                }

              }
            }
          }
          factory->current_job = order;
          factory->time_job_started = time;
          output += Start_factory_event(time, factory->id, order.recipe.first->name);
//std::cerr << Start_factory_event(time, factory_p->id, order.recipe->name) << std::endl;
          long long int time_finished = time + static_cast<int>(std::ceil(order.quantity/order.recipe.second)  * std::ceil(order.recipe.first->energy / factory->crafting_speed)) ;  //nochmal drueber nachdenken!
          future_events.push(Stop_factory_event(time_finished, factory->id));
          build_order.erase(*iterator);
          build_order_by_factories[factory->build_order_index].erase(iterator);
          return;
        }
      }
    }
  }
  factory->starved = true;
  starved_factories.push_back(factory);


}



/*void Event_generator::find_work(Factory* factory_p){
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
        else if (order.item->type == "item" || "factory"){
          if (order.item->stock > order.quantity){
            --order.purpose->ingredients_still_needed;
            order.item->stock -= order.quantity;
            list.erase(iterator);
            find_work(factory_p);
            return;

          }
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
                      int factory_id;
                      if (ingredient.first->name == ingredient_factory->name && ingredient_factory != factory_p && !ingredient_factory->destroyed){
                        ingredient_factory->destroyed = true;
                        int ingredient_factory_id = ingredient_factory->id;
                        if (ingredient_factory->starved){
                          for (std::deque<Factory*>::iterator starved_factory = starved_factories.begin(); starved_factory != starved_factories.end(); ++starved_factory){
                            if ((*starved_factory)->id == ingredient_factory->id){
                              starved_factories.erase(starved_factory);
                              factories.erase(ingredient_factory_id);
                              delete ingredient_factory;
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
                        output += Destroy_factory_event(time, ingredient_factory_id);//hier muss noch die Factory aus dem Vector geloescht werden und deleted werden

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
}*/

void Event_generator::print_events(){
  std::cout << std::setw(2) << output << std::endl;
}
