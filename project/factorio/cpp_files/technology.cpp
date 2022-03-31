#include "technology.hpp"
#include <iomanip>

Technology::Technology(const std::pair<std::string, json>& input, std::unordered_map<std::string, Item*>& items_blueprint, std::unordered_map<std::string, Recipe*>& recipes_blueprint):
  Item(input.first, "technology") {
    std::vector<json> effects_json_vector = input.second["effects"];
    for (json i : effects_json_vector){
      if (recipes_blueprint.contains(i["recipe"])) effects.push_back(recipes_blueprint[i["recipe"]]);
    }
    std::vector<json> ingredients_json_vector = input.second["ingredients"];

    for (json i : ingredients_json_vector){
      ingredients.push_back(std::pair<Item*,int>(items_blueprint[i["name"]], i["amount"]));
    }

}

Technology::Technology(){}

bool Technology::operator== (const Technology& other) const{
  return (*this).name == other.name;
}

json& operator<<(json& out, const Technology& technology){
  json effects_json = json::array();
  for (Recipe* effect : technology.effects){
    json effect_json;
    effect_json["type"] = "unlocke-recipe";
    effect_json["recipe"] = effect->name;
    effects_json += effect_json;
  }
  out[technology.name]["effects"] = effects_json;

  json ingredients_json = json::array();
  for (std::pair<Item*, int> ingredient : technology.ingredients){
    json ingredient_json;
    ingredient_json["name"] = ingredient.first->name;
    ingredient_json["amount"] = ingredient.second;
    ingredients_json += ingredient_json;
  }
  out[technology.name]["ingredients"] = ingredients_json;

  json prerequisites_json = json::array();
  for (Technology* prerequisite : technology.prerequisites){
    prerequisites_json += prerequisite->name;
  }
  out[technology.name]["prerequisites"] = prerequisites_json;
  out[technology.name]["researched"] = technology.researched;


  return out;
}

std::ostream& operator<<(std::ostream& out, const Technology& technology){
  json j;
  j << technology;
  out << std::setw(2) << j << std::endl;
  return out;
}
