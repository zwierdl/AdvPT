#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <iostream>

#include "nlohmann/json.hpp"
#include "item.hpp"
#include "factory.hpp"
#include "recipe.hpp"
#include "technology.hpp"

using json = nlohmann::json;

class Simulator {
public:

  Simulator();

  std::vector<Recipe> recipes_blueprint;
  std::vector<Item> items_blueprint;
  std::vector<Technology> technologies_blueprint;
  //std::map<std::string, Factory> factories_blueprint;
  std:: vector<Factory> factories_blueprint;
};
