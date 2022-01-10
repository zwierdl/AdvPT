#include <string>
#include <sstream>
#include <vector>
#include <list>

#include "order.hpp"
#include "events.hpp"
#include "event_generator.hpp"
#include "recipe.hpp"
#include "simulator.hpp"
#include "factorio_game.hpp"
#include"nlohmann/json.hpp"



int main(int argc, char* argv[]){
  std::fstream file("../factorio-data/input.json");
  json input;
  file >> input;
  std::string challenge(input["challenge"]);

  Factorio_game factorio;

  Simulator simulator(challenge, factorio);

  simulator.build_items();

  //simulator.printItems(std::cout);
  simulator.restore_original_state();
  //simulator.print_factories(std::cout);
  //simulator.printBuildOrder(std::cout);
  //std::cout << simulator.initial << std::endl;
  Event_generator event_generator(factorio, simulator.initial);
  event_generator.generate_events();
  //event_generator.print_events();
  //simulator.print_technologies(std::cerr);
  //simulator.printBuildOrder(std::cout);
}
