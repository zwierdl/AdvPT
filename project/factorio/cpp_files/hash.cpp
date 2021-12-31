#include "hash.hpp"



/*size_t Hash::operator() (const Technology& technology) const {
  return std::hash<std::string>()(technology.name);
}*/

/*size_t Hash::operator()(const Recipe& recipe) const{
  return std::hash<std::string>()(recipe.name);
}*/

/*size_t Hash::operator()(const Crafting_category& category) const{
  return std::hash<std::string>()(category.name);
}*/

size_t Hash::operator()(const Item& item) const{
  return std::hash<std::string>()(item.name);
}
