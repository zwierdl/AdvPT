#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include "item.hpp"
#include "crafting_category.hpp"

class Hash {
public:

   size_t operator() (const Item&) const;
};
