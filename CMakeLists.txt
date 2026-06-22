cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(SmtpServer
        VERSION 0.1.0
        DESCRIPTION "SMTP server using Boost.Asio, Crypto++, SQLite"
        LANGUAGES CXX
)

option(SMTP_BUILD_TESTS "Build tests" OFF)
option(SMTP_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

# BOOST
include(FetchContent)

set(BOOST_INCLUDE_LIBRARIES asio system)
set(BOOST_ENABLE_MPI OFF)
set(BOOST_ENABLE_PYTHON OFF)
set(BUILD_TESTING OFF)

FetchContent_Declare(
        Boost
        URL https://github.com/boostorg/boost/releases/download/boost-1.88.0/boost-1.88.0-cmake.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP ON
)

FetchContent_MakeAvailable(Boost)

# OTHER DEPENDENCIES
find_package(Threads REQUIRED)
find_package(SQLite3 REQUIRED)

# CRYPTO++

FetchContent_GetProperties(cryptopp_cmake)
if(NOT cryptopp_cmake_POPULATED)
    FetchContent_Declare(
            cryptopp_cmake
            GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake/
            GIT_TAG        CRYPTOPP_8_9_0
    )

    FetchContent_MakeAvailable(cryptopp_cmake)
endif()

add_executable(smtp_server
        #main.cpp
        include/MailBuilder.hpp
        src/MailBuilder.cpp
        include/SmtpClient.hpp
        src/SmtpClient.cpp
        tests/Tests.cpp
)

target_include_directories(smtp_server
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(smtp_server
        PRIVATE
        Boost::asio
        Boost::system
        SQLite::SQLite3
        Threads::Threads
        cryptopp::cryptopp
)

target_compile_definitions(smtp_server
        PRIVATE
        BOOST_ASIO_NO_DEPRECATED
)

#if(MSVC)
#    target_compile_options(smtp_server PRIVATE /W4)
#
#    if(SMTP_WARNINGS_AS_ERRORS)
#        target_compile_options(smtp_server PRIVATE /WX)
#    endif()
#else()
#    target_compile_options(smtp_server PRIVATE
#        -Wall
#        -Wextra
#        -Wpedantic
#        -Wshadow
#        -Wconversion
#        -Wnon-virtual-dtor
#    )
#
#    if(SMTP_WARNINGS_AS_ERRORS)
#        target_compile_options(smtp_server PRIVATE -Werror)
#    endif()
#endif()
#MY VLAD TEST
add_executable(my_tests
        tests/Tests.cpp
        src/MailBuilder.cpp
        src/SmtpClient.cpp
)
##
target_include_directories(my_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(my_tests PRIVATE Boost::asio Boost::system Threads::Threads ws2_32 mswsock)

if(BUILD_TESTS)
    enable_testing()

    include(FetchContent)

    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )

    # Required for Windows runtime compatibility
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    add_executable(smtp_server_tests
            tests/test_basic.cpp
            tests/Tests.cpp
    )

    target_include_directories(smtp_server_tests
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(smtp_server_tests
            PRIVATE
            GTest::gtest
            GTest::gtest_main
            GTest::gmock
            GTest::gmock_main
            Boost::asio
            Boost::system
            SQLite::SQLite3
            Threads::Threads
            cryptopp::cryptopp
    )

    include(GoogleTest)
    gtest_discover_tests(smtp_server_tests)
endif()