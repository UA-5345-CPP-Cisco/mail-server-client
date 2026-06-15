add_library(mail_logger
	src/Logger.cpp
	include/logger/Logger.h
)

target_include_directories(mail_logger
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(mail_logger
	PUBLIC
		cxx_std_17
)

target_link_libraries(mail_logger
	PUBLIC
		Threads::Threads
)

if(MSVC)
	target_compile_options(mail_logger PRIVATE /W4)

	if(MAIL_WARNINGS_AS_ERRORS)
		target_compile_options(mail_logger PRIVATE /WX)
	endif()
else()
	target_compile_options(mail_logger PRIVATE -Wall -Wextra -Wpedantic)

	if(MAIL_WARNINGS_AS_ERRORS)
		target_compile_options(mail_logger PRIVATE -Werror)
	endif()
endif()
