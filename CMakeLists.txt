cmake_minimum_required(VERSION 3.20)
project(QueueLibrary VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Политики CMake
cmake_policy(SET CMP0077 NEW)

# ДОБАВЛЯЕМ: глобальный путь к заголовкам
include_directories(${CMAKE_SOURCE_DIR}/include)

# Опции проекта
option(BUILD_TESTING "Build tests" ON)
option(BUILD_SAMPLES "Build samples" ON)

# Поддиректории
add_subdirectory(include)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()

if(BUILD_SAMPLES)
    add_subdirectory(samples)
endif()