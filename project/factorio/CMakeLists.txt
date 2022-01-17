include_directories(
	${PROJECT_SOURCE_DIR}/factorio/hpp_files
	)

add_subdirectory(cpp_files)

add_executable(factorio factorio.cpp cpp_files/simulator.cpp cpp_files/technology.cpp cpp_files/recipe.cpp)

target_compile_options(factorio PRIVATE -g -std=c++2a)

target_link_libraries(factorio technologylib)
target_link_libraries(factorio recipelib)
target_link_libraries(factorio itemlib)
target_link_libraries(factorio simulatorlib)
target_link_libraries(factorio factorylib)
target_link_libraries(factorio crafting_categorylib)
target_link_libraries(factorio hashlib)
target_link_libraries(factorio orderlib)
target_link_libraries(factorio eventslib)

target_link_libraries(factorio nlohmann_json::nlohmann_json)
