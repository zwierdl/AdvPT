include_directories(
  ${PROJECT_SOURCE_DIR}/factorio/hpp_files)

add_library(itemlib item.cpp)
add_library(recipelib recipe.cpp)
add_library(technologylib technology.cpp)
add_library(factorylib factory.cpp)
add_library(crafting_categorylib crafting_category.cpp)
add_library(simulatorlib simulator.cpp)
