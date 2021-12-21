#include "factory.hpp"

Factory::Factory(std::pair<std::string, json> input): Item(input.first, "factory"){   //anstatt "factory" muss type vielleicht "item" sein?!
  crafting_speed = input.second["crafting_speed"];
  std::map <std::string, json> crafting_categories_json_map = input.second["crafting_categories"];
  for (std::pair<std::string, json> i : crafting_categories_json_map){
    crafting_categories.push_back(Crafting_category(i.first));
  }
}

/*Factory::Factory(std::string name, json input): Item(name, "factory"){   //anstatt "factory" muss type vielleicht "item" sein?!
  crafting_speed = input["crafting_speed"];
  std::map <std::string, json> crafting_categories_json_map = input["crafting_categories"];
  for (std::pair<std::string, json> i : crafting_categories_json_map){
    crafting_categories.push_back(Crafting_category(i.first));
  }
}*/

Factory::Factory(){};


json& operator<<(json& out, const Factory& factory){
  out[factory.name]["crafting_speed"] = factory.crafting_speed;
  for (Crafting_category i : factory.crafting_categories){
    out[factory.name]["crafting_categories"][i.name] = true;
  }
  /*std::vector<json> crafting_categories_with_bool;
  json j;
  for (Crafting_category i : factory.crafting_categories){
    j[i.name] = true;
    crafting_categories_with_bool.push_back(j);
  }
  out[factory.name]["crafting_categories"] = crafting_categories_with_bool;*/
  //out[factory.name]["crafting_speed"] = factory.crafting_speed;
  return out;
}

std::ostream& operator<<(std::ostream& out, const Factory& factory){
  json j;
  j << factory;
  out << std::setw(2) << j << std::endl;
  return out;
}
