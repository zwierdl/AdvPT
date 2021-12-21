#pragma once

#include <string>
#include <iostream>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Item{
public:

  Item(std::pair<std::string, json>);
  Item(std::string, std::string);
  Item();

  friend std::ostream& operator<<(std::ostream&, const Item&);
  friend json& operator<<(json&, const Item&);

  std::string name;
  std::string type;
  int stock = 0;
};
