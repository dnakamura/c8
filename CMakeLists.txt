
cmake_minimum_required(VERSION 3.2)

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


# TODO the generation logic for the lexer requires  unix at the moment

add_subdirectory(lingo)

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


add_executable(c8
    src/main.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/Tokens.cpp
    src/Debug.cpp
    src/DebugPrinter.cpp
    Lexer_generated.cpp
)

target_include_directories(c8 PRIVATE src)

target_link_libraries(c8
    PRIVATE
        lingo
)

set_property(TARGET c8 PROPERTY CXX_STANDARD 14)
