#include "events.hpp"


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
  return "research-event";
}



Victory_event::Victory_event(long long int timestamp):
Event(timestamp){
  j["type"] = getType();
}

std::string Victory_event::getType() const {
  return "victory-event";
}



Factory_event::Factory_event(long long int timestamp, int factory_id):
Event(timestamp),
factory_id(factory_id){
  j["factory-id"] = factory_id;
}



Build_factory_event::Build_factory_event(long long int timestamp,  int factory_id, const std::string& factory_type, const std::string& factory_name):
Factory_event(timestamp, factory_id),
factory_type(factory_type),
factory_name(factory_name){
  j["type"] = getType();
  j["factory-name"] = factory_name;
  j["factory-type"] = factory_type;
}

std::string Build_factory_event::getType() const {
  return "build-factory-event";
}



Destroy_factory_event::Destroy_factory_event(long long int timestamp, int factory_id):
Factory_event(timestamp, factory_id){
  j["type"] = getType();
}

std::string Destroy_factory_event::getType() const{
  return "destroy-factory-event";
}



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
  return "stop-factory-event";
}



Start_factory_event::Start_factory_event(long long int timestamp, int factory_id, const std::string& recipe):
Factory_event(timestamp, factory_id),
recipe(recipe){
  j["type"] = getType();
  j["recipe"] = recipe;
}

std::string Start_factory_event::getType() const{
  return "start-factory-event";
}
