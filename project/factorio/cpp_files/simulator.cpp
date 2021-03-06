# include "simulator.hpp"
#include <cmath>
#include <iomanip>
#include <chrono>
#include <stdexcept>
#include <cstdlib>
//#include <cassert>

Simulator::Simulator(const std::string& challenge, int first_amount):
first_amount(first_amount)
{

  std::ifstream in("./factorio-data/item.json");
  json myjson;
  in >> myjson;

  std::unordered_map<std::string,json> items_json_map = myjson;
  for (std::pair<std::string, json> i : items_json_map){
    items_blueprint[i.first] = new Item(i);
  }

  in.close();
  myjson.clear();


  in.open("./factorio-data/factory.json");
  in >> myjson;

  std::unordered_map<std::string, json> factories_json_map = myjson;
  for (std::pair<std::string, json> i : factories_json_map){
    factories_blueprint[i.first] =  new Factory(i);
  }
  for (std::pair<std::string, Factory*> i : factories_blueprint){
    if (items_blueprint.contains(i.first)){
      delete items_blueprint[i.first];
    }
    items_blueprint[i.first] = i.second;
  }

  myjson.clear();
  in.close();


  in.open("./factorio-data/recipe.json");
  in >> myjson;

  std::unordered_map<std::string, json> recipes_json_map = myjson;
  for (std::pair<std::string, json> i :recipes_json_map){
    bool barrel = false;
    for (json::iterator j = i.second["products"].begin(); j != i.second["products"].end(); ++j){
      if ((*j)["name"] == "empty-barrel") barrel = true;
    }
    if (!barrel) recipes_blueprint[i.first] =  new Recipe(i, items_blueprint);
  }

  myjson.clear();
  in.close();

  --recipes_blueprint["coal-burner"]->products[0].second;
  recipes_blueprint["coal-burner"]->ingredients.pop_back();

  for (auto& i : recipes_blueprint){
    Recipe* recipe = i.second;
    for (auto& product : recipe->products){
      Item* item = product.first;
      int amount = product.second;
      items_blueprint[item->name]->recipes_produce.push_back(std::pair<Recipe*, int>(recipe, amount));
    }

    for (auto& ingredient : recipe->ingredients){
      Item* item = ingredient.first;
      int amount = ingredient.second;
      items_blueprint[item->name]->recipes_consume.push_back(std::pair<Recipe*, int>(recipe, amount));
    }
  }


  in.open("./factorio-data/technology.json");
  in >> myjson;

  std::unordered_map<std::string, json> technologies_json_map = myjson;
  for (auto i : technologies_json_map){
    technologies_blueprint[i.first] = new Technology(i, items_blueprint, recipes_blueprint);
  }

  for(auto i : technologies_json_map){
    const std::string& name = i.first;
    json technology_json = i.second;
    for (json::iterator prerequsite_name = technology_json["prerequisites"].begin(); prerequsite_name != technology_json["prerequisites"].end(); ++prerequsite_name){
      technologies_blueprint[name]->prerequisites.push_back(technologies_blueprint[*prerequsite_name]);
    }
  }

  myjson.clear();
  in.close();

  for (auto& i : technologies_blueprint){
    Technology* technology = i.second;
    for (Recipe* recipe : technology->effects){
      recipes_blueprint[recipe->name]->technology_for_unlock = technology;
    }
  }


  in.open(challenge.c_str());
  in >> initial;

  for (json::iterator i = initial["initial-factories"].begin(); i != initial["initial-factories"].end(); ++i){
    //++items_blueprint[(*i)["factory-type"]]->stock;
    Factory*  new_factory = new Factory((*i)["factory-type"], (*i)["factory-id"], (*i)["factory-name"], factories_blueprint);
    factories[(*i)["factory-id"]] = new_factory;
    initial_factories.push_back(*new_factory);
    ++next_factory_index;
  }

  for (json::iterator i = initial["initial-items"].begin(); i != initial["initial-items"].end(); ++i){
    items_blueprint[(*i)["name"]]->stock = (*i)["amount"];
  }


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
  }

  factories_to_build_in_advance.push_back(Order(items_blueprint["stone-furnace"], 1, std::pair<Recipe*, int>(nullptr,0), build_order.end(), true));
  factories_to_build_in_advance.push_back(Order(items_blueprint["burner-mining-drill"], 1, std::pair<Recipe*, int>(nullptr,0), build_order.end(), true));

  factories_to_build_in_advance.push_back(Order(items_blueprint["assembling-machine-1"], 1, std::pair<Recipe*, int>(nullptr,0), build_order.end(), true));
  items_and_insert_iterators[2] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["assembling-machine-1"], --factories_to_build_in_advance.end());
  items_and_insert_iterators[1] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["burner-mining-drill"], --factories_to_build_in_advance.end());
  items_and_insert_iterators[0] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["stone-furnace"], --factories_to_build_in_advance.end());

  factories_to_build_in_advance.push_back(Order(items_blueprint["assembling-machine-2"], 1, std::pair<Recipe*, int>(nullptr,0), build_order.end(), true));
  factories_to_build_in_advance.push_back(Order(items_blueprint["offshore-pump"], 1, std::pair<Recipe*, int>(nullptr,0), build_order.end(), true));
  factories_to_build_in_advance.push_back(Order(items_blueprint["pumpjack"], 1, std::pair<Recipe*, int>(nullptr,0), build_order.end(), true));
  factories_to_build_in_advance.push_back(Order(items_blueprint["oil-refinery"], 1, std::pair<Recipe*, int>(nullptr,0), build_order.end(), true));
  factories_to_build_in_advance.push_back(Order(items_blueprint["chemical-plant"], 1, std::pair<Recipe*, int>(nullptr,0), build_order.end(), true));
  items_and_insert_iterators[3] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["chemical-plant"], --factories_to_build_in_advance.end());
  items_and_insert_iterators[4] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["oil-refinery"], --factories_to_build_in_advance.end());
  items_and_insert_iterators[5] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["pumpjack"], --factories_to_build_in_advance.end());
  items_and_insert_iterators[6] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["offshore-pump"], --factories_to_build_in_advance.end());
  items_and_insert_iterators[7] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["assembling-machine-2"], --factories_to_build_in_advance.end());
  //factories_to_build_in_advance.push_back(Order(technologies_blueprint["advanced-oil-processing"], 1, std::pair<Recipe*, int>(nullptr,0), build_order.end()));
  //factories_to_build_in_advance.push_back(Order(technologies_blueprint["rocket-silo"], 1, std::pair<Recipe*, int>(nullptr,0), build_order.end()));


  factories_to_build_in_advance.push_back(Order(items_blueprint["rocket-silo"], 1, std::pair<Recipe*, int>(nullptr,0), build_order.end(), true));
  items_and_insert_iterators[8] = std::pair<Item*, std::list<Order>::iterator>(items_blueprint["rocket-silo"], --factories_to_build_in_advance.end());




  power_set.push_back(std::vector<std::pair<Item*, std::list<Order>::iterator>>(0));

  for (int i = 0; i < items_and_insert_iterators.size(); i++){
    size_t power_set_size = power_set.size();
    for (int j = 0; j < power_set_size; ++j){
      power_set.push_back(power_set[j]);
      power_set[j].push_back(items_and_insert_iterators[i]);
    }
  }

  //power_set.erase(power_set.begin());

  for (std::vector<std::vector<std::pair<Item*, std::list<Order>::iterator>>>::const_reverse_iterator i = power_set.rbegin(); i != power_set.rend(); ++i){
    if ((*i).size() > 3 || (*i).size() <= 1){
      power_set.erase(i.base()-1);
    }
  }

