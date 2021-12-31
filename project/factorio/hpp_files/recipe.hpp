#pragma once

#include <vector>
#include <string>
#include <unordered_map>

//class Item;
class Technology;

#include "item.hpp"
#include "crafting_category.hpp"
#include "technology.hpp"


using json = nlohmann::json;

class Recipe : public Item{
public:

  Recipe(const std::pair<std::string,json>&, std::unordered_map<std::string, Item*>&);
  Recipe();

  bool operator==(const Recipe&) const;

  friend std::ostream& operator<<(std::ostream&, const Recipe&);
  friend json& operator<<(json&, const Recipe&);

  const Crafting_category crafting_category;
  bool enabled;
  const bool enabled_at_begin = false;
  const int energy = -1;
  std::vector<std::pair<Item*, int>> ingredients;
  std::vector<std::pair<Item*, int>> products;

  Technology* technology_for_unlock = nullptr; //muss beim Zugriff zu Technology* gecastet werden

};
