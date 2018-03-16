cmake_minimum_required(VERSION 3.8)
project(TPGame)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES additionals.cpp main.cpp resources.cpp resourceList.cpp player.cpp playerCalc.cpp unit.cpp action.cpp game.cpp)
set(AVAILABLES availableUnits.cpp availableActions.cpp)
add_executable(TPGame ${SOURCES} ${AVAILABLES})
