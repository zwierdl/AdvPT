#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "item.hpp"
#include "crafting_category.hpp"

using json = nlohmann::json;

class Recipe : public Item{
public:

  Recipe(std::pair<std::string,json>, std::unordered_map<std::string, Item>&);
  Recipe();

  friend std::ostream& operator<<(std::ostream&, const Recipe&);
  friend json& operator<<(json&, const Recipe&);

  Crafting_category crafting_category;
  //std::string name;
  bool enabled;
  int energy;
  std::vector<std::pair<std::string, int>> ingredients;
  //std::vector<std::pair<Item, int>> ingredients;
  std::vector<std::pair<std::string, int>> products;
  //std::vector<std::pair<Item, int>> products;

};
