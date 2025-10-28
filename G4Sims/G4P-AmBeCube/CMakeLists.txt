cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(G4P-AmBeCube)

find_package(Geant4 REQUIRED ui_all vis_all)

include_directories(include)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

file(GLOB MACRO_FILES 
    "${PROJECT_SOURCE_DIR}/macros/*.mac"
)

file(COPY ${PROJECT_SOURCE_DIR}/macros DESTINATION ${PROJECT_BINARY_DIR})

add_executable(AmBeCube-EXE main.cc ${sources})
target_link_libraries(AmBeCube-EXE ${Geant4_LIBRARIES})
add_custom_target(G4P-AmBeCube DEPENDS AmBeCube-EXE)
