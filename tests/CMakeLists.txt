

set(This mygtests)

add_executable(${This} test.cpp test.h)

target_include_directories(${PROJECT_NAME} 
  PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include>
) 

message(${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include)

target_link_directories(${PROJECT_NAME} 
  PRIVATE ${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include
)

target_link_libraries(${This} PUBLIC 
  SkipList
  GTest::gtest_main
)

add_test(
  NAME ${This}
  COMMAND ${This}
  )
include (GoogleTest)
gtest_discover_tests(${This})
