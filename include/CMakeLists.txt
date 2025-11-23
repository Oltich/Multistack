# Интерфейсная библиотека для заголовочных файлов
add_library(QueueLibrary INTERFACE)

# ИСПРАВЛЕНИЕ: указываем правильную директорию с заголовками
target_include_directories(QueueLibrary INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# ДОБАВЛЯЕМ: псевдоним для обратной совместимости
add_library(multistack ALIAS QueueLibrary)

# Свойства компиляции
target_compile_features(QueueLibrary INTERFACE cxx_std_17)

# Настройки компилятора
if(MSVC)
    target_compile_options(QueueLibrary INTERFACE /W4)
else()
    target_compile_options(QueueLibrary INTERFACE -Wall -Wextra -pedantic)
endif()