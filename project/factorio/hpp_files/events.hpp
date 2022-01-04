#pragma once

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Event{
public:
  Event(long long int);
  virtual ~Event() = default;

  operator json() const;
  bool operator>(const Event&) const;
  virtual std::string getType() const = 0;

  friend std::ostream& operator<<(std::ostream&, const Event&);

  long long int timestamp;
  json j;
};

class Research_event: public Event {
public:
  Research_event(long long int, const std::string&);
  ~Research_event() = default;

  std::string getType() const;

  const std::string technology;
};

class Victory_event: public Event {
public:
  Victory_event(long long int);
  ~Victory_event() = default;

  std::string getType() const;
};

class Factory_event: public Event {
public:
  Factory_event(long long int, int);
  virtual ~Factory_event() = default;

  virtual std::string getType() const = 0;

  int factory_id = 0;
};

class Build_factory_event: public Factory_event {
public:
  Build_factory_event(long long int, int, const std::string&, const std::string&);
  ~Build_factory_event() = default;

  std::string getType() const;

  const std::string factory_type;
  const std::string factory_name;
};

class Destroy_factory_event: public Factory_event {
public:
  Destroy_factory_event(long long int, int);
  ~Destroy_factory_event() = default;

  std::string getType() const;
};

class Stop_factory_event: public Factory_event {
public:
  Stop_factory_event(long long int, int);
  ~Stop_factory_event() = default;
  Stop_factory_event& operator= (const Stop_factory_event&);

  std::string getType() const;
};

class Start_factory_event : public Factory_event {
public:
  Start_factory_event(long long int, int, const std::string&);
  ~Start_factory_event() =  default;

  std::string getType() const;

  const std::string recipe;
};
