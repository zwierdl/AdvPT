cmake_minimum_required(VERSION 3.16)

project(FactorioOptimizer VERSION 0.1
                          DESCRIPTION "Factorio optimizer for AdvPT"
                          LANGUAGES CXX)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_subdirectory(factorio)
