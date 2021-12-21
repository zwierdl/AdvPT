#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "item.hpp"
#include "recipe.hpp"

class Technology: public Item{
public:
  //std::string name;

  Technology(const std::pair<std::string, json>&, std::unordered_map<std::string, Item>&, std::unordered_map<std::string, Recipe>&);
  Technology();

  friend std::ostream& operator<<(std::ostream&, const Technology&);



  //std::vector<Recipe> effects;
  std::vector<std::string> effects;

  //std::vector<std::pair<Item, int>> ingredients;
  std::vector<std::pair<std::string, int>> ingredients;

  std::vector<std::string> prerequisites_string;
};
