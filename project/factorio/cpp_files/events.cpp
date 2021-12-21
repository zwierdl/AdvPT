class event {
  const uint_t timestamp;
};

class factory_event : public(event){
public:
  factory_event(const uint_t& timestamp, const uint_t& factory_id) : event::timestamp(timestamp), factory_id(factory_id) {};

  const uint_t factory_id;
};

class build_factory_event
