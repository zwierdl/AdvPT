
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "nlohmann/json.hpp"


using uint_t = uint16_t;
using json = nlohmann::json;

struct event {
  event(const uint_t& timestamp) : timestamp(timestamp) { j["timestamp"] = timestamp; };
  const uint_t timestamp = 0;

  virtual std::string getType() const = 0;
  friend std::ostream& operator<<(std::ostream& out, const event& e){
    out << e.j << std::endl;
    return out;
  };
  json j;
};

struct victory_event:  public event {
  victory_event(const uint_t& timestamp) : event(timestamp) {j["type"] = getType();};
  std::string getType() const { return "victory_event";}
};

struct factory_event: public event {
  factory_event(const uint_t& timestamp, const uint_t& factory_id) : event(timestamp), factory_id(factory_id) {j["factory_id"] = factory_id; };

  const uint_t factory_id = 0;
};

struct build_factory_event : public factory_event {
  build_factory_event(
    const uint_t& timestamp,
    const uint_t& factory_id,
    const std::string& factory_type,
    const std::string& factory_name
  ) :
  factory_event(timestamp, factory_id),
  factory_type(factory_type),
  factory_name(factory_name)
   { j["type"] = getType(); j["name"] = factory_name; j["factory_type"] = factory_type;};

  std::string getType() const { return "build_factory_event";}
  const std::string factory_type;
  const std::string factory_name;
};

struct destroy_factory_event : public factory_event {
  destroy_factory_event(
    const uint_t& timestamp,
    const uint_t& factory_id
  ) :
  factory_event(timestamp, factory_id) {j["type"] = getType();};
std::string getType() const { return "destroy_factory_event";}
};

struct stop_factory_event : public factory_event {
  stop_factory_event(
    const uint_t& timestamp,
    const uint_t& factory_id
  ) :
  factory_event(timestamp, factory_id) {j["type"] = getType();};

  std::string getType() const { return "stop_factory_event";}
};

struct start_factory_event : public factory_event {
  start_factory_event(
    const uint_t& timestamp,
    const uint_t& factory_id,
    const std::string& recipe
  ) :
  factory_event(timestamp, factory_id),
  recipe(recipe) { j["type"] = getType(); j["recipe"] = recipe; };

  std::string getType() const { return "start_factory_event";}
  const std::string recipe;
};


int main(int argc, char *argv[])
{
  //factory_event fe(1,1);
  victory_event ve(100);
  std::cout  << ve;

}
