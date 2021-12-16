# include "simulator.hpp"

Simulator::Simulator(){
  std::ifstream in("../factorio-data/factory.json");
  json myjson;
  in >> myjson;
  std::unordered_map<std::string, json> factories_json_map = myjson;

  //Factory factory;

  for (std::pair<std::string, json> i : factories_json_map){
    factories_blueprint.push_back(Factory(i));
    //factories_blueprint[i.first] = Factory(i);
    /*factory.name = i.first;
    factory.crafting_speed = i.second["crafting_speed"];

    std::unordered_map<std::string, json> crafting_categories_json_map = i.second["crafting_categories"];

    for (std::pair<std::string, json> j : crafting_categories_json_map){

    }*/

  }
  for (Factory i : factories_blueprint){
    std::cout << i << std::endl;
  }
  /*std::vector<json> json_vector = myjson;

  for (json i : json_vector){
    std::cout << i << std::endl;
  }
  */



  /*for (json::iterator it = myjson.begin(); it != myjson.end(); ++it){
    json_vector.push_back(it);
  }*/

  /*std::unordered_map<std::string, json> map = myjson;

  for (std::pair<std::string, json> i : map){
    std::cout << i.first << i.second <<std::endl;
  }
  */
  //Factory factory;
  //Crafting_category crafting_category;
  /*for (json::iterator it = myjson.begin(); it != myjson.end(); ++it){
    factory.name = it.key();
    factory.crafting_speed = it(0);
    for (json::iterator it2 = it["crafting_categories"].begin(), it != it["crafting_categories"].end(); ++it){
      crafting_category.name = it2.key();
      factory.crafting_categories.pushback(crafting_category);
    }
    factories_blueprint.pushback(factory);
  }
  */
  myjson.clear();

  std::cout << "json cleared:\n\n" << myjson << std::endl;

  in.close();

  in.open("../factorio-data/item.json");
  in >> myjson;

  std::cout << "item-json: \n" << myjson << std::endl;

  std::unordered_map<std::string, json> items_json_map = myjson;

  for (std::pair<std::string, json> i : items_json_map){
    items_blueprint.push_back(Item(i));
  }

  for (Item i : items_blueprint){
    std::cout << i << std::endl;
  }

  /*Item item;
  for (json::iterator it = myyson.begin(); it != myjson.end(); ++it){
    item.name = it.key();
    item.type = it["type"];
    items_blueprint.pushback(item);
  }
  */
  myjson.clear();
  in.close();

  in.open("../factorio-data/recipe.json");
  in >> myjson;

  std::unordered_map<std::string, json> recipes_json_map = myjson;

  for (std::pair<std::string, json> i : recipes_json_map){
    recipes_blueprint.push_back(Recipe(i));
  }

  std::cout << "/nRecipes:/n";

  for (Recipe i : recipes_blueprint){
    std::cout << i << std::endl;
  }

  /*
  Recipe recipe;
  Item ingredient;
  Item product;   //hier stimmt doch was nicht!
  for (json::iterator it = myyson.begin(); it != myjson.end(); ++it){
    recipe.name = it.key();
    recipe.crafting_category = it["category"];
    recipe.endabled = it["enabled"];
    recipe.energy = it["energy"];

    for (json::iterator it2 = it["ingredients"].begin(); it2 != it["ingredients"].end(), ++it2){
      ingredient.first.name = it2["name"];
      ingredient.second = it2["amaount"];
      recipe.ingredients.pushback(ingredient);
    }

    for (json::iterator it2 = it["products"].begin(); it2 != it["products"].end(); ++it2){
      product.first.name = it2["name"];
      product.second = it2["amount"];
      recipe.products.pushback(product);
    }

    recipes_blueprint.pushback(recipe);
  }
  */
  myjson.clear();
  in.close();

  in.open("../factorio-data/technology.json");
  in >> myjson;

  std::unordered_map<std::string, json> technologies_json_map;

  for (std::pair<std::string, json> i : technologies_json_map){
    technologies_blueprint.push_back(Technology(i));
  }
  std::cout << "\nTechnologies:\n";
  for (Technology i : technologies_blueprint){
    std::cout << i << std::endl;
  }

  /*
  Technology technology;
  Recipe effect;
  st::pair<Item, int> ingredient;
  Technology prerequisite;
  for (json::iterator it = myjson.begin(); it != myjson.end(); ++it){
    technology.name = it.key();

    for (json::iterator it2 = it["effects"].begin(); it2 != it.end["effects"](); ++it2){
      effect.name = it2.key();
      technology.effects.pushback(effect);
    }

    for (json::iterator it2 = it["ingredients"].begin(); it2 != it["ingredients"].end(); ++it2){
      ingredient.first.name = it2["name"];
      ingredient.second = it2["amount"];
      technology.ingredients.pushback(ingredient);
    }

    for (json::iterator it2 = it["preequisites"].begin(); it2 != it["perquesities"].end(); ++it2){
      prerequisite.name = it;  //?????
      technology.prerequisites.pushback(prerequisite);

    }
    technologies_blueprint.pushback(technology);
  }


  technologies_blueprint = myjson;

*/
  myjson.clear();
  in.close();

/*
  in.open("../json/factorio-simulator/inputs/challange-1");
  in >> initial_json;
  in.close()
*/
}
