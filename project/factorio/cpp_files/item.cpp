#include "item.hpp"


  Item::Item(std::pair<std::string, json> input): name(input.first), type(input.second["type"]){}
  Item::Item(std::string name, std::string type): name(name), type(type){}


std::ostream& operator<<(std::ostream& out, const Item& item){
  json j;
  j << item;
  out << j << std::endl;
  return out;
}

json& operator<<(json& out, const Item& item){
  out[item.name]["type"] = item.type;
  return out;
}
