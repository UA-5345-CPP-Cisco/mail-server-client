cmake_minimum_required(VERSION 3.14) 
project(IMAP_Server)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CORE_SOURCES
    src/Server.cpp
    src/Client.cpp
    src/Thread_pool.cpp
    src/Logger.cpp
)
add_library(core_server_lib STATIC ${CORE_SOURCES})

target_include_directories(core_server_lib PUBLIC src)

add_executable(mail_backend src/main.cpp)
target_link_libraries(mail_backend PRIVATE core_server_lib)

enable_testing()
add_subdirectory(tests)