//  factories_to_build_in_advance.push_back(Order(technologies_blueprint["chemical-science-pack"], 1, std::pair<Recipe*, int>(nullptr,0), build_order.end()));

}



Simulator::~Simulator(){
  for (auto& factory : factories){
    delete factory.second;
  }

  for (auto& i : recipes_blueprint){
    delete i.second;
  }
  for (auto& i : technologies_blueprint){
    delete i.second;
  }
  for (auto& i : items_blueprint){
    delete i.second;
  }
}



void Simulator::build_items(){

  for (Order& factory_order : factories_to_build_in_advance){
    std::list<Order>::iterator iterator = build_order.end();
    process_order(factory_order, build_order, iterator);
  }

  for (auto& i : goal){
//build_order.push_back(std::list<Order>());
    Item* item_p = i.first;
    int ordered_amount = i.second;
    std::list<Order>::iterator iterator = build_order.end();
    process_order(Order(item_p, ordered_amount, std::pair<Recipe*, int>(nullptr, 0), build_order.end()), build_order, iterator);
//std::cout << *i.first << std::endl;
  }
}


void Simulator::process_order(Order order, std::list<Order>& list, std::list<Order>::iterator& iterator){
  Item* item_p = order.item;
  double amount_ordered = order.quantity;

  if (item_p->type == "item" || item_p->type == "factory"){// wahrscheinlich unnoetig
    std::pair<Recipe*, int> recipe_p = item_p->best_recipe;
    if (recipe_p.first == nullptr){
      try {
        unlock_missing_ingredient(item_p, list, iterator);
      } catch (std::invalid_argument& e){
        std::cerr << "Cannot build " << e.what() << " with given factories" << std::endl;

        exit(EXIT_FAILURE);
      }
    }
    recipe_p = item_p->best_recipe;

    /*if (item_p->name == "coal" && ((amount_ordered == 1) || (amount_ordered == 2)) ){
      recipe_p.first = recipes_blueprint["coal"];
      recipe_p.second = 1;
    }*/

    /*if (!(item_p->type == "factory")){  //nur fuer die validator-goetter
      //amount_ordered += 1;
    }*/

    list.insert(iterator, Order(item_p, amount_ordered, recipe_p, order.purpose, order.establish_factory));

    --iterator;
    std::list<Order>::iterator purpose = iterator;
    int amount_produced = recipe_p.second;
    for (auto& ingredient : recipe_p.first->ingredients){
      Item* item_ingredient_p = ingredient.first;
      int amount_consumed = ingredient.second;
      process_order(Order(item_ingredient_p, (std::ceil((amount_ordered)/amount_produced))  * amount_consumed, std::pair<Recipe*, int>(nullptr,0), purpose), list, iterator);
    }
    if (item_p->name == "coal-burner"){
      process_order(Order(items_blueprint["coal"], amount_ordered, std::pair<Recipe*, int>(nullptr, 0), purpose), list, iterator);
    }
    //if (item_p->type == "factory"){
    if (order.establish_factory){
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
  else if (item_p->type == "technology"){
    research_Technology(static_cast<Technology*>(item_p), list, iterator);
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
  if (item_p->best_recipe.first == nullptr){
    throw std::invalid_argument(item_p->name);
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
  list.insert(iterator, Order(technology_p, 1, std::pair<Recipe*, int>(nullptr, 0), build_order.end()));
  --iterator;
  std::list<Order>::iterator purpose = iterator;
  for (auto& i : technology_p->ingredients){
    process_order(Order(i.first, i.second, std::pair<Recipe*, int>(nullptr,0), purpose), list, iterator);
  }
//std::cerr << technology_p->name << std::endl;
  technology_p->researched = true;
  for (Recipe* effect : technology_p->effects){
    effect->enabled = true;
    for (auto& i : effect->products){
      Item* item_p = i.first;
      item_p->calculate_energy(available_categories);
    }
  }

}

void Simulator::set_Indices(){
  int next_index = 0;
  for (std::list<Order>::iterator iterator = build_order.begin(); iterator != build_order.end(); ++iterator){
    (*iterator).index = next_index++;
  }
}


void Simulator::sort_Orders_by_factories(){

    for (std::list<Order>::iterator pos = build_order.begin(); pos != build_order.end(); ++pos){

      /*if ((*pos).item->type == "technology"){
                                                      build_order_by_factories[0].push(pos);
      }
      else if ((*pos).ingredients_still_needed == 0){*/
      if ((*pos).ingredients_still_needed == 0){
        const std::string& category = (*pos).recipe.first->crafting_category.name;
        if (category == "wood-crafting" || category == "basic-solid" || category == "crafting" || category == "basic-crafting" || category == "advanced-crafting")
                                                      build_order_by_factories[1].push(pos);
        else if (category == "crafting-with-fluid")   build_order_by_factories[2].push(pos);
        else if (category == "oil-processing")        build_order_by_factories[3].push(pos);
        else if (category == "chemistry")             build_order_by_factories[4].push(pos);
        else if (category == "rocket-building")       build_order_by_factories[5].push(pos);
        else if (category == "basic-smelting")        build_order_by_factories[6].push(pos);
        else if (category == "burner-solid")          build_order_by_factories[7].push(pos);
        else if (category == "basic-fluid")           build_order_by_factories[8].push(pos);
        else if (category == "water-pump")            build_order_by_factories[9].push(pos);
      }
    }

}


void Simulator::optimize(){
  build_items();
  set_Indices();
  restore_original_state();
  sort_Orders_by_factories();
  int bestmark = generate_events(false);
  //printbuild_order(std::cerr);
  restore_original_state();

  std::chrono::time_point<std::chrono::steady_clock> begin = std::chrono::steady_clock::now();
  std::chrono::time_point<std::chrono::steady_clock> right_now;
  std::chrono::duration<double> time_elapsed;
  bool progress = true;
  //for (int amount = 4; amount >=1 && time_elapsed.count() < 250; amount /= 2){
  for (int amount = first_amount; amount >= 1; amount /=2){
    //std::cerr << "amount: " << amount << std::endl;
    //std::cerr << "time: " << time_elapsed.count() << std::endl;
    progress = true;
    while(progress && time_elapsed.count() < 60){
      //progress = false;
      while (progress){
        progress = false;
        for (auto& item_and_insert_iterator : items_and_insert_iterators){
          factories_to_build_in_advance.insert(std::next(item_and_insert_iterator.second, 1), Order(item_and_insert_iterator.first, amount , std::pair<Recipe*, int>(nullptr, 0), build_order.end(), true));
          build_items();
          set_Indices();
          restore_original_state();
          sort_Orders_by_factories();
          int duration = generate_events(false);
          printbuild_order(std::cerr);
          restore_original_state();
          /*if (amount <= 4){
            std::cerr << "duration now: " << duration << ": " << amount << ":"  << item_and_insert_iterator.first->name << std::endl;
          }*/
          if (duration < bestmark){
            progress = true;
            bestmark = duration;
            //std::cerr <<  bestmark << ": " << amount << ": " << item_and_insert_iterator.first->name << std::endl;
            break;
          }
          else{
            factories_to_build_in_advance.erase(std::next(item_and_insert_iterator.second, 1));
          }
        }
      }

      for (int i = 0; i < power_set.size(); ++i){
        for (int j = 0; j < power_set[i].size(); ++j){
          factories_to_build_in_advance.insert(std::next(power_set[i][j].second, 1), Order(power_set[i][j].first, amount, std::pair<Recipe*, int>(nullptr, 0), build_order.end(), true));
        }
        build_items();
        set_Indices();
        restore_original_state();
        sort_Orders_by_factories();
        int duration = generate_events(false);
        printbuild_order(std::cerr);
        restore_original_state();
        /*if (amount <= 4){
          std::cerr << "duration now: " << duration << ": " << amount << ":"  << item_and_insert_iterator.first->name << std::endl;
        }*/
        if (duration < bestmark){
          progress = true;
          bestmark = duration;
          //std::cerr <<  bestmark << ": " << amount << ": ";
          for (int j = 0; j < power_set[i].size(); ++j){
            //std::cerr<< power_set[i][j].first->name << " ";
          }
          //std::cerr << std::endl;
          break;
        }
        else{
          for (int j = 0; j < power_set[i].size(); ++j){
            factories_to_build_in_advance.erase(std::next(power_set[i][j].second, 1));
          }
        }
      }

      /*
      factories_to_build_in_advance.insert(std::next(items_and_insert_iterators[0].second, 1), Order(items_and_insert_iterators[0].first, 1 , std::pair<Recipe*, int>(nullptr, 0), build_order.end(), true));
      factories_to_build_in_advance.insert(std::next(items_and_insert_iterators[1].second, 1), Order(items_and_insert_iterators[1].first, 1 , std::pair<Recipe*, int>(nullptr, 0), build_order.end(), true));
      factories_to_build_in_advance.insert(std::next(items_and_insert_iterators[2].second, 1), Order(items_and_insert_iterators[2].first, 1 , std::pair<Recipe*, int>(nullptr, 0), build_order.end(), true));
      factories_to_build_in_advance.insert(std::next(items_and_insert_iterators[3].second, 1), Order(items_and_insert_iterators[3].first, 1 , std::pair<Recipe*, int>(nullptr, 0), build_order.end(), true));
      factories_to_build_in_advance.insert(std::next(items_and_insert_iterators[4].second, 1), Order(items_and_insert_iterators[4].first, 1 , std::pair<Recipe*, int>(nullptr, 0), build_order.end(), true));
      factories_to_build_in_advance.insert(std::next(items_and_insert_iterators[5].second, 1), Order(items_and_insert_iterators[5].first, 1 , std::pair<Recipe*, int>(nullptr, 0), build_order.end(), true));
      factories_to_build_in_advance.insert(std::next(items_and_insert_iterators[6].second, 1), Order(items_and_insert_iterators[6].first, 1 , std::pair<Recipe*, int>(nullptr, 0), build_order.end(), true));
      factories_to_build_in_advance.insert(std::next(items_and_insert_iterators[7].second, 1), Order(items_and_insert_iterators[7].first, 1 , std::pair<Recipe*, int>(nullptr, 0), build_order.end(), true));

      build_items();
      set_Indices();
      restore_original_state();
      sort_Orders_by_factories();
      int duration = generate_events(false);
      printbuild_order(std::cerr);
      restore_original_state();

      if (duration < bestmark){
        progress = true;
        bestmark = duration;
        std::cerr <<  bestmark << ": " << amount << ": " << "alles" << std::endl;
      }
      else{
        factories_to_build_in_advance.erase(std::next(items_and_insert_iterators[7].second, 1));
        factories_to_build_in_advance.erase(std::next(items_and_insert_iterators[6].second, 1));
        factories_to_build_in_advance.erase(std::next(items_and_insert_iterators[5].second, 1));
        factories_to_build_in_advance.erase(std::next(items_and_insert_iterators[4].second, 1));
        factories_to_build_in_advance.erase(std::next(items_and_insert_iterators[3].second, 1));
        factories_to_build_in_advance.erase(std::next(items_and_insert_iterators[2].second, 1));
        factories_to_build_in_advance.erase(std::next(items_and_insert_iterators[1].second, 1));
        factories_to_build_in_advance.erase(std::next(items_and_insert_iterators[0].second, 1));
      }
      */

    right_now = std::chrono::steady_clock::now();
    time_elapsed = right_now - begin;
    }

  }
  build_items();
  set_Indices();
  restore_original_state();
  sort_Orders_by_factories();
  generate_events();
  printbuild_order(std::cerr);
  print_events();
  time_elapsed = right_now - begin;
  //std::cerr << "time: " << time_elapsed.count() << std::endl;

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

  starved_factories.clear();

  for (auto& i : items_blueprint){
    Item* item_p = i.second;
    item_p->calculate_energy(available_categories);
  }

  for(json::iterator i = initial["initial-items"].begin(); i != initial["initial-items"].end(); ++i){
    items_blueprint[(*i)["name"]]->stock = (*i)["amount"];
  }

  time = 0;
  output.clear();
}


int Simulator::generate_events(bool print_to_json){
  for (auto& i: factories_blueprint){
    Factory* f = i.second;
    for (int j = 0; j < f->stock; j++){
      Factory* new_factory = new Factory(f->name, next_factory_index, "dummy-factory-name", factories_blueprint);
      //factories.push_back(new_factory);
      factories[next_factory_index++] = new_factory;
      if (print_to_json){
        output += Build_factory_event(time, new_factory->id, new_factory->name, new_factory->factory_name);
      }
    }
  }
  for (auto& i : factories){
    Factory* factory = i.second;
    find_work(factory, print_to_json);
  }
  do{
    while(!future_events.empty()){
      /*if (build_order.front().item->name == "electronic-circuit"){
        std::cerr << "halt" << std::endl;
      }*/

      Stop_factory_event event = future_events.top();
      //std::cout << std::setw(2) << event << std::endl;
      future_events.pop();
      Factory* factory = factories[event.factory_id];
      if (factories[event.factory_id]->destroyed){//mittlerweile wahrscheinlich unnoetig
        factories.erase(event.factory_id);
        delete factory;
        continue;
      }
      if (factories[event.factory_id]->current_job.purpose != build_order.end()){
        std::list<Order>::iterator& purpose = factories[event.factory_id]->current_job.purpose;
        --(*purpose).ingredients_still_needed;
        if ((*purpose).ingredients_still_needed == 0){
          if ((*purpose).item->type == "technology"){
            build_order_by_factories[0].push(purpose);
          }
          else{
            const std::string& category = (*purpose).recipe.first->crafting_category.name;
            if (category == "wood-crafting" || category == "basic-solid" || category == "crafting" || category == "basic-crafting" || category == "advanced-crafting")
                                                        build_order_by_factories[1].push(purpose);
            else if (category == "crafting-with-fluid")   build_order_by_factories[2].push(purpose);
            else if (category == "oil-processing")        build_order_by_factories[3].push(purpose);
            else if (category == "chemistry")             build_order_by_factories[4].push(purpose);
            else if (category == "rocket-building")       build_order_by_factories[5].push(purpose);
            else if (category == "basic-smelting")        build_order_by_factories[6].push(purpose);
            else if (category == "burner-solid")          build_order_by_factories[7].push(purpose);
            else if (category == "basic-fluid")           build_order_by_factories[8].push(purpose);
            else if (category == "water-pump")            build_order_by_factories[9].push(purpose);
          }
          //build_order.erase(purpose);
        }
      }
      time = event.timestamp;
      Order current_job = factories[event.factory_id]->current_job;
      int overshoot = current_job.recipe.second * (time - factory->time_job_started) / static_cast<int>(std::ceil(current_job.recipe.first->energy / factory->crafting_speed)) - current_job.quantity; //keine ahnung obs hier rundungsfehler gibt
      //current_job.item->stock += overshoot;
      output += event;
//std::cout << event << std::endl;

      //if (factories[event.factory_id]->current_job.item->type == "factory" && factories[event.factory_id]->current_job.item->name != "electric-furnace"){
      if (factories[event.factory_id]->current_job.establish_factory){
        for (int i = 0; i < factories[event.factory_id]->current_job.quantity; ++i){
          Factory* new_factory = new Factory(factories[event.factory_id]->current_job.item->name, next_factory_index, "dummy-factory-name", factories_blueprint);
          //factories.push_back(new_factory);
          factories[next_factory_index++] = new_factory;
          if (print_to_json){
            output += Build_factory_event(time, new_factory->id, new_factory->name, new_factory->factory_name);
          }
//std::cerr << Build_factory_event(time, new_factory->id, new_factory->name, new_factory->factory_name) <<std::endl;
          find_work(new_factory, print_to_json);
        }
      }

      find_work(factories[event.factory_id]);

      /*if ((*build_order_by_factories[1].front()).item->name == "electronic-circuit"){
        std::cerr << "halt" << std::endl;
      }*/



      for (int i = 0; i < starved_factories.size(); ++i){
        starved_factories.front()->starved = false;
        find_work(starved_factories.front(), print_to_json);
        starved_factories.pop_front();
      }

    }
    bool progress = false;
    for (int i  = 0; i < starved_factories.size(); ++i){
      starved_factories.front()->starved = false;
      progress += find_work(starved_factories.front(), print_to_json);
      starved_factories.pop_front();
    }
    if (!progress){
      time += 1;
    }

  }while(!build_order.empty());


  for (auto& factory_to_destroy : factories){//mittlerweile wahrscheinlich unnoetig
    if (factory_to_destroy.second->name != "player"){
      output += Destroy_factory_event(time+1, factory_to_destroy.second->id);
    }
  }

  output += Victory_event(time+2);
  return time+2;
/*for (Factory* factory : factories){
std::cerr << factory << std::endl;
}*/


}


bool Simulator::find_work(Factory* factory, bool print_to_json){
  /*if ((*build_order_by_factories[1].front()).item->name == "copper-cable"){
    std::cerr << "halt" << std::endl;
  }*/

  /*if (build_order_by_factories[factory->build_order_index].empty()){

    factory->all_jobs_done = true;
    factories_all_jobs_done.insert(factory);


    return;
  }*/

  if (!build_order_by_factories[0].empty()){
    if((*build_order_by_factories[0].top()).ingredients_still_needed == 0 && time != time_of_last_research_event){
      Order& order = *build_order_by_factories[0].top();
      if (print_to_json){
        output += Research_event(time, order.item->name);
      }
      //std::cout << std::setw(2) << Research_event(time, order.item->name) << std::endl;
      Technology* technology = static_cast<Technology*>(order.item);
      technology->researched = true;
      for (Recipe* effect : technology->effects){
        effect->enabled = true;
      }
      /*for (std::list<std::list<Order>::iterator>::iterator iterator = build_order_by_factories[0].begin(); iterator != build_order_by_factories[0].end(); ++iterator){
        Technology* advanced_technology = static_cast<Technology*>((**iterator).item);
        for (Technology* prerequisite : advanced_technology->prerequisites){
          if (prerequisite->name == technology->name){
            --((**iterator).ingredients_still_needed);

          }
        }
      }*/
      for (std::list<Order>::iterator iterator = build_order.begin(); iterator != build_order.end(); ++iterator){
        Order& advanced_order= *iterator;
        if (advanced_order.ingredients_still_needed != 0 && advanced_order.item->type != "technology"){
          for (Recipe* effect : technology->effects){
            if (advanced_order.recipe.first == effect){
              --advanced_order.ingredients_still_needed;

              if (advanced_order.ingredients_still_needed == 0){
                const std::string& category = advanced_order.recipe.first->crafting_category.name;
                if (category == "wood-crafting" || category == "basic-solid" || category == "crafting" || category == "basic-crafting" || category == "advanced-crafting")
                                                              build_order_by_factories[1].push(iterator);
                else if (category == "crafting-with-fluid")   build_order_by_factories[2].push(iterator);
                else if (category == "oil-processing")        build_order_by_factories[3].push(iterator);
                else if (category == "chemistry")             build_order_by_factories[4].push(iterator);
                else if (category == "rocket-building")       build_order_by_factories[5].push(iterator);
                else if (category == "basic-smelting")        build_order_by_factories[6].push(iterator);
                else if (category == "burner-solid")          build_order_by_factories[7].push(iterator);
                else if (category == "basic-fluid")           build_order_by_factories[8].push(iterator);
                else if (category == "water-pump")            build_order_by_factories[9].push(iterator);
              }
            }
          }
        }
        else if(advanced_order.item->type == "technology"){
          Technology* advanced_technology = static_cast<Technology*>(advanced_order.item);
          for (Technology* prerequisite : advanced_technology->prerequisites){
            if (prerequisite->name == technology->name){
              --(advanced_order.ingredients_still_needed);
              if (advanced_order.ingredients_still_needed == 0){
                build_order_by_factories[0].push(iterator);
              }
            }
          }
        }
      }
      build_order.erase(build_order_by_factories[0].top());
      build_order_by_factories[0].pop();
      time_of_last_research_event = time;
      find_work(factory);
      return true;
    }
  }

  //for (std::list<std::list<Order>::iterator>::iterator iterator = build_order_by_factories[factory->build_order_index].begin(); iterator != build_order_by_factories[factory->build_order_index].end(); ++iterator){


  if (!build_order_by_factories[factory->build_order_index].empty()){
    std::list<Order>::iterator iterator = build_order_by_factories[factory->build_order_index].top();
  //std::list<std::list<Order>::iterator>::iterator iterator = build_order_by_factories[factory->build_order_index].begin();
  //if (iterator != build_order_by_factories[factory->build_order_index].end()){
    Order& order = *iterator;
    if (order.ingredients_still_needed == 0){
      if (order.item->stock > order.quantity){
        if (order.purpose != build_order.end()){
          --((*order.purpose).ingredients_still_needed);
        }
        if ((*order.purpose).ingredients_still_needed == 0){
          if ((*order.purpose).item->type == "technology"){
            build_order_by_factories[0].push(order.purpose);
          }
          else{
            const std::string& category = (*order.purpose).recipe.first->crafting_category.name;
            if (category == "wood-crafting" || category == "basic-solid" || category == "crafting" || category == "basic-crafting" || category == "advanced-crafting")
                                                      build_order_by_factories[1].push(order.purpose);
            else if (category == "crafting-with-fluid")   build_order_by_factories[2].push(order.purpose);
            else if (category == "oil-processing")        build_order_by_factories[3].push(order.purpose);
            else if (category == "chemistry")             build_order_by_factories[4].push(order.purpose);
            else if (category == "rocket-building")       build_order_by_factories[5].push(order.purpose);
            else if (category == "basic-smelting")        build_order_by_factories[6].push(order.purpose);
            else if (category == "burner-solid")          build_order_by_factories[7].push(order.purpose);
            else if (category == "basic-fluid")           build_order_by_factories[8].push(order.purpose);
            else if (category == "water-pump")            build_order_by_factories[9].push(order.purpose);
          }
        }





        order.item->stock -= order.quantity;
        build_order.erase(iterator);
        build_order_by_factories[factory->build_order_index].pop();
        find_work(factory);
        return true; // das ist ein fehler !!!! eigentlich return find_work(factory);

      }//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if (order.recipe.first->enabled){
        bool recipe_can_be_executed = false;
        for (Crafting_category& category : factory->crafting_categories){
          if (category == order.recipe.first->crafting_category){
            recipe_can_be_executed = true;
            break;
          }
        }
        if (recipe_can_be_executed){
          /*for (std::pair<Item*, int>& ingredient : order.recipe.first->ingredients){

            if (ingredient.first->type == "factory" && ingredient.first->name != "electric-furnace"){
              for (int i = 0; i < ingredient.second * std::ceil(order.quantity/order.recipe.second); ++i){
                for (auto& j : factories){
                  Factory* ingredient_factory = j.second;
                  int factory_id;
                  if (ingredient.first->name == ingredient_factory->name && ingredient_factory != factory && !ingredient_factory->destroyed){
                    ingredient_factory->destroyed = true;
                    int ingredient_factory_id = ingredient_factory->id;

                    if (ingredient_factory->all_jobs_done){
                      factories_all_jobs_done.erase(ingredient_factory);
                      factories.erase(ingredient_factory_id);
                    }


                    else if (ingredient_factory->starved){
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
                      if (print_to_json){
                        output += Stop_factory_event(time, ingredient_factory->id);
                      }
                      //std::cout << std::setw(2) << Stop_factory_event(time, ingredient_factory->id) << std::endl;
//std::cerr << Stop_factory_event(time, ingredient_factory->id) << std::endl;
                    }
                    if (print_to_json){
                      output += Destroy_factory_event(time, ingredient_factory_id);//hier muss noch die Factory aus dem Vector geloescht werden und deleted werden
                    }
                    //std::cout << std::setw(2) << Destroy_factory_event(time, ingredient_factory_id);
//std::cerr << Destroy_factory_event(time, ingredient_factory->id) << std::endl;
                    break;

                  }
                }

              }
            }
          }*/
          factory->current_job = order;
          factory->time_job_started = time;
          if (print_to_json){
            output += Start_factory_event(time, factory->id, order.recipe.first->name);
          }
          //std::cout << std::setw(2) << Start_factory_event(time, factory->id, order.recipe.first->name) << std::endl;
//std::cerr << Start_factory_event(time, factory_p->id, order.recipe->name) << std::endl;
          int time_finished = time + static_cast<int>((std::ceil(order.quantity/order.recipe.second))  * (std::ceil(order.recipe.first->energy / factory->crafting_speed)));  //nochmal drueber nachdenken!
          future_events.push(Stop_factory_event(time_finished, factory->id));
          build_order.erase(iterator);
          build_order_by_factories[factory->build_order_index].pop();
          return true;
        }
      }
    }
  }

  factory->starved = true;
  starved_factories.push_back(factory);
  return false;
}




void Simulator::print_events(){
  std::cout << std::setw(2) << output << std::endl;
}


void Simulator::printbuild_order(std::ostream& out){
    for (auto& order : build_order){
      out << *order.item << std::endl << order.quantity << std::endl;
      out << "ingredients_still_needed: " << order.ingredients_still_needed << std::endl;
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
