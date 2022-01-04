#include <string>
#include <sstream>
#include <vector>

#include "events.hpp"
#include "event_generator.hpp"
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
  //std::cout << simulator.initial << std::endl;
  Event_generator event_generator(simulator.buildOrder, simulator.initial, simulator.factories_blueprint, simulator.items_blueprint);
  event_generator.generate_events();
  event_generator.print_events();
}
