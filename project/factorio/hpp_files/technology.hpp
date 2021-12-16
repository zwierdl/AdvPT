#pragma once

#include <string>
#include <vector>

#include "item.hpp"
#include "recipe.hpp"

class Technology: public Item{
public:
  //std::string name;

  Technology(std::pair<std::string, json>);

  friend std::ostream& operator<<(std::ostream&, const Technology&);



  std::vector<std::string> effects;
  std::vector<std::pair<Item, int>> ingredients;
  std::vector<std::string> prerequisites;
};
