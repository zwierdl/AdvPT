#include "recipe.hpp"

Recipe::Recipe(std::pair<std::string, json> input): Item(input.first, "Recipe"), crafting_category(input.second["category"]), enabled(input.second["enabled"]), energy(input.second["energy"]){
    std::vector<json> ingredients_json_vector = input.second["ingredients"];
    for (json i : ingredients_json_vector){
      ingredients.push_back(std::pair<Item,int>(Item(i["name"], "item"),i["amount"]));
    }
    std::vector<json> products_json_vector = input.second["products"];

    for (json i : products_json_vector){
      products.push_back(std::pair<Item,int>(Item(i["name"], "item"), i["amount"]));
    }
}

std::ostream& operator<<(std::ostream& out, const Recipe& recipe){
  json j;
  j << recipe;
  out << j << std::endl;
  return out;
}

json& operator<<(json& out, const Recipe& recipe){
  out[recipe.name]["category"] = recipe.crafting_category.name;
  out[recipe.name]["enabled"] = recipe.enabled;
  out[recipe.name]["energy"] = recipe.energy;
  json ingredients_json = json::array();
  for (std::pair<Item, int> ingredient : recipe.ingredients){
    json ingredient_json;
    ingredient_json["name"] = ingredient.first.name;
    ingredient_json["amount"] = ingredient.second;
    ingredients_json += ingredient_json;
  }
  out[recipe.name]["ingredients"] = ingredients_json;

  json products_json = json::array();
  for (std::pair<Item, int> product : recipe.products){
    json product_json;
    product_json["name"] = product.first.name;
    product_json["amount"] = product.second;
    products_json += product_json;
  }
  out[recipe.name]["products"] = products_json;
  return out;
}
