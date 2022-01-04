#include "crafting_category.hpp"

//Crafting_category::Crafting_category(std::pair<std::string, json> input): Item(input.first, "Crafting_category"){}
Crafting_category::Crafting_category(const std::string& name): Item(name, "Crafting_ategory"){}

Crafting_category::Crafting_category(){}

bool Crafting_category::operator==(const Crafting_category& other) const{
  return this->name == other.name;
}
