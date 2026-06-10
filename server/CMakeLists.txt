cmake_minimum_required(VERSION 3.20)

project(mail_server_client_server VERSION 0.1.0 LANGUAGES CXX)

add_library(smtp_server
    src/smtp/BoostSocketsManager.cpp
    src/smtp/Logger.cpp
    src/smtp/Server.cpp
    src/smtp/Services.cpp
    src/smtp/Session.cpp
    src/smtp/SmtpSessionHandler.cpp
    src/smtp/ThreadPool.cpp
)

target_compile_features(smtp_server PUBLIC cxx_std_20)
target_include_directories(smtp_server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(smtp_server_app
    src/main.cpp
)

target_link_libraries(smtp_server_app PRIVATE smtp_server)
set_target_properties(smtp_server_app PROPERTIES OUTPUT_NAME smtp-server)

if(MSVC)
    target_compile_options(smtp_server PRIVATE /W4)
    target_compile_options(smtp_server_app PRIVATE /W4)
else()
    target_compile_options(smtp_server PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(smtp_server_app PRIVATE -Wall -Wextra -Wpedantic)
endif()
