add_executable(smtp_server
    main.cpp
)

target_link_libraries(smtp_server
    PRIVATE
        mail_storage
        Boost::asio
        Boost::system
        Threads::Threads
        cryptopp::cryptopp
)

target_compile_definitions(smtp_server
    PRIVATE
        BOOST_ASIO_NO_DEPRECATED
)

