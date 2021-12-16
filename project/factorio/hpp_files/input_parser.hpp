# pragma once

#include <vector>

#include <nlohmann/json.hpp>
using json = lohmann::json;

class Input_parser {
public:
  Input_Parser(std::vector<Factory>&, std::vector<Item>&, std::vector<Recipe>&, std::vector<Technology>&, std::vector<Item>&, std::vector<Factory>&, std::vector<Item>&);

}
