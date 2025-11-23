# Загружаем GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Функция для создания тестов
function(add_queue_test test_name source_file)
    add_executable(${test_name} ${source_file})
    target_link_libraries(${test_name} PRIVATE QueueLibrary gtest_main)
    
    # Настройки компилятора для тестов
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${test_name} PRIVATE -Wno-deprecated-copy)
    endif()
    
    # Регистрируем в CTest
    include(GoogleTest)
    gtest_discover_tests(${test_name})
endfunction()

# Создаем тесты
add_queue_test(ListQueueTests test_multistack.cpp)

# Тесты с ручным main (если нужно)
add_executable(AllQueueTestsManual 
    test_multistack.cpp 
)
target_link_libraries(AllQueueTestsManual PRIVATE QueueLibrary gtest)
gtest_discover_tests(AllQueueTestsManual)