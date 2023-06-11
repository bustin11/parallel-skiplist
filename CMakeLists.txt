cmake_minimum_required(VERSION 3.20.5)
project(main LANGUAGES C CXX)

add_executable(${PROJECT_NAME} main.cpp problems.cpp) # top level
set_property(TARGET main PROPERTY CXX_STANDARD 14)

add_subdirectory("src")
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>) 
target_link_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>) # in build, we need to link the libraries and the name of that folder
target_link_libraries(${PROJECT_NAME} SkipList) # SkipList := name given in the add_library of the CMakeLists.txt in src/

add_subdirectory("helpers")
target_include_directories(${PROJECT_NAME} PUBLIC helpers)

# MESSAGE(STATUS "All objects files: " ${ALL_OBJECT_FILES})
# target_link_directories(${PROJECT_NAME} PUBLIC helpers)
# target_link_libraries(${PROJECT_NAME} Helpers)