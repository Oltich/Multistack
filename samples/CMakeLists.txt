# Пример программы
add_executable(SampleProgram main.cpp)

# Явно указываем абсолютный путь к заголовкам
target_include_directories(SampleProgram PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

# Для отладки - выводим путь
message(STATUS "Include path: ${CMAKE_SOURCE_DIR}/include")

target_link_libraries(SampleProgram QueueLibrary)

# Устанавливаем выходную директорию
set_target_properties(SampleProgram PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)