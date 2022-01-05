# include "simulator.hpp"
#include <cmath>

Simulator::Simulator(std::string challenge){


  std::ifstream in("../factorio-data/item.json");
  json myjson;
  in >> myjson;

  std::unordered_map<std::string, json> items_json_map = myjson;
  for (std::pair<std::string, json> i : items_json_map){
    items_blueprint[i.first] = new Item(i);
  }
  in.close();
  myjson.clear();

  in.open("../factorio-data/factory.json");
  in >> myjson;

  std::unordered_map<std::string, json> factories_json_map = myjson;

  for (std::pair<std::string, json> i : factories_json_map){

    factories_blueprint[i.first] = new Factory(i);  //hier entstehen memory-leaks
  }


  for (std::pair<std::string, Factory*> i : factories_blueprint){
    items_blueprint[i.first] = i.second;
  }


  /*std::cout << "\nfactories:\n" << std::endl;
  for (std::pair<std::string, Factory*> i : factories_blueprint){
    std::cout << "\n" << i.first << std::endl;
    std::cout << *(i.second) << std::endl;
  }*/


  myjson.clear();
  in.close();


  in.open("../factorio-data/recipe.json");
  in >> myjson;

  std::unordered_map<std::string, json> recipes_json_map = myjson;
  for (std::pair<std::string, json> i : recipes_json_map){
    bool barrel = false; //catalyst amount?? was ist das bei rezepten
    for (json::iterator j = i.second["products"].begin(); j != i.second["products"].end(); ++ j){
      if ((*j)["name"] == "empty-barrel") barrel = true;
    }

    if (!barrel) recipes_blueprint[i.first] = new Recipe(i, items_blueprint);
  }

  /*std::cout << "\nRecipes:\n" << std::endl;
  for (std::pair<std::string,Recipe*> i : recipes_blueprint){
    std::cout << i.first << std::endl << (*i.second) << std::endl;
  }*/

  for (auto& i : recipes_blueprint){
    Recipe* recipe_p = i.second;
    for(auto& j: recipe_p->products){
      Item* item = j.first;
      int amount = j.second;
      (items_blueprint[item->name]->recipes_produce).push_back(std::pair<Recipe*, int>(recipe_p, amount));
    }

    for (auto& j : recipe_p->ingredients){
      Item* item = j.first;
      int amount = j.second;
      (items_blueprint[item->name]->recipes_consume).push_back(std::pair<Recipe*, int>(recipe_p, amount));
    }
  }



  /*std::cout << "items:\n" << std::endl;
  for (auto& i : items_blueprint){
    std::cout << *(i.second) << std::endl;
  }*/

  /*
  for (auto i : recipes_blueprint){
    for (std::pair<std::string, int> j : i.second.products){
      recipes_by_item.insert(std::pair<std::string, std::pair<Recipe, int>>(j.first, std::pair<Recipe, int>(i.second, j.second)));
    }
  }
  */
  /*std::cout << "recipes_by_item:\n" << std::endl;
  for (auto i : recipes_by_item){
    std::cout << i.first << std::endl << i.second.first << std::endl << i.second.second;
  }*/

  myjson.clear();
  in.close();


  in.open("../factorio-data/technology.json");
  in >> myjson;

  std::unordered_map<std::string, json> technologies_json_map = myjson;
  for (std::pair<std::string, json> i : technologies_json_map){
    technologies_blueprint[i.first] = new Technology(i, items_blueprint, recipes_blueprint);
  }

  for (std::pair<std::string, json> i : technologies_json_map){
    std::string name = i.first;
    json technology_json = i.second;
    for (json::iterator j = technology_json["prerequisites"].begin(); j != technology_json["prerequisites"].end(); ++j){
      (technologies_blueprint[name]->prerequisites).push_back(technologies_blueprint[*j]);
    }
  }

  for (auto& i : technologies_blueprint){
    Technology* technology_p = i.second;
    for (Recipe* recipe_p : technology_p->effects){
      //if (recipes_blueprint.contains(recipe_p->name)){
      //  std::cout << recipe_p->name << *technology_p << std::endl;
        recipes_blueprint[recipe_p->name]->technology_for_unlock = technology_p;
      //}
    }

  }



  /*std::cout <<"technologies:\n" << std::endl;
  for (auto& i :technologies_blueprint){
    std::cout << i.first << std::endl << *(i.second) << std::endl;
  }*/


  myjson.clear();
  in.close();



  /*
  myjson.clear();
  in.close();
  */

  in.open(challenge.c_str());
  in >> initial;

  int id = 0;
  for (json::iterator i = initial["initial-factories"].begin(); i != initial["initial-factories"].end(); ++i){
    items_blueprint[(*i)["factory-type"]]->stock = 1; // fehleranfaellig bessser ++
    factories.push_back(Factory((*i)["factory-type"], (*i)["factory-id"], (*i)["factory-name"], factories_blueprint));
    ++id;
  }

  for (json::iterator i = initial["initial-items"].begin(); i != initial["initial-items"].end(); ++i){
    items_blueprint[(*i)["name"]]->stock = (*i)["amount"];
    if (factories_blueprint.contains((*i)["name"])){
      for (int j = 0; j < (*i)["amount"]; ++j){
        factories.push_back(Factory((*i)["name"], id++, "dummy-name", factories_blueprint));
      }
    }
  }


  for (Factory& factory : factories){
    for (Crafting_category& category : factory.crafting_categories){
      available_categories.insert(category);
    }
  }

  for (auto& i : items_blueprint){
    Item* item_p = i.second;
    /*if (*item_p == *items_blueprint["stone-furnace"] || *item_p == *items_blueprint["stone"]){
      std::cout << "stop";
    }*/
    item_p->calculate_energy(available_categories);
  }


  for (json::iterator i = initial["goal-items"].begin(); i != initial["goal-items"].end(); ++i){
    goal.push_back(std::pair<Item*, int>(items_blueprint[(*i)["name"]], (*i)["amount"]));
    //goal.push_back(std::pair<Item, int>(Item((*i)["name"], "item"), (*i)["amount"]));
  }

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
  //in.close();

}

