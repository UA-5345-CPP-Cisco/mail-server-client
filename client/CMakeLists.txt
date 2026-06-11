cmake_minimum_required(VERSION 3.20)

project(mail_server_client_client VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

option(MAIL_CLIENT_BUILD_GUI "Build the Qt Quick mail client GUI" ON)

if(MSVC AND VCPKG_TARGET_TRIPLET MATCHES "-static")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_path(BOOST_ASIO_INCLUDE_DIR boost/asio.hpp REQUIRED)

add_library(mail_client_core
    src/MailMessage.cpp
    src/SmtpClient.cpp
)

target_compile_features(mail_client_core PUBLIC cxx_std_20)
target_include_directories(mail_client_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${BOOST_ASIO_INCLUDE_DIR}
)
target_compile_definitions(mail_client_core PUBLIC
    BOOST_ERROR_CODE_HEADER_ONLY
    _WIN32_WINNT=0x0A00
)
target_link_libraries(mail_client_core PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
)

if(WIN32)
    target_link_libraries(mail_client_core PUBLIC ws2_32 mswsock crypt32)
endif()

add_executable(mail_client
    main.cpp
)

target_link_libraries(mail_client PRIVATE mail_client_core)
set_target_properties(mail_client PROPERTIES OUTPUT_NAME mail-client)

if(MSVC)
    target_compile_options(mail_client_core PRIVATE /W4)
    target_compile_options(mail_client PRIVATE /W4)
else()
    target_compile_options(mail_client_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(mail_client PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(MAIL_CLIENT_BUILD_GUI)
    find_package(Qt6 QUIET COMPONENTS Core Quick QuickControls2 QuickLayouts)

    if(Qt6_FOUND)
        qt_standard_project_setup(REQUIRES 6.5)

        qt_add_executable(mail_client_gui
            src/gui/main.cpp
            src/gui/MailClientController.cpp
            include/mail_client/gui/MailClientController.hpp
        )

        file(GLOB GUI_ASSET_FILES CONFIGURE_DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/qml/assets/*
        )

        foreach(asset_file ${GUI_ASSET_FILES})
            get_filename_component(asset_name ${asset_file} NAME)
            set_source_files_properties(${asset_file}
                PROPERTIES QT_RESOURCE_ALIAS "qml/assets/${asset_name}"
            )
        endforeach()

        qt_add_qml_module(mail_client_gui
            URI MailClientGui
            VERSION 1.0
            QML_FILES
                qml/Main.qml
                qml/NavigationQML.qml
                qml/NewMessageQML.qml
                qml/EmailsListQML.qml
                qml/DraftsListQML.qml
                qml/ListItem.qml
                qml/DraftItem.qml
                qml/ContentBlankPageQML.qml
                qml/ContentPageLetterQML.qml
                qml/ContentPageLetterTempQML.qml
                qml/SettingsQML.qml
                qml/SwitchAccountQML.qml
            RESOURCES
                ${GUI_ASSET_FILES}
        )

        target_link_libraries(mail_client_gui PRIVATE
            mail_client_core
            Qt6::Core
            Qt6::Quick
            Qt6::QuickControls2
            Qt6::QuickLayouts
        )

        set_target_properties(mail_client_gui PROPERTIES
            OUTPUT_NAME mail-client-gui
            WIN32_EXECUTABLE TRUE
        )

        if(MSVC)
            target_compile_options(mail_client_gui PRIVATE /W4)
        else()
            target_compile_options(mail_client_gui PRIVATE -Wall -Wextra -Wpedantic)
        endif()

        if(WIN32 AND DEFINED VCPKG_TARGET_TRIPLET)
            set(QT_VCPKG_RUNTIME_ROOT
                ${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}
            )
            configure_file(
                ${CMAKE_CURRENT_SOURCE_DIR}/cmake/qt.conf.in
                ${CMAKE_CURRENT_BINARY_DIR}/qt.conf
                @ONLY
            )
            file(GLOB QT_RUNTIME_DLLS CONFIGURE_DEPENDS
                ${QT_VCPKG_RUNTIME_ROOT}/bin/*.dll
            )

            if(QT_RUNTIME_DLLS)
                add_custom_command(TARGET mail_client_gui POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            ${QT_RUNTIME_DLLS}
                            $<TARGET_FILE_DIR:mail_client_gui>
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            ${CMAKE_CURRENT_BINARY_DIR}/qt.conf
                            $<TARGET_FILE_DIR:mail_client_gui>/qt.conf
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                            ${QT_VCPKG_RUNTIME_ROOT}/Qt6/plugins/platforms
                            $<TARGET_FILE_DIR:mail_client_gui>/platforms
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                            ${QT_VCPKG_RUNTIME_ROOT}/Qt6/qml
                            $<TARGET_FILE_DIR:mail_client_gui>/qml
                    COMMENT "Copying Qt runtime files for mail-client-gui"
                    VERBATIM
                )
            endif()
        endif()
    else()
        message(WARNING "Qt6 was not found, so the mail_client_gui target will not be built.")
    endif()
endif()

if(BUILD_TESTING)
    add_executable(mail_client_core_tests
        tests/ClientCoreTests.cpp
    )

    target_link_libraries(mail_client_core_tests PRIVATE mail_client_core)

    if(MSVC)
        target_compile_options(mail_client_core_tests PRIVATE /W4)
    else()
        target_compile_options(mail_client_core_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    add_test(NAME mail_client_core_tests COMMAND mail_client_core_tests)
endif()
