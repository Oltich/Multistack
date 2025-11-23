cmake_minimum_required(VERSION 3.16)
project(MultiStack VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTING "Build tests" ON)

# Создаем интерфейсную библиотеку для multistack
add_library(multistack INTERFACE)
target_include_directories(multistack INTERFACE include)

# Основное приложение
add_executable(multistack_demo samples/main.cpp)
target_link_libraries(multistack_demo multistack)

# Тесты
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    add_executable(multistack_tests test/test_multistack.cpp)
    target_link_libraries(multistack_tests 
        gtest_main 
        multistack
    )
    
    include(GoogleTest)
    gtest_discover_tests(multistack_tests)
endif()

# Компиляторные флаги
if(MSVC)
    target_compile_options(multistack_demo PRIVATE /W4)
    if(BUILD_TESTING)
        target_compile_options(multistack_tests PRIVATE /W4)
    endif()
else()
    target_compile_options(multistack_demo PRIVATE -Wall -Wextra)
    if(BUILD_TESTING)
        target_compile_options(multistack_tests PRIVATE -Wall -Wextra)
    endif()
endif()

message(STATUS "MultiStack project configured")