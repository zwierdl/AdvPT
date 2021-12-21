#include "technology.hpp"

Technology::Technology(const std::pair<std::string, json>& input, std::unordered_map<std::string, Item>& items_blueprint, std::unordered_map<std::string, Recipe>& recipes_blueprint): Item(input.first, "technology") {
  std::vector<json> effects_json_vector = input.second["effects"];
  for (json i : effects_json_vector){
    effects.push_back(i["recipe"]);
  }
  std::vector<json> ingredients_json_vector = input.second["ingredients"];

  for (json i : ingredients_json_vector){
    ingredients.push_back(std::pair<std::string,int>(i["name"], i["amount"]));
  }

  std::vector<json> prerequisites_json_vector = input.second["prerequisites"];
  for (json i : prerequisites_json_vector){
    prerequisites_string.push_back(i);
    //prerequisites.push_back(std::pair<Item, int>(items_blueprint[i["name"]], i["amount"]));
  }
}

Technology::Technology(){}

json& operator<<(json& out, const Technology& technology){
  //out[technology.name]["category"] = technology.Crafting_category.name;
  //out[technology.name]["enabled"] = recipe.enabled;
  //out[technology.name]["energy"] = recipe.energy;
  json effects_json = json::array();
  for (std::string effect : technology.effects){
    json effect_json;
    effect_json["type"] = "unlocke-recipe";
    effect_json["recipe"] = effect;
    effects_json += effect_json;
  }
  out[technology.name]["effects"] = effects_json;

  json ingredients_json = json::array();
  for (std::pair<std::string, int> ingredient : technology.ingredients){
    json ingredient_json;
    ingredient_json["name"] = ingredient.first;
    ingredient_json["amount"] = ingredient.second;
    ingredients_json += ingredient_json;
  }
  out[technology.name]["ingredients"] = ingredients_json;

  json prerequisites_json = json::array();
  for (std::string prerequisite : technology.prerequisites_string){
    //json prerequisite_json;
    prerequisites_json += prerequisite;
    //prerequisites_json += prerequisite_json;
  }
  out[technology.name]["prerequisites"] = prerequisites_json;


  return out;
}

std::ostream& operator<<(std::ostream& out, const Technology& technology){
  json j;
  j << technology;
  out << std::setw(2) << j << std::endl;
  return out;
}
