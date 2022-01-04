#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include "item.hpp"
#include "crafting_category.hpp"
//#include "technology.hpp"
//#include "recipe.hpp"

//class Item;

class Hash {
public:
   //size_t operator() (const Technology&) const;
   //size_t operator() (const Recipe&) const;
   size_t operator() (const Crafting_category&) const;
   size_t operator() (const Item&) const;
};
