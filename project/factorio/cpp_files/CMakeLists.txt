include_directories(
  ${PROJECT_SOURCE_DIR}/factorio/hpp_files)

add_library(itemlib item.cpp)
add_library(recipelib recipe.cpp)
add_library(technologylib technology.cpp)
add_library(factorylib factory.cpp)
add_library(crafting_categorylib crafting_category.cpp)
add_library(hashlib hash.cpp)
add_library(simulatorlib simulator.cpp)
add_library(orderlib order.cpp)

target_compile_options(simulatorlib PRIVATE -std=c++2a)
target_compile_options(itemlib PRIVATE -g -std=c++2a)
target_compile_options(recipelib PRIVATE -g -std=c++2a)
target_compile_options(technologylib PRIVATE -g -std=c++2a)
target_compile_options(crafting_categorylib PRIVATE -g -std=c++2a)
target_compile_options(hashlib PRIVATE -g -std=c++2a)
target_compile_options(orderlib PRIVATE -g -std=c++2a)
