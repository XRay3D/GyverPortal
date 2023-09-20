cmake_minimum_required(VERSION 3.5)

project(Test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Widgets
    Network
    WebEngineCore
    WebEngineWidgets
    Xml
)

add_definitions(
    -DPROGMEM=/**/
    "-DPGM_P=const char*"
    "-DFPSTR=(const char*)"
    -DPSTR=FPSTR
    -DGP_NO_MDNS
    -DGP_NO_DNS
    -DGP_NO_OTA
    -Dstrlen_P=strlen
)

include_directories(
    ../src
    QtWebApp-1.8.6/QtWebApp/httpserver
    QtWebApp-1.8.6/QtWebApp
    ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB QtWebApp
    QtWebApp-1.8.6/QtWebApp/httpserver/*.cpp
    QtWebApp-1.8.6/QtWebApp/httpserver/*.h
)

#file(GLOB GPortal
#    ../src/*.cpp
#    ../src/*.h
#)

set(GPortal
    ../src/CamStream.h
#    ../src/CustomOTA.h
    ../src/GyverPortal.cpp
    ../src/GyverPortal.h
    ../src/TimeTicker.h
    ../src/buildMacro.h
    ../src/builder.h
    ../src/canvas.h
    ../src/list.h
    ../src/log.h
    ../src/objects.h
    ../src/parsers.h
    ../src/portal.h
    ../src/scripts.h
    ../src/themes.h
    ../src/utils.cpp
    ../src/utils.h
    ../src/version.h
)

set(PROJECT_SOURCES
    Arduino.h
    DNSServer.h
    ESPmDNS.h
    FS.h
    Print.h
    Print.cpp
    StreamString.h
    Update.h
    WebServer.h
    WiFi.h
    WString.cpp
    WString.h


    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    GP_JS_TOP.js
    DARK.css
    LIGHT.css


    ${QtWebApp}
    ${GPortal}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Test
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Test APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Test SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Test
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Test PRIVATE
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::WebEngineCore
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
)

set_target_properties(Test PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Test
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Test)
endif()
