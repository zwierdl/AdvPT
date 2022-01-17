#pragma once

#include <string>

#include "nlohmann/json.hpp"
#include "item.hpp"

using json = nlohmann::json;

class Crafting_category : public Item{
public:
  Crafting_category(const std::string&);
  Crafting_category();
};
