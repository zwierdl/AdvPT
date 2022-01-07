#include "item.hpp"
#include "recipe.hpp"
#include "hash.hpp"

Item::Item(std::pair<std::string, json> input): name(input.first), type("item"){}
Item::Item(std::string name, std::string type): name(name), type(type){}
Item::Item(){}

bool Item::operator==(const Item& other) const {
  return this->name == other.name;
}

void Item::calculate_energy(std::unordered_set<Item, Hash>& available_categories){
  double new_energy = -1;
  for (auto& i : this->recipes_produce){
    new_energy = 0;
    Recipe* recipe_p = i.first;
    int amount_produced = i.second;
    if (recipe_p->enabled && available_categories.contains(recipe_p->crafting_category)){
      for (auto& j : recipe_p->ingredients){
        Item* ingredient = j.first;
        int amount_consumed = j.second;
        if (ingredient->energy == -1){
          new_energy = -1;
          break;
        }
        new_energy += ingredient->energy * amount_consumed;
      }

      if (new_energy != -1){
        if (this->energy == -1 || (new_energy + recipe_p->energy) / amount_produced < this->energy){
          this->energy = (new_energy + recipe_p->energy) / amount_produced;
          this->best_recipe.first = recipe_p;
          this->best_recipe.second = amount_produced;

          for (auto& j : this->recipes_consume){
            Recipe* recipe_p = j.first;
            for (auto& k : recipe_p->products){
              Item* product = k.first;
              product->calculate_energy(available_categories);
            }
          }
        }
      }
    }
  }
}


std::ostream& operator<<(std::ostream& out, const Item& item){
  json j;
  j << item;
  out << std::setw(2) << j << std::endl;
  return out;
}

json& operator<<(json& out, const Item& item){
  out[item.name]["type"] = item.type;
  out[item.name]["energy"] = item.energy;
  /*json j;
  Recipe* recipe_p = item.best_recipe.first;
  if (recipe_p != nullptr){
    j << *recipe_p;
  }*/
  //out[item.name]["best_recipe"] = j;
  //out[item.name]["energy"] = item.energy;
  /*out[item.name]["stock"] = item.stock;

  json recipes_json = json::array();
  for (auto& i : item.recipes){
    Recipe* recipe = static_cast<Recipe*>(i.first);
    int amount = i.second;
    json entry;
    entry["recipe"] << *recipe;
    entry["amount"] = amount;
    recipes_json += entry;
  }
  out[item.name]["recipes"] = recipes_json;*/
  return out;
}
