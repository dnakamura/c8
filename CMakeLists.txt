
cmake_minimum_required(VERSION 3.2)

enable_testing()

project(c8 VERSION 0.1 LANGUAGES C CXX)

function(c8_find_program var name)
    find_program("${var}" "${name}")
    if(NOT ${var})
        message(SEND_ERROR "Could not find the ${name} executable")
    endif()
endfunction()


c8_find_program(RAGEL_PATH ragel)
c8_find_program(M4_PATH m4)
c8_find_program(SED_PATH sed)


# TODO the generation logic for the lexer requires unix at the moment

add_subdirectory(lingo)
add_subdirectory(googletest)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ragel_fragment.rl"
    COMMAND "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/translate.sh"
    DEPENDS translate.sh transform.m4 src/TokenKinds.def
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Lexer.rl"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/ragel_fragment.rl" src/Lexer.rl.in
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND "${M4_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/src/Lexer.rl.in" "-I." > "Lexer.rl"
)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Lexer_generated.cpp"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Lexer.rl"
    COMMAND "${RAGEL_PATH}" Lexer.rl -o Lexer_generated.cpp
)

# C8 Library

add_library(c8
    src/Lexer.cpp
    src/Parser.cpp
    src/Tokens.cpp
    src/Debug.cpp
    src/DebugPrinter.cpp
    Lexer_generated.cpp
)

target_include_directories(c8 PUBLIC src)

target_link_libraries(c8
    PUBLIC lingo
)

set_property(TARGET c8 PROPERTY CXX_STANDARD 14)


# C8 Executable

add_executable(c8_exe
    src/main.cpp)

set_target_properties(c8_exe PROPERTIES OUTPUT_NAME "c8")

target_link_libraries(c8_exe c8)

target_include_directories(c8_exe PUBLIC src)

set_property(TARGET c8_exe PROPERTY CXX_STANDARD 14)


# C8 Testing

add_executable(c8test
    test/main.cpp)

target_link_libraries(c8test
    c8
    gtest_main)

set_property(TARGET c8test PROPERTY CXX_STANDARD 14)

add_test(NAME c8test COMMAND c8test)
