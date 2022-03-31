#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <recipe.hpp>

#include "item.hpp"

//class Recipe;


class Technology: public Item{
public:

  Technology(const std::pair<std::string, json>&, std::unordered_map<std::string, Item*>&, std::unordered_map<std::string, Recipe*>&);
  Technology();


  bool operator==(const Technology&) const;

  friend std::ostream& operator<<(std::ostream&, const Technology&);


  bool researched = false;
  std::vector<Recipe*> effects;
  std::vector<std::pair<Item*, int>> ingredients;
  std::vector<Technology*> prerequisites;
};
