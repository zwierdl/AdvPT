#include <unordered_map>

class Item;
class Factory;
class Recipe;
class Technology;

class Factorio_game{
public:
  Factorio_game();
  ~Factorio_game();

  std::unordered_map<std::string, Item*> items_blueprint;
  std::unordered_map<std::string, Factory*> factories_blueprint;
  std::unorederd_map<std::string, Recipe*> recipes_blueprint;
  std::unordered_map<std::string, Technology*> technologies_blueprint;
}
