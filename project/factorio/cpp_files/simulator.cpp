# include "simulator.hpp"
#include <cmath>

Simulator::Simulator(const std::string& challenge):
  items_blueprint(factorio.items_blueprint),
  factories_blueprint(factorio.factories_blueprint),
  recipes_blueprint(factorio.recipes_blueprint),
  technologies_blueprint(factorio.technologies_blueprint),
  build_order(factorio.build_order),
  build_order_by_factories(factorio.build_order_by_factories)
  //event_generator(factorio, initial)
  {
//printItems(std::cerr);
  std::ifstream in(challenge.c_str());
  in >> initial;
  //event_generator = event_generator(factorio, initial);

  for (json::iterator i = initial["initial-factories"].begin(); i != initial["initial-factories"].end(); ++i){
    items_blueprint[(*i)["factory-type"]]->stock = 1; // fehleranfaellig bessser ++
    //factories.push_back(Factory((*i)["factory-type"], (*i)["factory-id"], (*i)["factory-name"], factories_blueprint));
    Factory*  new_factory = new Factory((*i)["factory-type"], (*i)["factory-id"], (*i)["factory-name"], factories_blueprint);
    factories[(*i)["factory-id"]] = new_factory;
    initial_factories.push_back(*new_factory);
    ++next_factory_index;
  }

  /*for (json::iterator i = initial["initial-items"].begin(); i != initial["initial-items"].end(); ++i){
    items_blueprint[(*i)["name"]]->stock = (*i)["amount"];
  }*/


  for (Factory factory : initial_factories){
    for (Crafting_category& category : factory.crafting_categories){
      available_categories.insert(category);
    }
  }


  for (auto& i : items_blueprint){
    Item* item_p = i.second;
    item_p->calculate_energy(available_categories);
  }

  for (json::iterator i = initial["goal-items"].begin(); i != initial["goal-items"].end(); ++i){
    goal.push_back(std::pair<Item*, int>(items_blueprint[(*i)["name"]], (*i)["amount"]));
    //goal.push_back(std::pair<Item, int>(Item((*i)["name"], "item"), (*i)["amount"]));
  }

  //std::list<Order>::iterator iterator = build_order.end();
  factories_to_build_in_advance.push_back(Order(items_blueprint["stone-furnace"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr));
  factories_to_build_in_advance.push_back(Order(items_blueprint["burner-mining-drill"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr));
  factories_to_build_in_advance.push_back(Order(items_blueprint["assembling-machine-1"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr));
  items_and_insert_iterators[0] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["stone-furnace"], factories_to_build_in_advance.end());
  items_and_insert_iterators[1] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["burner-mining-drill"], factories_to_build_in_advance.end());
  items_and_insert_iterators[2] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["assembling-machine-1"], factories_to_build_in_advance.end());

  factories_to_build_in_advance.push_back(Order(items_blueprint["assembling-machine-2"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr));
  factories_to_build_in_advance.push_back(Order(items_blueprint["offshore-pump"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr));
  factories_to_build_in_advance.push_back(Order(items_blueprint["pumpjack"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr));
  factories_to_build_in_advance.push_back(Order(items_blueprint["oil-refinery"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr));
  factories_to_build_in_advance.push_back(Order(items_blueprint["chemical-plant"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr));
  items_and_insert_iterators[3] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["assembling-machine-2"], factories_to_build_in_advance.end());
  items_and_insert_iterators[4] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["offshore-pump"], factories_to_build_in_advance.end());
  items_and_insert_iterators[5] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["pumpjack"], factories_to_build_in_advance.end());
  items_and_insert_iterators[6] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["oil-refinery"], factories_to_build_in_advance.end());
  items_and_insert_iterators[7] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["chemical-plant"], factories_to_build_in_advance.end());

  factories_to_build_in_advance.push_back(Order(items_blueprint["rocket-silo"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr));
  items_and_insert_iterators[8] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["rocket-silo"], factories_to_build_in_advance.end());

//printItems(std::cerr);


  /*
  std::cout << "factories:\n" << std::endl;
  for (Factory i : factories){
    std::cout << i << std::endl;
  }

  std::cout << "items:\n" << std::endl;
  for (std::pair<Item, int> i : goal){
    std::cout << i.first << i.second << std::endl;
  }
  */


}



void Simulator::build_items(std::list<Order>& factories_to_build){

  for (Order& factory_order : factories_to_build){
    std::list<Order>::iterator iterator = build_order.end();
    process_order(factory_order, build_order, iterator);
  }

  for (auto& i : goal){
//build_order.push_back(std::list<Order>());
    Item* item_p = i.first;
    int ordered_amount = i.second;
    std::list<Order>::iterator iterator = build_order.end();
    process_order(Order(item_p, ordered_amount, std::pair<Recipe*, int>(nullptr, 0), nullptr), build_order, iterator);
//std::cout << *i.first << std::endl;
  }
}


