add_executable(c8lexer_test main.cpp)

target_link_libraries(c8lexer_test
    c8
    gtest_main)

set_property(TARGET c8lexer_test PROPERTY CXX_STANDARD 14)

if(CMAKE_VERSION VERSION_LESS "3.10")
  add_test(
    NAME lexer_test
    COMMAND c8lexer_test
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  )
else()
  include(GoogleTest)
  gtest_discover_tests(c8lexer_test WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

