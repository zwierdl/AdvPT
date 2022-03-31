#include <string>
#include <sstream>
#include <vector>
#include <list>

#include "order.hpp"
#include "recipe.hpp"
#include "simulator.hpp"
#include"nlohmann/json.hpp"



int main(int argc, char* argv[]){
  std::fstream file("./factorio-data/input.json");
  json input;
  file >> input;
  int first_amount = input["first_amount"];

  std::string challenge;
  if (argc >= 2){
    challenge = argv[1];
  }
  else{
    challenge = input["challenge"];
  }
  file.close();
  //std::cout << challenge<< std::endl;
  //Factorio_game factorio;

  Simulator simulator(challenge, first_amount);
  simulator.optimize();
/*
  simulator.build_items();
  simulator.set_Indices();
  simulator.sort_Orders_by_factories();
  //simulator.printbuild_order(std::cerr);
  //simulator.printbuild_order(std::cout);
  simulator.restore_original_state();
  //simulator.printbuild_order(std::cout);
  simulator.generate_events();
  simulator.print_events();
  simulator.printbuild_order(std::cerr);
*/
}
