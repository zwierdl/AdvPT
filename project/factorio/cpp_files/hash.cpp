#include "hash.hpp"


size_t Hash::operator()(const Item& item) const{
  return std::hash<std::string>()(item.name);
}

bool MyCompare::operator()(const std::list<Order>::iterator a, const std::list<Order>::iterator b){
  return (*a).index >= (*b).index;
}
