#include "hash.hpp"


size_t Hash::operator()(const Item& item) const{
  return std::hash<std::string>()(item.name);
}