/*void Simulator::buildItemBuildOrder(){
  int index = 0;
  for (auto& i : goal){
    buildOrder.push_back(std::list<Order>());
    Item* item_p = i.first;
    int amount_ordered = i.second;
    std::list<Order> toProcess{Order(item_p, amount_ordered, nullptr)};
    int coal_count = 0;
    while (!toProcess.empty()){
      item_p = toProcess.back().item;
      amount_ordered = toProcess.back().quantity;
      toProcess.pop_back();
      if (amount_ordered != 1 || item_p->name != "coal"){
        Recipe* recipe_p = static_cast<Recipe*>(item_p->recipes[0].first);
        int amount_produced = item_p->recipes[0].second;
        buildOrder[index].push_front(Order(item_p, amount_ordered, recipe_p));
        //std::cout << *item_p << amount_ordered << *recipe_p << std::endl;
        for (auto& ingredient : recipe_p->ingredients){
          item_p = ingredient.first;
          int amount_needed_for_recipe = ingredient.second;
          toProcess.push_back(Order(item_p, (amount_ordered / amount_produced + 1) *amount_needed_for_recipe, nullptr));
        }
      }
      else{
        coal_count++;
      }
    }
    buildOrder[index].push_front(Order(items_blueprint["coal"], coal_count/6 + 1, recipes_blueprint["coal-burner"]));
    buildOrder[index].push_front(Order(items_blueprint["coal"], 1, recipes_blueprint["coal"]));
    ++index;
  }
}*/

/*void Simulator::addTechnologies(){
  for (std::list<Order>& list : buildOrder){
    for (std::list::iterator i = list.begin(); i != list.end(); ++i){
      Order& order = *i;
      Recipe* recipe_p = static_cast<Recipe*> order.purpose;
      if (!recipe_p->enabled){
        //std::list<Order> toAdd;
        std::list<Order> toAdd;

        Technology* technology_for_unlock = static_cast<Technology*>(recipe_p->technology_for_unlock);
        process_technology(technolog_for_unlock, toAdd);
      }
    }

  }
  /*
  while(!buildOrder1.empty()){
    Order order = buildOrder1.front();
    buildOrder1.pop_front();
    Recipe* recipe_p = static_cast<Recipe*> order.purpose;
    if (!recipe_p->enabled){

    }
    bui
  }
  */
//}






void Simulator::build_items(){
  int index = 0;
  for (auto& i : goal){
    buildOrder.push_back(std::list<Order>());
    Item* item_p = goal[index].first;
    int ordered_amount = goal[index].second;
    std::list<Order>::iterator iterator = buildOrder[index].end();
    process_order(Order(item_p, ordered_amount, nullptr, nullptr), buildOrder[index], iterator);
    ++index;
  }
}


