#include "technology.hpp"

Technology::Technology(std::pair<std::string, json> input): Item(input.first, "Technology") {
  std::vector<json> effects_json_vector = input.second["effects"];
  for (json i : effects_json_vector){
    effects.push_back(i["recipe"]);
  }
  std::vector<json> ingredients_json_vector = input.second["ingredients"];

  for (json i : ingredients_json_vector){
    ingredients.push_back(std::pair<Item,int>(Item(i["name"], "item"), i["amount"]));
  }

  std::vector<json> prerequisites_json_vector = input.second["prerequisites"];

  for (json i : ingredients_json_vector){
    ingredients.push_back(std::pair<Item, int>(Item(i["name"],"item"), i["amount"]));
  }

}

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
  for (std::pair<Item, int> ingredient : technology.ingredients){
    json ingredient_json;
    ingredient_json["name"] = ingredient.first.name;
    ingredient_json["amount"] = ingredient.second;
    ingredients_json += ingredient_json;
  }
  out[technology.name]["ingredients"] = ingredients_json;

  json prerequisites_json = json::array();
  for (std::string prerequisite : technology.prerequisites){
    json prerequisite_json;
    prerequisite_json["name"] = prerequisite;
    //prerequisite_json["amount"] = prerequisite.second;
    prerequisites_json += prerequisite_json;
  }


  return out;
}

std::ostream& operator<<(std::ostream& out, const Technology& technology){
  json j;
  j << technology;
  out << j << std::endl;
  return out;
}
