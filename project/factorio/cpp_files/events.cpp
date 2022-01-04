#include "events.hpp"


//using uint_t = uint16_t;
//using json = nlohmann::json;

/*struct event {
  event(const uint_t& timestamp) : timestamp(timestamp) { j["timestamp"] = timestamp; };
  const uint_t timestamp = 0;

  virtual std::string getType() const = 0;
  friend std::ostream& operator<<(std::ostream& out, const event& e){
    out << e.j << std::endl;
    return out;
  };
  json j;
};*/

Event::Event(long long int timestamp):
timestamp(timestamp){
  j["timestamp"] = timestamp;
}

Event::operator json() const{
  return j;
}

bool Event::operator>(const Event& other) const {
  return timestamp > other.timestamp;
}

std::ostream& operator<<(std::ostream& out, const Event& event){
  out << event.j << std::endl;
  return out;
}

Research_event::Research_event(long long int timestamp, const std::string& technology):
Event(timestamp),
technology(technology){
  j["type"] = getType();
  j["technology"] = technology;
}

std::string Research_event:: getType() const {
  return "reserch_event";
}


/*struct victory_event:  public event {
  victory_event(const uint_t& timestamp) : event(timestamp) {j["type"] = getType();};
  std::string getType() const { return "victory_event";}
};*/

Victory_event::Victory_event(long long int timestamp):
Event(timestamp){
  j["type"] = getType();
}

std::string Victory_event::getType() const {
  return "victory_event";
}

/*struct factory_event: public event {
  factory_event(const uint_t& timestamp, const uint_t& factory_id) : event(timestamp), factory_id(factory_id) {j["factory_id"] = factory_id; };

  const uint_t factory_id = 0;
};*/

Factory_event::Factory_event(long long int timestamp, int factory_id):
Event(timestamp),
factory_id(factory_id){
  j["factory_id"] = factory_id;
}

/*struct build_factory_event : public factory_event {
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
};*/

Build_factory_event::Build_factory_event(long long int timestamp,  int factory_id, const std::string& factory_type, const std::string& factory_name):
Factory_event(timestamp, factory_id),
factory_type(factory_type),
factory_name(factory_name){
  j["type"] = getType();
  j["name"] = factory_name;
  j["factory_type"] = factory_type;
}

std::string Build_factory_event::getType() const {
  return "build_factory_event";
}

/*struct destroy_factory_event : public factory_event {
  destroy_factory_event(
    const uint_t& timestamp,
    const uint_t& factory_id
  ) :
  factory_event(timestamp, factory_id) {j["type"] = getType();};
std::string getType() const { return "destroy_factory_event";}
};*/

Destroy_factory_event::Destroy_factory_event(long long int timestamp, int factory_id):
Factory_event(timestamp, factory_id){
  j["type"] = getType();
}

std::string Destroy_factory_event::getType() const{
  return "destroy_factory_event";
}

/*struct stop_factory_event : public factory_event {
  stop_factory_event(
    const uint_t& timestamp,
    const uint_t& factory_id
  ) :
  factory_event(timestamp, factory_id) {j["type"] = getType();};

  std::string getType() const { return "stop_factory_event";}
};*/

Stop_factory_event::Stop_factory_event(long long int timestamp, int factory_id):
Factory_event(timestamp, factory_id){
  j["type"] = getType();
}

Stop_factory_event& Stop_factory_event:: operator=(const Stop_factory_event& other){
  j = other.j;
  timestamp = other.timestamp;
  factory_id = other.factory_id;
  return *this;

}

std::string Stop_factory_event::getType() const{
  return "stop_factory_event";
}

/*struct start_factory_event : public factory_event {
  start_factory_event(
    const uint_t& timestamp,
    const uint_t& factory_id,
    const std::string& recipe
  ) :
  Factory_event(timestamp, factory_id),
  recipe(recipe) { j["type"] = getType(); j["recipe"] = recipe; };

  std::string getType() const { return "start_factory_event";}
  const std::string recipe;
};*/

Start_factory_event::Start_factory_event(long long int timestamp, int factory_id, const std::string& recipe):
Factory_event(timestamp, factory_id),
recipe(recipe){
  j["type"] = getType();
  j["recipe"] = recipe;
}

std::string Start_factory_event::getType() const{
  return "start_factory_event";
}


/*int main(int argc, char *argv[])
{
  //factory_event fe(1,1);
  victory_event ve(100);
  std::cout  << ve;

}*/