void Simulator::process_order(Order order, std::list<Order>& list, std::list<Order>::iterator& iterator){
  Item* item_p = order.item;
  double amount_ordered = order.quantity;

  if (typeid(item_p) == typeid(Item*)){// wahrscheinlich unnoetig
    Recipe* recipe_p = item_p->best_recipe.first;
    if (recipe_p == nullptr){
      unlock_missing_ingredient(item_p, list, iterator);
      /*if (!item_p->recipes_produce[0].first->enabled)
        research_Technology(item_p->recipes_produce[0].first->technology_for_unlock, list, iterator);
      }*/
    }
    /*while (item_p->best_recipe.first == nullptr){
      unlock_missing_ingredent(item_p, list, iterator);
    }*/
    recipe_p = item_p->best_recipe.first;
    /*if (item_p->name == "spidertron"){
      printItems(std::cout);
    }*/
    if (item_p->name == "coal" && amount_ordered == 1){
      recipe_p = recipes_blueprint["coal"];
    }
    list.insert(iterator, Order(item_p, amount_ordered, recipe_p, order.purpose));
    --iterator;
    Order* purpose = &(*iterator);
    int amount_produced = item_p->best_recipe.second;
    for (auto& ingredient : recipe_p->ingredients){
      Item* item_ingredient_p = ingredient.first;
      int amount_consumed = ingredient.second;
      process_order(Order(item_ingredient_p, std::ceil(amount_ordered/amount_produced) * amount_consumed, nullptr, purpose), list, iterator);
    }
    if (item_p->type == "factory"){
      bool new_category = false;
      for (auto category : factories_blueprint[item_p->name]->crafting_categories){
        new_category += available_categories.insert(category).second;
      }
      if (new_category){
        for (auto& string_item_p_pair : items_blueprint){
          string_item_p_pair.second->calculate_energy(available_categories);
        }
      }
    }
  }

}

