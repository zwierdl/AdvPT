#include <string>
#include <sstream>
#include <vector>
#include <list>

#include "order.hpp"
#include "recipe.hpp"
#include "simulator.hpp"
#include"nlohmann/json.hpp"



int main(int argc, char* argv[]){
  std::string challenge;

  if (argc >= 2){
    challenge = argv[1];
  }
  else{
    std::fstream file("../factorio-data/input.json");
    json input;
    file >> input;
    challenge = input["challenge"];
  }
  //Factorio_game factorio;

  Simulator simulator(challenge);
  //if (challenge == "../factorio-data/challenge-7.json"){
    //simulator.optimize();
  //}
  //else {
    simulator.build_items();
    simulator.sort_Orders_by_factories();
    simulator.restore_original_state();
    simulator.generate_events();
    simulator.printbuild_order(std::cout);
    simulator.print_events();
  //}
}
