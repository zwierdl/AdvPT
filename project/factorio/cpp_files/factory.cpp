#include <iomanip>

#include "factory.hpp"
#include "recipe.hpp"

Factory::Factory(const std::pair<std::string, json>& input):
  Item(input.first, "factory"),
  crafting_speed(input.second["crafting_speed"]) {
  //crafting_speed = input.second["crafting_speed"];
    std::map <std::string, json> crafting_categories_json_map = input.second["crafting_categories"];
    for (std::pair<std::string, json> i : crafting_categories_json_map){
      crafting_categories.push_back(Crafting_category(i.first));
    }
}


Factory::Factory(){};


Factory::Factory(const std::string& name, int id, const std::string& factory_name, std::unordered_map<std::string, Factory*>& factories_blueprint):
Item(name, "factory"),
id(id),
factory_name(factory_name),
crafting_speed(factories_blueprint[name]->crafting_speed),
crafting_categories(factories_blueprint[name]->crafting_categories){
  if (name == "assembling-machine-1" || name == "player")   {build_order_index = 1;}
  else if (name == "assembling-machine-2")                  {build_order_index = 2;}
  else if (name == "oil-refinery")                          {build_order_index = 3;}
  else if (name == "chemical-plant")                        {build_order_index = 4;}
  else if (name == "rocket-silo")                           {build_order_index = 5;}
  else if (name == "stone-furnace")                         {build_order_index = 6;}
  else if (name == "burner-mining-drill")                   {build_order_index = 7;}
  else if (name == "pumpjack")                              {build_order_index = 8;}
  else if (name == "offshore-pump")                         {build_order_index = 9;}
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
  return out;
}

std::ostream& operator<<(std::ostream& out, const Factory& factory){
  json j;
  j << factory;
  out << std::setw(2) << j << std::endl;
  return out;
}
