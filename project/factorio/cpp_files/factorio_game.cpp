#include "factorio_game.hpp"

#include <iostream>
#include <fstream>

#include "nlohmann/json.hpp"
#include "item.hpp"
#include "factory.hpp"
#include "recipe.hpp"
#include "technology.hpp"

using json = nlohmann::json;

Factorio_game::Factorio_game(){

  std::ifstream in("../factorio-data/item.json");
  json myjson;
  in >> myjson;

  std::unordered_map<std::string,json> items_json_map = myjson;
  for (std::pair<std::string, json> i : items_json_map){
    items_blueprint[i.first] = new Item(i);
  }

  in.close();
  myjson.clear();


  in.open("../factorio-data/factory.json");
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


  in.open("../factorio-data/recipe.json");
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


  in.open("../factorio-data/technology.json");
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
}


Factorio_game::~Factorio_game(){

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


void Factorio_game::restore_original_state(){

  for (auto& i : recipes_blueprint){
    Recipe* recipe = i.second;
    recipe->enabled = recipe->enabled_at_begin;
  }

  for (auto& i : technologies_blueprint){
    Technology* technology = i.second;
    technology->researched = false;
  }

}
