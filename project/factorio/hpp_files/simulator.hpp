#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <iostream>

#include "nlohmann/json.hpp"
#include "item.hpp"
#include "factory.hpp"
#include "recipe.hpp"
#include "technology.hpp"
#include <deque>


using json = nlohmann::json;

class Simulator {
public:

  Simulator(const std::string);

  void buildBasicBuildOrder();

  //std::deque<std::tupel<Item, int, Item>> basicBuildOrder();
  //void printRecipeDoubles();
/*
  json items;
  json recipes;
  json technologies;
  json factories;
  json initial;
*/

  //std::vector<Factory> factories_blueprint;
  //std::vector<Recipe> recipes_blueprint;
  //std::vector<Item> items_blueprint;
  //std::vector<Technology> technologies_blueprint;

  //std::map<std::string, Factory> factories_blueprint;
  std::unordered_map<std::string, Factory> factories_blueprint;
  std::unordered_map<std::string, Recipe> recipes_blueprint;
  std::unordered_multimap<std::string, std::pair<Recipe, int>> recipes_by_item;
  std::unordered_map<std::string, Technology> technologies_blueprint;
  std::unordered_map<std::string, Item> items_blueprint;
  json initial;
  std::vector<Factory> factories;
  std::vector<std::pair<Item, int>> goal;
  std::deque<std::pair<std::pair<Item, int>, Item>> basicBuildOrder;
};
