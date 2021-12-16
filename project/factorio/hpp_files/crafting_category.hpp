#pragma once

#include <string>

#include "nlohmann/json.hpp"
#include "item.hpp"

using json = nlohmann::json;

class Crafting_category : public Item{
public:
   //Crafting_category(std::pair<std::string, json>);
  Crafting_category(std::string);

};
