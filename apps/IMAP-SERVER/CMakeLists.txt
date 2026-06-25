add_library(imap_back_lib
    src/Client.cpp
    src/Server.cpp
)

target_include_directories(imap_back_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(imap_back_lib
PUBLIC
    mail_logger
    mail_thread_pool
PRIVATE
    Boost::json 
    Threads::Threads
)

add_executable(mail_back_imap src/main.cpp)
target_link_libraries(mail_back_imap PRIVATE imap_back_lib)



