#include <string>
#include <sstream>
#include <vector>

#include "recipe.hpp"
#include "simulator.hpp"
#include"nlohmann/json.hpp"


int main(int argc, char* argv[]){
  std::fstream file("../factorio-data/input.json");
  //std::string input;
  //file >> input;
  //std::cout  << input;
  json input;
  file >> input;
  std::string challenge(input["challenge"]);

  Simulator simulator(challenge);

  /*std::vector<Recipe&> versuch;
  versuch.push_back(simulator.recipes_blueprint["big-electric-pole"]);
  versuch.push_back(simulator.recipes_blueprint["substation"]);

  simulator.recipes_blueprint["substation"].energy = -1000;
  for (Recipe& i : versuch){
    std::cout << i << std::endl;
  }*/

  //std::cout << std::setw(2) << simulator.items <<  std::endl; //"\n\n" << std::setw(2) << simulator.recipes << "\n\n" << std::setw(2) << simulator.technologies << "\n\n" << std::setw(2) << simulator.factories << "\n\n" << std::setw(2) << simulator.initial << std::endl;
  //std::cout << "naja" << std::endl;

  //simulator.printRecipeDoubles();

  //simulator.basicBuildOrder();

}
