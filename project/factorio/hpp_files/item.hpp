#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <unordered_set>

class Hash;
class Recipe;

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Item{
public:

  Item(std::pair<std::string, json>);
  Item(std::string, std::string);
  Item();
  virtual ~Item() = default;

  bool operator==(const Item&) const;

  void calculate_energy(std::unordered_set<Item, Hash>&);

  friend std::ostream& operator<<(std::ostream&, const Item&);
  friend json& operator<<(json&, const Item&);

  const std::string name;
  const std::string type;
  std::pair<Recipe*, int> best_recipe = std::pair<Recipe*, int>(nullptr,0);
  std::vector<std::pair<Recipe*, int>> recipes_produce;
  std::vector<std::pair<Recipe*, int>> recipes_consume;
  int stock = 0;
  double energy = -1;
};
