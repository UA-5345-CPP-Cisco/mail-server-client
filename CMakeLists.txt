cmake_minimum_required(VERSION 3.14) 
project(Mail_Server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/logger)
add_subdirectory(libs/thread_pool)
add_subdirectory(libs/storage)
add_subdirectory(libs/smtp_client)

add_library(json_lib INTERFACE)
target_include_directories(json_lib INTERFACE libs/json)

add_library(mail_backend_lib 
    apps/IMAP-SERVER/Server.cpp
    apps/IMAP-SERVER/Client.cpp
)
target_link_libraries(mail_backend_lib PRIVATE mail_logger mail_thread_pool json_lib)
add_executable(mail_backend apps/IMAP-SERVER/main.cpp)
target_link_libraries(mail_backend PRIVATE mail_backend_lib)


add_subdirectory(apps/smtp_server)


enable_testing()
add_subdirectory(tests)