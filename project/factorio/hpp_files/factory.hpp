#pragma once

#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "crafting_category.hpp"
#include "item.hpp"

using json = nlohmann::json;

class Factory : public Item {
public:
  Factory(std::pair<std::string, json>);
  //Factory(std::string, json);
  Factory();
  Factory(std::string, int, std::string, std::unordered_map<std::string, Factory>&);

  friend json& operator<<(json& out, const Factory& factory);
  friend std::ostream& operator<<(std::ostream& out, const Factory& factory);

  double crafting_speed;
  std::vector<Crafting_category> crafting_categories;
  std::string factory_name;
  int id;

};