void Simulator::process_order(Order order, std::list<Order>& list, std::list<Order>::iterator& iterator){
  Item* item_p = order.item;
  double amount_ordered = order.quantity;

  if (item_p->type == "item" || item_p->type == "factory"){// wahrscheinlich unnoetig
    std::pair<Recipe*, int> recipe_p = item_p->best_recipe;
    if (recipe_p.first == nullptr){
      unlock_missing_ingredient(item_p, list, iterator);
    }
    recipe_p = item_p->best_recipe;
/*if (item_p->name == "spidertron"){
printItems(std::cerr);
}*/
/*if (recipe_p.first == nullptr){
std::cerr << item_p->name <<std::endl;
}*/
    if (item_p->name == "coal" && amount_ordered == 1){
      recipe_p.first = recipes_blueprint["coal"];
      recipe_p.second = 1;
    }
    list.insert(iterator, Order(item_p, amount_ordered, recipe_p, order.purpose));
    --iterator;
    Order* purpose = &(*iterator);
    int amount_produced = recipe_p.second;
    for (auto& ingredient : recipe_p.first->ingredients){
      Item* item_ingredient_p = ingredient.first;
      int amount_consumed = ingredient.second;
      process_order(Order(item_ingredient_p, std::ceil(amount_ordered/amount_produced) * amount_consumed, std::pair<Recipe*, int>(nullptr,0), purpose), list, iterator);
    }
    if (item_p->type == "factory"){
      bool new_category = false;
      for (auto category : factories_blueprint[item_p->name]->crafting_categories){
        new_category += available_categories.insert(category).second;
      }
      if (new_category){
        for (auto& string_item_p_pair : items_blueprint){
//std::cerr << string_item_p_pair.second->name << std::endl;
          string_item_p_pair.second->calculate_energy(available_categories);

        }
      }
    }
  }

}

void Simulator::unlock_missing_ingredient(Item* item_p ,std::list<Order>& list, std::list<Order>::iterator iterator){
  std::pair<Recipe*, int> recipe_p = item_p->best_recipe;
  if (recipe_p.first == nullptr){
    if (!item_p->recipes_produce[0].first->enabled){
      research_Technology(item_p->recipes_produce[0].first->technology_for_unlock, list, iterator);
    }
  }
  recipe_p = item_p->best_recipe;
  if (recipe_p.first == nullptr){
    for (auto& i : item_p->recipes_produce[0].first->ingredients){
      Item* ingredient = i.first;
      if (ingredient->energy == -1){
        unlock_missing_ingredient(ingredient, list, iterator);
      }
    }
  }
}


void Simulator::research_Technology(Technology* technology_p, std::list<Order>& list, std::list<Order>::iterator iterator){
  for (Technology* prerequisite : technology_p->prerequisites){
    if (!prerequisite->researched){
      research_Technology(prerequisite, list, iterator);
    }
  }
//std::cerr << technology_p->name << std::endl;
//printItems(std::cerr);
  list.insert(iterator, Order(technology_p, 1, std::pair<Recipe*, int>(nullptr, 0), nullptr));
  --iterator;
  Order* purpose = &(*iterator);
  for (auto& i : technology_p->ingredients){
    process_order(Order(i.first, i.second, std::pair<Recipe*, int>(nullptr,0), purpose), list, iterator);
  }
//std::cerr << *technology_p << std::endl;
  technology_p->researched = true;
  for (Recipe* effect : technology_p->effects){
    effect->enabled = true;
    for (auto& i : effect->products){
      Item* item_p = i.first;
      item_p->calculate_energy(available_categories);
    }
  }

}


void Simulator::sort_Orders_by_factories(){

    for (std::list<Order>::iterator pos = build_order.begin(); pos != build_order.end(); ++pos){
      if ((*pos).item->type == "technology"){
                                                      build_order_by_factories[0].push_back(pos);
      }
      else{
        const std::string& category = (*pos).recipe.first->crafting_category.name;
        if (category == "wood-crafting" || category == "basic-solid" || category == "crafting" || category == "basic-crafting" || category == "advanced-crafting")
                                                      build_order_by_factories[1].push_back(pos);
        else if (category == "crafting-with-fluid")   build_order_by_factories[2].push_back(pos);
        else if (category == "oil-processing")        build_order_by_factories[3].push_back(pos);
        else if (category == "chemistry")             build_order_by_factories[4].push_back(pos);
        else if (category == "rocket-building")       build_order_by_factories[5].push_back(pos);
        else if (category == "basic-smelting")        build_order_by_factories[6].push_back(pos);
        else if (category == "burner-solid")          build_order_by_factories[7].push_back(pos);
        else if (category == "basic-fluid")           build_order_by_factories[8].push_back(pos);
        else if (category == "water-pump")            build_order_by_factories[9].push_back(pos);
      }
    }

}


