#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include "item.hpp"
#include "crafting_category.hpp"
#include <list>
#include "order.hpp"


class Hash {
public:

   size_t operator() (const Item&) const;
};

class MyCompare{
public:

  bool operator()(const std::list<Order>::iterator, const std::list<Order>::iterator);
};
