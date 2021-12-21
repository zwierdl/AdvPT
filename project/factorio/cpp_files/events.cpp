struct event {
  const uint_t timestamp;
};

struct victory_event:  public event {
  victory_event(const uint_t& timestamp) : event::timestamp(timestamp) {};
};

struct factory_event : public event {
  factory_event(const uint_t& timestamp, const uint_t& factory_id) : event::timestamp(timestamp), factory_id(factory_id) {};

  const uint_t factory_id;
};

struct build_factory_event : public factory_event {
  build_factory_event(
    const uint_t& timestamp,
    const uint_t& factory_id,
    const std::string& factory_type,
    const std::string& factory_name
  ) :
  event::timestamp(timestamp),
  factory_event::factory_id(factory_id),
  factory_type(factory_type),
  factory_name(factory_name)
   {};

  const std::string factory_type;
  const std::string factory_name;
};

struct destroy_factory_event : public factory_event {
  destroy_factory_event(
    const uint_t& timestamp,
    const uint_t& factory_id
  ) :
  event::timestamp(timestamp),
  factory_event::factory_id(factory_id) {};
};

struct stop_factory_event : public factory_event {
  stop_factory_event(
    const uint_t& timestamp,
    const uint_t& factory_id
  ) :
  event::timestamp(timestamp),
  factory_event::factory_id(factory_id) {};
};

struct start_factory_event : public factory_event {
  build_factory_event(
    const uint_t& timestamp,
    const uint_t& factory_id,
    const std::string& recipe
  ) :
  event::timestamp(timestamp),
  factory_event::factory_id(factory_id),
  recipe(recipe) {};

  const std::string recipe;
};
