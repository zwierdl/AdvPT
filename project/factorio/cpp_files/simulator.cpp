# include "simulator.hpp"

Simulator::Simulator(std::string challenge){
  /*
  std::ifstream file ("../factorio-data/item.json");
  file >> items;
  file.close();
  */
  //std::cout << items << "items\n\n" << std::endl;


  /*
  file.open("../factorio-data/recipe.json");
  file >> recipes;
  file.close();
  */
  //std::cout << "recipe\n\n" << std::endl;

  /*
  file.open("../factorio-data/technology.json");
  file >> technologies;
  file.close();
  */
  //std::cout << "technology\n\n" << std::endl;

  /*
  file.open("../factorio-data/factory.json");
  file >> factories;
  file.close();
  */
  //std::cout << "factories\n\n" << std::endl;

  /*
  std::string challenge_path("../factorio-data/");
  challenge_path+= challenge;

  std::cout << challenge_path << std::endl;

  file.open(challenge_path.c_str());
  file >> initial;
  file.close();

  for (json::iterator i = items.begin(); i != items.end(); ++i){
    (*i)["stock"] = 0;
  }


  std::string name;
  for (json::iterator i = initial["initial-items"].begin(); i != initial["initial-items"].end(); ++i){
    name = (*i)["name"];
    items.operator[](name)["stock"] = (*i)["amount"];
  }

  */

  std::ifstream in("../factorio-data/factory.json");
  json myjson;
  in >> myjson;

  std::unordered_map<std::string, json> factories_json_map = myjson;
  /*for (std::pair<std::string, json> i : factories_json_map){
    factories_blueprint.push_back(Factory(i));
  }*/

  for (std::pair<std::string, json> i : factories_json_map){
    factories_blueprint[i.first] = Factory(i);
  }

  /*std::cout << "\nfactories:\n" << std::endl;
  for (std::pair<std::string, Factory> i : factories_blueprint){
    std::cout << "\n" << i.first << std::endl;
    std::cout << i.second << std::endl;
  }*/

  myjson.clear();
  in.close();


  in.open("../factorio-data/recipe.json");
  in >> myjson;

  std::unordered_map<std::string, json> recipes_json_map = myjson;
  for (std::pair<std::string, json> i : recipes_json_map){
    //recipes_blueprint.push_back(Recipe(i));
    recipes_blueprint[i.first] = Recipe(i, items_blueprint);
  }

  /*std::cout << "\nRecipes:\n" << std::endl;
  for (std::pair<std::string,Recipe> i : recipes_blueprint){
    std::cout << i.first << std::endl << i.second << std::endl;
  }*/

  for (auto i : recipes_blueprint){
    for (std::pair<std::string, int> j : i.second.products){
      recipes_by_item.insert(std::pair<std::string, std::pair<Recipe, int>>(j.first, std::pair<Recipe, int>(i.second, j.second)));
    }
  }

  std::cout << "recipes_by_item:\n" << std::endl;
  for (auto i : recipes_by_item){
    std::cout << i.first << std::endl << i.second.first << std::endl << i.second.second;
  }

  myjson.clear();
  in.close();


  in.open("../factorio-data/technology.json");
  in >> myjson;
  //std::cout << "technologies reading" << std::endl;
  std::unordered_map<std::string, json> technologies_json_map = myjson;
  for (std::pair<std::string, json> i : technologies_json_map){
    //technologies_blueprint.push_back(Technology(i));
    technologies_blueprint[i.first] = Technology(i, items_blueprint, recipes_blueprint);
  }

  /*for (std::pair<std::string, Technology> i : technologies_blueprint){
    for (std::string j : i.second.prerequisites_string){
      i.second.prerequisites_technology.push_back(techn)
    }
  }*/

  /*std::cout << "\nTechnologies:\n" << std::endl;
  for (std::pair<std::string, Technology> i : technologies_blueprint){
    std::cout << i.first << std::endl << i.second << std::endl;
  }*/

  myjson.clear();
  in.close();

  in.open("../factorio-data/item.json");
  in >> myjson;

  std::unordered_map<std::string, json> items_json_map = myjson;
  for (std::pair<std::string, json> i : items_json_map){
    //items_blueprint.push_back(Item(i));
    items_blueprint[i.first] = Item(i);
  }

  for (std::pair<std::string, Factory> i : factories_blueprint){
    //items_blueprint[i.first] = Item(i.first, "factory");
    items_blueprint[i.first] = i.second;
  }
  for (std::pair<std::string, Recipe> i : recipes_blueprint){   //wahrscheinlich unnoeting
    //items_blueprint[i.first] = Item(i.first, "recipe");
    items_blueprint[i.first] = i.second;
  }
  for (std::pair<std::string, Technology> i : technologies_blueprint){
    //items_blueprint[i.first] = Item(i.first, "technology");
    items_blueprint[i.first] = i.second;
  }

  /*std::cout << "\nItems:\n" << std::endl;
  for (std::pair<std::string, Item> i : items_blueprint){
    std::cout << i.first << std::endl << i.second << std::endl;
  }*/

  myjson.clear();
  in.close();


  in.open(challenge.c_str());
  in >> initial;

  for (json::iterator i = initial["initial-items"].begin(); i != initial["initial-items"].end(); ++i){
    items_blueprint[(*i)["name"]].stock = (*i)["amount"];
  }

  for (json::iterator i = initial["initial-factories"].begin(); i != initial["initial-factories"].end(); ++i){
    items_blueprint[(*i)["factory-type"]].stock = 1;
    factories.push_back(Factory((*i)["factory-type"], (*i)["factory-id"], (*i)["factory-name"], factories_blueprint));
  }

  for (json::iterator i = initial["goal-items"].begin(); i != initial["goal-items"].end(); ++i){
    goal.push_back(std::pair<Item, int>(Item((*i)["name"], "item"), (*i)["amount"]));
  }
  /*
  std::cout << "factories:\n" << std::endl;
  for (Factory i : factories){
    std::cout << i << std::endl;
  }

  std::cout << "items:\n" << std::endl;
  for (std::pair<Item, int> i : goal){
    std::cout << i.first << i.second << std::endl;
  }
  */
  in.close();

}


