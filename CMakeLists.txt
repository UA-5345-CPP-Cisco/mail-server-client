cmake_minimum_required(VERSION 3.20)

project(MailServerClient
		VERSION 0.1.0
		DESCRIPTION "Mail server and client implementation"
		LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MAIL_BUILD_TESTS "Build tests" OFF)
option(MAIL_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

include(FetchContent)

set(BOOST_INCLUDE_LIBRARIES asio property_tree system)
set(BOOST_ENABLE_MPI OFF)
set(BOOST_ENABLE_PYTHON OFF)
set(BUILD_TESTING OFF)

FetchContent_Declare(
		Boost
		URL https://github.com/boostorg/boost/releases/download/boost-1.88.0/boost-1.88.0-cmake.tar.xz
		DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(Boost)

FetchContent_Declare(
		cryptopp_cmake
		GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake/
		GIT_TAG CRYPTOPP_8_9_0
)
set(CRYPTOPP_BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cryptopp_cmake)

# Declare the community CMake wrapper for OpenSSL
FetchContent_Declare(
		openssl
		GIT_REPOSITORY https://github.com/janbar/openssl-cmake.git
		GIT_TAG        "1.1.1k-20210430"
		PATCH_COMMAND  ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/patch_openssl.cmake
)
FetchContent_MakeAvailable(openssl)

# Create standard aliases matching the official FindOpenSSL module
if(NOT TARGET OpenSSL::SSL)
	add_library(OpenSSL::SSL ALIAS ssl)
endif()

if(NOT TARGET OpenSSL::Crypto)
	add_library(OpenSSL::Crypto ALIAS crypto)
endif()

find_package(Threads REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Quick Sql QuickControls2)

add_library(sqlite3 STATIC
		libs/storage/sqlite3/sqlite3.c
)

target_include_directories(sqlite3 PUBLIC
		libs/storage/sqlite3
)

add_subdirectory(libs/logger)
add_subdirectory(libs/storage)
add_subdirectory(libs/thread_pool)
add_subdirectory(libs/smtp_client)
add_subdirectory(apps/smtp_server)
add_subdirectory(apps/simple-client)
add_subdirectory(apps/qt-mail-client)

if(MAIL_BUILD_TESTS)
	enable_testing()
	FetchContent_Declare(
			googletest
			URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
			DOWNLOAD_EXTRACT_TIMESTAMP ON
	)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)
	add_subdirectory(tests)
endif()