#include <string>
#include <sstream>
#include <vector>

#include "recipe.hpp"
#include "simulator.hpp"
#include"nlohmann/json.hpp"


int main(int argc, char* argv[]){
  std::fstream file("../factorio-data/input.json");
  json input;
  file >> input;
  std::string challenge(input["challenge"]);

  Simulator simulator(challenge);
  simulator.build_items();
  simulator.restore_original_state();

  //simulator.printItems(std::cout);
  simulator.printBuildOrder(std::cout);
  //simulator.addTechnologies();

}
