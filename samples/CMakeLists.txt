# Пример программы
add_executable(SampleProgram main.cpp)

# ДОБАВЛЯЕМ: явно указываем путь к заголовкам
target_include_directories(SampleProgram PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(SampleProgram QueueLibrary)

# Устанавливаем выходную директорию
set_target_properties(SampleProgram PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)