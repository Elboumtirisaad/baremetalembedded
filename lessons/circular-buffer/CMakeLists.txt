cmake_minimum_required(VERSION 3.22)
project(critical_circular_buffer LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(CB_ENABLE_SANITIZERS "Enable host sanitizers" OFF)

function(strict_target target_name)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target_name} PRIVATE
            -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror)
        if(CB_ENABLE_SANITIZERS)
            target_compile_options(${target_name} PRIVATE
                -fsanitize=address,undefined -fno-omit-frame-pointer)
            target_link_options(${target_name} PRIVATE -fsanitize=address,undefined)
        endif()
    endif()
endfunction()

add_library(circular_buffer STATIC src/circular_buffer.c)
target_include_directories(circular_buffer PUBLIC include)
strict_target(circular_buffer)

include(CTest)
if(BUILD_TESTING)
    add_executable(circular_buffer_tests tests/test_circular_buffer.c)
    target_link_libraries(circular_buffer_tests PRIVATE circular_buffer)
    strict_target(circular_buffer_tests)
    add_test(NAME circular_buffer_tests COMMAND circular_buffer_tests)
endif()
