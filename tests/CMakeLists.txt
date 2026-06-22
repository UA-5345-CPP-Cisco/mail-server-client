cmake_policy(SET CMP0135 NEW)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(run_logger_test logger_test.cpp)
target_link_libraries(run_logger_test PRIVATE core_server_lib GTest::gtest_main GTest::gmock)

add_executable(run_thread_pool_test thread_pool_test.cpp)
target_link_libraries(run_thread_pool_test PRIVATE core_server_lib GTest::gtest_main GTest::gmock)

add_executable(run_server_test server_test.cpp)
target_link_libraries(run_server_test PRIVATE core_server_lib GTest::gtest_main GTest::gmock)

include(GoogleTest)
gtest_discover_tests(run_logger_test)
gtest_discover_tests(run_thread_pool_test)
gtest_discover_tests(run_server_test)
