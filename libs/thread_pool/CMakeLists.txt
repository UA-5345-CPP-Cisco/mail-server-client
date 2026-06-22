add_library(mail_thread_pool
	src/ThreadPool.cpp
	include/thread_pool/ThreadPool.h
)

target_include_directories(mail_thread_pool
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(mail_thread_pool
	PUBLIC
		cxx_std_17
)

target_link_libraries(mail_thread_pool
	PUBLIC
		Threads::Threads
)

if(MSVC)
	target_compile_options(mail_thread_pool PRIVATE /W4)

	if(MAIL_WARNINGS_AS_ERRORS)
		target_compile_options(mail_thread_pool PRIVATE /WX)
	endif()
else()
	target_compile_options(mail_thread_pool PRIVATE -Wall -Wextra -Wpedantic)

	if(MAIL_WARNINGS_AS_ERRORS)
		target_compile_options(mail_thread_pool PRIVATE -Werror)
	endif()
endif()
