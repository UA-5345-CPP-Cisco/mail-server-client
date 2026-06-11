cmake_minimum_required(VERSION 3.20)

project(mail_server_client_server VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

if(MSVC AND VCPKG_TARGET_TRIPLET MATCHES "-static")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

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
    ${Boost_INCLUDE_DIRS}
)
target_compile_definitions(smtp_server PUBLIC
    BOOST_ERROR_CODE_HEADER_ONLY
    _WIN32_WINNT=0x0A00
)
target_link_libraries(smtp_server PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
    Boost::boost
)

if(WIN32)
    target_link_libraries(smtp_server PUBLIC ws2_32 mswsock crypt32)
endif()

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

if(BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)
    include(GoogleTest)

    add_executable(smtp_server_tests
        tests/ServerTests.cpp
        tests/ServicesTests.cpp
        tests/SessionTests.cpp
        tests/SmtpSessionHandlerTests.cpp
        tests/ThreadPoolTests.cpp
    )

    target_link_libraries(smtp_server_tests PRIVATE
        smtp_server
        GTest::gtest_main
    )

    if(MSVC)
        target_compile_options(smtp_server_tests PRIVATE /W4)
    else()
        target_compile_options(smtp_server_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    gtest_discover_tests(smtp_server_tests)
endif()
