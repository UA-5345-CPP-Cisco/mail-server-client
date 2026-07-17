cmake_minimum_required(VERSION 3.20)

project(MailServerClient
		VERSION 0.1.0
		DESCRIPTION "Mail server and client implementation"
		LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MAIL_BUILD_TESTS "Build tests" ON)
option(MAIL_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(MAIL_BUILD_QT_CLIENT "Build Qt mail client" OFF)
option(MAIL_BUILD_QT_CLIENT_TESTS "Build Qt mail client tests" OFF)

include(FetchContent)

# ──────────────────────────────────────────
# External dependencies (ALL via FetchContent)
# ──────────────────────────────────────────

# --- Threads (needed by several deps below) ---
find_package(Threads REQUIRED)

# --- Boost ---
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

# --- Crypto++ ---
FetchContent_Declare(
		cryptopp_cmake
		GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake/
		GIT_TAG CRYPTOPP_8_9_0
)
set(CRYPTOPP_BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cryptopp_cmake)

find_package(Threads REQUIRED)

# --- nlohmann JSON ---
FetchContent_Declare(
		nlohmann_json
		URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
		DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(nlohmann_json)

# --- OpenSSL (CMake wrapper) ---
set(WITH_APPS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
		openssl
		GIT_REPOSITORY https://github.com/janbar/openssl-cmake.git
		GIT_TAG 1.1.1w-20250419
)
FetchContent_MakeAvailable(openssl)

# Aliases to match find_package(OpenSSL) target names
if(NOT TARGET OpenSSL::SSL)
	add_library(OpenSSL::SSL ALIAS ssl)
endif()
if(NOT TARGET OpenSSL::Crypto)
	add_library(OpenSSL::Crypto ALIAS crypto)
endif()

# ──────────────────────────────────────────
# Internal libraries
# ──────────────────────────────────────────
add_subdirectory(libs/logger)
add_subdirectory(libs/mail-storage)
add_subdirectory(libs/thread_pool)
add_subdirectory(libs/smtp-server-sdk)
add_subdirectory(libs/mail-server-sdk)

# ──────────────────────────────────────────
# Applications
# ──────────────────────────────────────────
add_subdirectory(apps/smtp-server)
add_subdirectory(apps/simple-client)
add_subdirectory(apps/mail-server)

if(MAIL_BUILD_QT_CLIENT)
	add_subdirectory(apps/mail-client)
endif()

# ──────────────────────────────────────────
# Tests
# ──────────────────────────────────────────
if(MAIL_BUILD_TESTS)
	enable_testing()
	FetchContent_Declare(
			googletest
			URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
			DOWNLOAD_EXTRACT_TIMESTAMP ON
	)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)
	add_subdirectory(tests)
	if(MAIL_BUILD_QT_CLIENT_TESTS)
		add_subdirectory(tests-client)
	endif()
endif()
