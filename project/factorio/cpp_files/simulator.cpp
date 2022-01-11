# include "simulator.hpp"
# include "factorio_game.hpp"
#include <cmath>

Simulator::Simulator(const std::string& challenge, Factorio_game& factorio):
  items_blueprint(factorio.items_blueprint),
  factories_blueprint(factorio.factories_blueprint),
  recipes_blueprint(factorio.recipes_blueprint),
  technologies_blueprint(factorio.technologies_blueprint),
  buildOrder(factorio.build_order),
  build_order_by_factories(factorio.build_order_by_factories){
//printItems(std::cerr);
  std::ifstream in(challenge.c_str());
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
    item_p->calculate_energy(available_categories);
  }

  for (json::iterator i = initial["goal-items"].begin(); i != initial["goal-items"].end(); ++i){
    goal.push_back(std::pair<Item*, int>(items_blueprint[(*i)["name"]], (*i)["amount"]));
    //goal.push_back(std::pair<Item, int>(Item((*i)["name"], "item"), (*i)["amount"]));
  }
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


void Simulator::build_items(){
  std::list<Order>::iterator iterator = buildOrder.end();
  process_order(Order(items_blueprint["stone-furnace"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr), buildOrder, iterator);
  iterator = buildOrder.end();
  process_order(Order(items_blueprint["burner-mining-drill"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr), buildOrder, iterator);
  iterator = buildOrder.end();
  process_order(Order(items_blueprint["assembling-machine-1"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr), buildOrder, iterator);
  iterator = buildOrder.end();
  process_order(Order(items_blueprint["assembling-machine-2"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr), buildOrder, iterator);
  iterator = buildOrder.end();
  process_order(Order(items_blueprint["offshore-pump"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr), buildOrder, iterator);
  iterator = buildOrder.end();
  process_order(Order(items_blueprint["pumpjack"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr), buildOrder, iterator);
  iterator = buildOrder.end();
  process_order(Order(items_blueprint["oil-refinery"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr), buildOrder, iterator);
  iterator = buildOrder.end();
  process_order(Order(items_blueprint["chemical-plant"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr), buildOrder, iterator);
  iterator = buildOrder.end();
  process_order(Order(items_blueprint["rocket-silo"], 1, std::pair<Recipe*, int>(nullptr,0), nullptr), buildOrder, iterator);
  iterator = buildOrder.end();

  for (auto& i : goal){
//buildOrder.push_back(std::list<Order>());
    Item* item_p = i.first;
    int ordered_amount = i.second;
    process_order(Order(item_p, ordered_amount, std::pair<Recipe*, int>(nullptr, 0), nullptr), buildOrder, iterator);
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

    for (std::list<Order>::iterator pos = buildOrder.begin(); pos != buildOrder.end(); ++pos){
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


void Simulator::printBuildOrder(std::ostream& out){
    for (auto& order : buildOrder){
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
