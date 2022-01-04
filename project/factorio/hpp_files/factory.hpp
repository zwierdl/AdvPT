#pragma once

#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "crafting_category.hpp"
#include "item.hpp"
#include "order.hpp"

using json = nlohmann::json;

class Factory : public Item {
public:
  Factory(const std::pair<std::string, json>&);
  //Factory(std::string, json);
  Factory();
  Factory(const std::string&, int, const std::string&, std::unordered_map<std::string, Factory*>&);

  friend json& operator<<(json& out, const Factory& factory);
  friend std::ostream& operator<<(std::ostream& out, const Factory& factory);

  const double crafting_speed = -1;
  std::vector<Crafting_category> crafting_categories;
  const std::string factory_name;
  const int id = -1;
  Order current_job;
  bool starved = false;
  //Recipe* default_job = nullptr;

};
