include(GoogleTest)

set(CMAKE_AUTOMOC ON)

option(CACHE_TESTS "Build cache tests" OFF)

find_package(Qt6 COMPONENTS Core Gui Sql REQUIRED)

if(NOT TARGET qtappmail_core)
	add_subdirectory(
			${CMAKE_CURRENT_SOURCE_DIR}/../apps/mail-client
			${CMAKE_CURRENT_BINARY_DIR}/apps/mail-client
	)
endif()

add_executable(state_tests
		state_tests/CurrentUserTests.cpp
		state_tests/AccountListModelTests.cpp
		state_tests/EmailFilterProxyTests.cpp
		state_tests/EmailListModelTests.cpp
		state_tests/main.cpp
)

target_link_libraries(state_tests
		PRIVATE
		gtest
		qtappmail_core
)

target_compile_features(state_tests
		PRIVATE
		cxx_std_17
)

gtest_add_tests(TARGET state_tests SOURCES
		state_tests/CurrentUserTests.cpp
		state_tests/AccountListModelTests.cpp
		state_tests/EmailFilterProxyTests.cpp
		state_tests/EmailListModelTests.cpp
)

if(CACHE_TESTS)
	add_executable(state_tests
			state_tests/EmailCacheTests.cpp
			state_tests/UserCacheTests.cpp
	)

	gtest_add_tests(TARGET state_tests SOURCES
			state_tests/EmailCacheTests.cpp
			state_tests/UserCacheTests.cpp
	)
endif()

add_executable(model_tests
		model_tests/DatabaseManagerTests.cpp
		model_tests/MessageComposerTests.cpp
		model_tests/MessageSearchModelTests.cpp
		model_tests/AuthHandlerTests.cpp
		state_tests/main.cpp
)

target_link_libraries(model_tests
		PRIVATE
		gtest
		qtappmail_core
)

target_compile_features(model_tests
		PRIVATE
		cxx_std_17
)

gtest_add_tests(TARGET model_tests SOURCES
		model_tests/DatabaseManagerTests.cpp
		model_tests/MessageComposerTests.cpp
		model_tests/MessageSearchModelTests.cpp
		model_tests/AuthHandlerTests.cpp
)
