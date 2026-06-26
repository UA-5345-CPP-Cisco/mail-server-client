add_executable(simple_smtp_client
        main.cpp
)

target_link_libraries(simple_smtp_client
        PRIVATE
        smtp_client
)

target_compile_features(simple_smtp_client
        PRIVATE
        cxx_std_17
)

if (MSVC)
    target_compile_options(simple_smtp_client PRIVATE /W4)

    if (MAIL_WARNINGS_AS_ERRORS)
        target_compile_options(simple_smtp_client PRIVATE /WX)
    endif ()
else ()
    target_compile_options(
            simple_smtp_client
            PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )

    if (MAIL_WARNINGS_AS_ERRORS)
        target_compile_options(simple_smtp_client PRIVATE -Werror)
    endif ()
endif ()
