cmake_minimum_required(VERSION 3.16)
project(MultiStack VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTING "Build tests" ON)

# Основное приложение
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/samples/main.cpp)
    add_executable(multistack_demo samples/main.cpp)
elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    add_executable(multistack_demo src/main.cpp)
else()
    message(WARNING "main.cpp not found, creating minimal demo")
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/minimal_demo.cpp
        "#include \"include/multistack.h\"\n#include <iostream>\n\nint main() {\n    MultiStack<int> ms(10, 2);\n    ms.Push(0, 42);\n    std::cout << \\\"Demo completed!\\\" << std::endl;\n    return 0;\n}\n"
    )
    add_executable(multistack_demo minimal_demo.cpp)
endif()

target_include_directories(multistack_demo PRIVATE include)

# Тесты с исправленной версией GTest
if(BUILD_TESTING)
    # Используем более новую версию GTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0  # Более новая версия
    )
    
    # Для Windows добавляем флаги для подавления предупреждений
    if(MSVC)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        add_compile_definitions(_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
        add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    endif()
    
    FetchContent_MakeAvailable(googletest)
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/test_multistack.cpp)
        add_executable(multistack_tests test/test_multistack.cpp)
        target_include_directories(multistack_tests PRIVATE include)
        target_link_libraries(multistack_tests PRIVATE gtest_main)
        
        # Отключаем определенные предупреждения для тестов
        if(MSVC)
            target_compile_options(multistack_tests PRIVATE /W4 /wd4100 /wd4201 /wd4505)
        else()
            target_compile_options(multistack_tests PRIVATE -Wall -Wextra -Wno-unused-parameter)
        endif()
        
        include(GoogleTest)
        gtest_discover_tests(multistack_tests)
        
        message(STATUS "GTest tests enabled with version 1.14.0")
    else()
        message(WARNING "test_multistack.cpp not found, GTest tests disabled")
    endif()
endif()

# Компиляторные флаги для основного приложения
if(MSVC)
    target_compile_options(multistack_demo PRIVATE /W4)
    target_compile_definitions(multistack_demo PRIVATE _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
else()
    target_compile_options(multistack_demo PRIVATE -Wall -Wextra)
endif()

message(STATUS "MultiStack project configured")