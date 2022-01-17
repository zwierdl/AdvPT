#include "order.hpp"
#include "technology.hpp"
#include "recipe.hpp"
#include "item.hpp"

Order::Order(Item* item, double quantity, std::pair<Recipe*, int> recipe):
item(item),
quantity(quantity),
recipe(recipe){}

Order::Order(Item* item, double quantity, std::pair<Recipe*, int> recipe, Order* purpose):
item(item),
quantity(quantity),
recipe(recipe),
purpose(purpose){
  if (typeid(item) == typeid(Item*)){
    if (recipe.first != nullptr){
      ingredients_still_needed = recipe.first->ingredients.size();
    }
  }
  if (item->type == "technology"){
    Technology* technology_p = static_cast<Technology*>(item);
    ingredients_still_needed = technology_p->prerequisites.size() + technology_p->ingredients.size();
  }
}

Order::Order(){}
