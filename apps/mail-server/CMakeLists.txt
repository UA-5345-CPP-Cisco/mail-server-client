add_executable(mail_server
        main.cpp
)

target_link_libraries(mail_server
        PUBLIC
        mail_logger
        PRIVATE
        mail_server_sdk

)

target_compile_features(mail_server
        PRIVATE
        cxx_std_17
)

if(MSVC)
    target_compile_options(mail_server PRIVATE /W4)
else()
    target_compile_options(mail_server PRIVATE -Wall -Wextra -Wpedantic)
endif()