void Simulator::buildBasicBuildOrder(){
  std::deque<std::pair<Item, int>> processing;
  for (std::pair<Item, int> i : goal){
    processing.push_front(i);
  }
  while(!processing.empty()){
    std::pair<Item, int> toProduce = processing.back();
    std::pair<Recipe, int> recipe = recipes_by_item.find(toProduce.first.name)->second;
    //Item* toProduce_Item = new Item(toProduce.first);
    //int toProduce_int = toProduce.second;
    //Item* myRecipe = new Item(recipe.first);
    for (std::pair<std::string, int> ingredient : recipe.first.ingredients){
      processing.push_front(std::pair<Item, int>(items_blueprint[ingredient.first], (toProduce.second/recipe.second + 1) * ingredient.second));
    }
    //basicBuildOrder.push_front(std::make_tuple<Item, int, Item>(toProduce.first, toProduce.second, recipe.first));
    //basicBuildOrder.push_front(std::make_tuple<Item*, int, Item*>(toProduce_Item, toProduce_int, myRecipe));
    basicBuildOrder.push_front(std::pair<std::pair<Item, int>, Item>(std::pair<Item, int>(toProduce.first, toProduce.second), recipe.first));

    processing.pop_back();
  }
  std::cout << "basic build order\n" << std::endl;
  for (auto i : basicBuildOrder){
    std::cout << i.first.first << std::endl << i.first.second << std::endl << i.second << std::endl;
  }
}


/*
std::deque<std::tupel<Item, int, Item>> Simulator::basicBuildOrder(){
  */

  /*
  std::deque <std::pair<std::string, int>> toProduce;

  for (json::iterator i = initial["goal-items"].begin(); i != initial["goal-items"].end(); ++i){
      toProduce.push_back(std::pair<std::string, int>((*i)["name"], (*i)["amount"]));
  }

  for (std::pair<std::string, int> i : toProduce){
    //std::cout << "name: " << i.first << "amount" << i.second << std::endl;

  }*/
  /*
  std::deque<std::pair<std::string, int>> produced;

  while (!toProduce.empty()){
    json recipe = recipes.find[toProduce.pop_front()];

    for (json:iter)
  }
  */
  /*
  return std::deque<std::string>();
}*/

//void Simulator::printRecipeDoubles(){
    /*for (json::iterator recipe1 = recipes.begin(); recipe1 != recipes.end(); ++recipe1){
    for (json::iterator recipe2 = recipe1 ; recipe2 != recipes.end(); ++recipe2){
      if (recipe1 != recipe2){
        for (json::iterator product1 = (*recipe1)["products"].begin(); product1 !=(*recipe1)["products"].end(); ++product1){
          for (json::iterator product2 = (*recipe2)["products"].begin(); product2 != (*recipe2)["products"].end(); ++product2){
            if ((*product1)["name"] == (*product2)["name"]){
              std::cout << "Product : " << (*product1)["name"] << "\nRecipe1: \n" << recipe1.key() << "\n" << (*recipe1) << "\nRecipe2: \n" <<  recipe2.key() << "\n" << (*recipe2) << "\n" << std::endl;
            }
          }
        }
      }
    }
  }*/
//}