void Simulator::unlock_missing_ingredient(Item* item_p ,std::list<Order>& list, std::list<Order>::iterator iterator){
  Recipe* recipe_p = item_p->best_recipe.first;
  if (recipe_p == nullptr){
    if (!item_p->recipes_produce[0].first->enabled){
      research_Technology(item_p->recipes_produce[0].first->technology_for_unlock, list, iterator);
    }
  }
  recipe_p = item_p->best_recipe.first;
  if (recipe_p == nullptr){
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
  list.insert(iterator, Order(technology_p, 1, nullptr, nullptr));
  --iterator;
  Order* purpose = &(*iterator);
  for (auto& i : technology_p->ingredients){
    process_order(Order(i.first, i.second, nullptr, purpose), list, iterator);
  }
  technology_p->researched = true;
  for (Recipe* effect : technology_p->effects){
    effect->enabled = true;
    for (auto& i : effect->products){
      Item* item_p = i.first;
      item_p->calculate_energy(available_categories);
    }
  }

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

  for (auto& i : items_blueprint){
    Item* item_p = i.second;
    item_p->calculate_energy(available_categories);
  }
}

/*void Simulator::apply_on_all_items(void(*function)(Item*)){
  for (auto& i : items_blueprint){
    function(i.second);
  }
}*/

/*void Simulator::calculate_energies(){
  bool done = false;
  std::unordered_set<Item*> set;
  for (auto& i : items_blueprint){
    Item* item_p = i.second;
    set.insert(item_p);
  }

  while(!done){
    bool something_changed = false;
    for (Item* item_p : queue){
      bool recipe_available = false;
      for (auto& i : item_p->recipes){
        Recipe* recipe_p = static_cast<Recipe*>(i.first);
        if (recipe_p->enabled){
          recipe_available = true;
          for (auto& j : recipe_p->ingredients){
            Item* ingredient_p = j.first;
            if (set.contains(ingredients_p) && ingredient_p->energy !=-1){

            }
          }
        }
      }
    }
  }

}*/

/*void Simulator::process_technology(Technology* technology_p, std::list<Order>& toAdd){
  toAdd.push_front(order(technology_p, 1, nullptr));
  for (Technology* prerequisite : technology_p->prerequisites){
    if (prerequsite->researched == false){
      process_technology(prerequsite, toAdd)
    }
  }
  for (auto& ingredient : technology_p->ingredients){  //hier muss man noch auf enablete recepte schauen
    Item* item_p = ingredient.first;
    amount_ordered = ingredient.second
  }

}*/

/*void Simulator::concartinate_identical_items(){
  for (std::list<Order>& list : buildOrder){
    std::list<Order>::iterator drag = list.begin();
    if (!list.empty()){
      for (std::list<Order>::iterator pos = ++list.begin(); pos != list.end(); ++pos){
        if (drag->item == pos->item){
          pos->quantity += drag->quantity;
        }
      }
    }
  }
}*/

void Simulator::printBuildOrder(std::ostream& out){
  for (auto& list : buildOrder){
    for (auto& order : list){
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


/*void Simulator::buildBasicBuildOrder(){
  std::deque<std::pair<Item, int>> processing;

  for (std::pair<Item, int> i : goal){
    processing.push_front(i);
  }
  while(!processing.empty()){
    Item product = processing.back().first;
    int wanted_amount = processing.back().second;
    std::pair<Recipe, int>& recipe_entry = recipes_by_item.find(product.name)->second;
    Recipe& recipe = recipe_entry.first;
    int product_amount = recipe_entry.second;
    for (std::pair<std::string, int> i : recipe.ingredients){
      std::string ingredient = i.first;
      int ingredient_amount = i.second;
      processing.push_front(std::pair<Item, int>(items_blueprint[ingredient], (wanted_amount/product_amount + 1) * ingredient_amount));
    }

    basicBuildOrder.push_front(std::tie(product, wanted_amount, recipe));

    processing.pop_back();
  }
  std::cout << "basic build order\n" << std::endl;
  /*for (auto i : basicBuildOrder){
    std::cout << std::get<0>(i) << std::endl << std::get<1>(i) << std::endl << static_cast<Recipe&>(std::get<2>(i)) << std::endl;
  }*/
//}

/*void Simulator::addTechnologies(){
  std::unordered_set<Technology, Hash> enabled_technologies;
  std::unordered_set<Recipe, Hash> enabled_recipes;
  std::unordered_map<std::string, Technology> technologies_by_recipe;

  for (auto& i : technologies_blueprint){
    Technology& technology = i.second;
    for (std::string& recipe_string : technology.effects){
      technologies_by_recipe[recipe_string] = technology;
    }
  }

  for (auto& i : recipes_blueprint){
    Recipe& recipe = i.second;
    if (recipe.enabled){
      enabled_recipes.insert(recipe);
    }
  }

  for (std::deque<std::tuple<Item, int, Item&>>::iterator entry = basicBuildOrder.begin(); entry != basicBuildOrder.end(); ++entry){
    Recipe& recipe = static_cast<Recipe&>(std::get<2>(*entry));
    if (!enabled_recipes.contains(recipe)){
      std::deque<std::tuple<Item, int, Item&>> toProduce;
    }


    //std::cout << std::get<2>(*entry) << std::endl;

  }
}*/


/*void Simulator::buildOrderAddFactories()
  bool finished = false;
  std::unordered_multimap<std::string, Factory> factories_by_crafting_category;

  for (Factory factory : factories){
    for (Crafting_category category : factory.crafting_categories){
      factories_by_crafting_category.insert(std::pair<std::string, Factory>(category.name, factory));
    }
  }


  for (std::pair<std::pair<Item, int>, Recipe> node : basicBuildOrder){
    if (!factories_by_crafting_category.contains(node.second.crafting_category.name))

  }
}*/


/*
std::deque<std::tupel<Item, int, Item>> Simulator::basicBuildOrder(){
  */

  /*
  std::deque <std::pair<std::string, int>> toProduce;

  for (json::iterator i = initial["goal-items"].begin(); i != initial["goal-items"].end(); ++i){
      toProduce.push_back(std::pair<std::string, int>((*i)["name"], (*i)["amount"]));
  }

  for (std::pair<std::string, int> i : toProduce){
    //std::cout << "name: " << i.first << "amount" << i.second << std::endl;

  }*/
  /*
  std::deque<std::pair<std::string, int>> produced;

  while (!toProduce.empty()){
    json recipe = recipes.find[toProduce.pop_front()];

    for (json:iter)
  }
  */
  /*
  return std::deque<std::string>();
}*/

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
