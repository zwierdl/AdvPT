#include "order.hpp"

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
    /*for (Technology* prerequisite : technology_p->prerequisites){
      if (!prerequisite->researched){
        ++ingredients_still_needed;
      }
    }*/
  }
  /*if (recipe != nullptr){
    ingredients_still_needed = recipe->ingredients.size();  //muss man fuer prerequisites wahrscheinlich noch aendern
  }
  else if (item->type == "technology"){
    Technology* technology_p = static_cast<Technology*>(item);
    for (Technology* prerequisite : technology_p->prerequisites){
      if (!prerequisite->researched){
        ++ingredients_still_needed;
      }
    }
  }*/
}

Order::Order(){}
