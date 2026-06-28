add_executable(aimap_server
        main.cpp
)

target_link_libraries(aimap_server
        PRIVATE
        mail_aimap
        mail_logger
)

target_compile_features(aimap_server
        PRIVATE
        cxx_std_17
)

if(MSVC)
    target_compile_options(aimap_server PRIVATE /W4)
else()
    target_compile_options(aimap_server PRIVATE -Wall -Wextra -Wpedantic)
endif()