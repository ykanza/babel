cmake_minimum_required(VERSION 3.20)
project(babel)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-g3")
include(./build/conanbuildinfo.cmake)

conan_basic_setup()

if(APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/lib")
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")
endif()
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5OpenGL CONFIG REQUIRED)


add_executable(babel_client client/src/main.cpp
                            client/src/network.cpp
                            client/include/client.hpp
                            client/include/network.hpp
        client/src/Codec/Opus.cpp
        client/src/Codec/Opus.hpp
                            ${QT_GUI_H}
                            ${MOC_SRCS}
#        GUI/main.cpp
        GUI/mainwindow.cpp
        GUI/mainwindow.hpp
        GUI/secdialog.cpp
        GUI/secdialog.hpp
        client/src/Audio/PA.cpp
        client/src/Audio/PA.hpp
        client/src/Audio/IAudio.hpp
        client/src/Codec/ICodec.hpp client/src/Codec/EncodedPacket.hpp client/src/Codec/Opus.hpp
        Utils/Utils.hpp Exception/Error.cpp Exception/Error.hpp Config/Config.hpp)
add_executable(babel_server server/src/main.cpp
                            server/src/database.cpp
                            server/src/network.cpp
        Exception/Error.cpp Exception/Error.hpp server/src/Exchange/Exchange.hpp
                            ${MOC_SRCS} server/include/Client.hpp)


conan_target_link_libraries(babel_client    CONAN_PKG::portaudio
                                            CONAN_PKG::opus
                                            CONAN_PKG::qt
                                            Qt5::Widgets
                                            Qt5::Network
                                            Qt5::Core
                                            Qt5::Gui
                                            Qt5::OpenGL)
target_include_directories(babel_client PUBLIC ${CONAN_INCLUDE_DIRS})

conan_target_link_libraries(babel_server    CONAN_PKG::portaudio
                                            CONAN_PKG::opus
                                            CONAN_PKG::qt)
target_include_directories(babel_server PUBLIC ${CONAN_INCLUDE_DIRS})