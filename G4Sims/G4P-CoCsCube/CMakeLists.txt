cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(CoCsCube)

find_package(Geant4 REQUIRED ui_all vis_all)

include_directories(include)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

## Copy macros directory from source into the build directory so macros/ is
## available next to the executable in the build tree.
file(COPY ${PROJECT_SOURCE_DIR}/macros DESTINATION ${PROJECT_BINARY_DIR})

add_executable(CoCsCubeEXE main.cc ${sources})
target_link_libraries(CoCsCubeEXE ${Geant4_LIBRARIES})
add_custom_target(CoCsCube DEPENDS CoCsCubeEXE)
