#include "factory.hpp"

Factory::Factory(const std::pair<std::string, json>& input):
  Item(input.first, "factory"),
  crafting_speed(input.second["crafting_speed"]) {
  //crafting_speed = input.second["crafting_speed"];
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

Factory::Factory(const std::string& name, int id, const std::string& factory_name, std::unordered_map<std::string, Factory*>& factories_blueprint):
Item(name, "factory"),
id(id),
factory_name(factory_name),
crafting_speed(factories_blueprint[name]->crafting_speed),
crafting_categories(factories_blueprint[name]->crafting_categories){
  switch (name){
    case "assembling-machine-1":
    case "player":                build_order_index = 0;
                                  break;
    case "assembling-machine-2":  build_order_index = 1;
                                  break;
    case "oil-refinery":          build_order_index = 2;
                                  break;
    case "cemical-plan":          build_order_index = 3;
                                  break;
    case "rocket-silo":            build_order_index = 4;
                                  break;
    case "stone-furnace":         build_order_index = 5;
                                  break;
    case "burner-mining-drill":   build_order_index = 6;
                                  break;
    case "pump-jack":             build_order_index = 7;
                                  break;
    case "offshore-pump"          build_order_index = 8;
                                  break;
  }
}

void Factory::shrink_job(int time_aborted){
  int count_recipe_completed = (time_aborted - time_job_started) / (current_job.recipe.first->energy / crafting_speed);
  int amount_already_produced = current_job.recipe.second * count_recipe_completed;
  current_job.quantity -= amount_already_produced;
  if (current_job.quantity < 0){
    current_job.quantity = 0;
  }
}


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
