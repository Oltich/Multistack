# Пример программы
add_executable(SampleProgram main.cpp)

# ИСПРАВЛЕНИЕ: меняем multistack на QueueLibrary
target_link_libraries(SampleProgram QueueLibrary)

# Устанавливаем выходную директорию
set_target_properties(SampleProgram PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)