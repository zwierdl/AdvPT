#include <iomanip>

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
  return out;
}
