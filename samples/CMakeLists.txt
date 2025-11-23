# Пример программы
add_executable(SampleProgram main.cpp)
target_link_libraries(SampleProgram PRIVATE QueueLibrary)

# Устанавливаем выходную директорию
set_target_properties(SampleProgram PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)