void Simulator::optimize(){
  build_items(factories_to_build_in_advance);
  restore_original_state();
  int bestmark = generate_events();
  restore_original_state();

  std::list<Order> factories_to_build_variant;
  std::list<Order> factories_to_build_best =  factories_to_build_in_advance;
  int run = 0;
  bool progress = true;
  while (run < 100 && progress){
    progress = false;
    for (auto& item_and_insert_iterator : items_and_insert_iterators){

      factories_to_build_variant = factories_to_build_in_advance;
      factories_to_build_variant.insert(item_and_insert_iterator.second, Order(item_and_insert_iterator.first, 1 , std::pair<Recipe*, int>(nullptr, 0), nullptr));
      build_items(factories_to_build_variant);
      restore_original_state();
      int time = generate_events();
      restore_original_state();
      if (time < bestmark){
        progress = true;
        factories_to_build_best = factories_to_build_variant;
        bestmark = time;
      }
    }
    factories_to_build_in_advance = factories_to_build_best;

  }
  build_items(factories_to_build_in_advance);
  restore_original_state();
  generate_events();

}


void Simulator::restore_original_state(){
  for (auto& i : recipes_blueprint){
    Recipe*  recipe_p = i.second;
    recipe_p->enabled = recipe_p->enabled_at_begin;
  }

  for (auto& i : technologies_blueprint){
    Technology* technology_p = i.second;
    technology_p->researched = false;
  }

  for (auto& i : items_blueprint){
    Item* item_p = i.second;
    item_p->stock = 0;
    item_p->energy = -1;
    item_p->best_recipe = std::pair<Recipe*, int>(nullptr, 0);
  }

  available_categories.clear(); //hier muessen noch die initial_factories wieder eingefuegt werden

  for (auto& i : factories){
    int index = i.first;
    Factory* factory = i.second;
    delete factory;
  }

  factories.clear();
  next_factory_index = 0;

  for (Factory& factory : initial_factories){
    for (Crafting_category& category : factory.crafting_categories){
      available_categories.insert(category);
    }
    factories[factory.id] = new Factory(factory);
    ++next_factory_index;
  }

  for (auto& i : items_blueprint){
    Item* item_p = i.second;
    item_p->calculate_energy(available_categories);
  }
}


int Simulator::generate_events(){
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
  return time;
/*for (Factory* factory : factories){
std::cerr << factory << std::endl;
}*/
}


void Simulator::find_work(Factory* factory){
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


void Simulator::print_events(){
  std::cout << std::setw(2) << output << std::endl;
}


void Simulator::printbuild_order(std::ostream& out){
    for (auto& order : build_order){
      out << *order.item << std::endl << order.quantity << std::endl;
      /*if (order.recipe != nullptr){
        Recipe* recipe_p = order.recipe;
        out << *recipe_p << std::endl;
      }
      out << "ingredients_still_needed: " << order.ingredients_still_needed << std::endl;
      if (order.purpose != nullptr){
        out <<  "purpose: ";
        out << *order.purpose->item <<std::endl;
      }*/
    }

}


void Simulator::printItems(std::ostream& out){
  out << "\nItems:\n" << std::endl;
  for (std::pair<std::string, Item*> i : items_blueprint){
    out<< i.first << std::endl << *i.second << std::endl;
  }
}


void Simulator::print_factories(std::ostream& out){
  out << "\nFactories: \n" << std::endl;
  for (std::pair<std::string, Factory*> i : factories_blueprint){
    out << *i.second << std::endl;
  }
}


void Simulator::print_technologies(std::ostream& out){
  for (auto& i : technologies_blueprint){
    out << *i.second << std::endl;
  }
}


//void Simulator::printRecipeDoubles(){
    /*for (json::iterator recipe1 = recipes.begin(); recipe1 != recipes.end(); ++recipe1){
    for (json::iterator recipe2 = recipe1 ; recipe2 != recipes.end(); ++recipe2){
      if (recipe1 != recipe2){
        for (json::iterator product1 = (*recipe1)["products"].begin(); product1 !=(*recipe1)["products"].end(); ++product1){
          for (json::iterator product2 = (*recipe2)["products"].begin(); product2 != (*recipe2)["products"].end(); ++product2){
            if ((*product1)["name"] == (*product2)["name"]){
              std::cout << "Product : " << (*product1)["name"] << "\nRecipe1: \n" << recipe1.key() << "\n" << (*recipe1) << "\nRecipe2: \n" <<  recipe2.key() << "\n" << (*recipe2) << "\n" << std::endl;
            }
          }
        }
      }
    }
  }*/
//}
