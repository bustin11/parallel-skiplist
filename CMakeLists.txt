cmake_minimum_required(VERSION 3.20.5)
project(main LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} main.cpp problems.cpp) # top level
set_property(TARGET main PROPERTY CXX_STANDARD 14)

add_subdirectory(src)
add_subdirectory(helpers)

target_include_directories(${PROJECT_NAME} 
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src> # in build, we need to link the libraries and the name of that folder
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/helpers>
) 

# specifies where to look for the libraries, called libxxx.a, after "make"ing the files (run ./build.sh)
target_link_directories(${PROJECT_NAME} 
  PRIVATE src
)

target_link_libraries(${PROJECT_NAME} 
  SkipList
) # SkipList := name given in the add_library of the CMakeLists.txt in src/


# ================ TESTS ==================

# adding unit tests 
enable_testing()
add_subdirectory(third_party/googletest)
add_subdirectory(tests) # calls CMakeLists.txt

# target_include_directories(${PROJECT_NAME} 
#   PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest/include>
# ) 
# target_link_directories(${PROJECT_NAME} 
#   PRIVATE third_party/googletest/googletest
# )

# this will be a library for google test so that google test can run it 
# SkipList library already added in the src/ dir, no need to add in again
# add_library(${P1ROJECT_NAME} STATIC src/skiplist.cpp src/skiplist.h)