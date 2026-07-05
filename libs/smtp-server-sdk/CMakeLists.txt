add_library(smtp_server_sdk
        src/MailBuilder.cpp
        src/SmtpClient.cpp
        include/smtp_server_sdk/MailBuilder.h
        include/smtp_server_sdk/SmtpClient.h
)

target_include_directories(smtp_server_sdk
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(smtp_server_sdk
        PUBLIC
        Boost::asio
        Boost::system
        Threads::Threads
)

target_compile_features(smtp_server_sdk
        PUBLIC
        cxx_std_17
)
