cmake_minimum_required(VERSION 3.5)

project(Test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(${CMAKE_CURRENT_SOURCE_DIR})

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
    -DGP_NO_MDNS
    -DGP_NO_DNS
    -DGP_NO_OTA
    -DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

include_directories(
    ../src
    hw
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
    ../src/src.ino
)

set(PROJECT_SOURCES
    hw/Arduino.h
    hw/DNSServer.h
    hw/ESPmDNS.h
    hw/FS.h
    hw/Print.h
    hw/Print.cpp
    hw/StreamString.h
    hw/Update.h
    hw/WebServer.h
    hw/WiFi.h
    hw/WString.cpp
    hw/WString.h
    hw/xmlhighlighter.h
    hw/xmlhighlighter.cpp


    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    GP_JS_TOP.js
    DARK.css
    LIGHT.css
    tests.md


    ${QtWebApp}
    ${GPortal}

    ../examples/system/accessNetIp/accessNetIp.ino
    ../examples/demos/actionClick/actionClick.ino
    ../examples/demos/actionForm/actionForm.ino
    ../examples/demos/actionUpdate/actionUpdate.ino
    ../examples/design/blocks/blocks.ino
    ../examples/design/blocks2/blocks2.ino
    ../examples/design/boxes/boxes.ino
    ../examples/design/boxesMacro/boxesMacro.ino
    ../examples/demos/buttonHold/buttonHold.ino
    ../examples/demos/canvas/canvas.ino
    ../examples/customComponents/customComponents.ino
    ../examples/demos/demoAllComponents/demoAllComponents.ino
    ../examples/demos/demoAuth/demoAuth.ino
    ../examples/demos/demoColor/demoColor.ino
    ../examples/demos/demoLog/demoLog.ino
    ../examples/demos/demoLogManual/demoLogManual.ino
    ../examples/builds/dynamicComponents/dynamicComponents.ino
    ../examples/projects/eepromLogPlot/eepromLogPlot.ino
    ../examples/demos/esp32-stream/esp32-stream.ino
    ../examples/files/fileDownloadAuto/fileDownloadAuto.ino
    ../examples/files/fileDownloadManual/fileDownloadManual.ino
    ../examples/files/fileEmbed/fileEmbed.ino
    ../examples/files/fileUploadAuto/fileUploadAuto.ino
    ../examples/files/fileUploadManual/fileUploadManual.ino
    ../examples/design/GridAndBlocks/GridAndBlocks.ino
    ../examples/design/icons/icons.ino
    ../examples/demos/jQupdate/jQupdate.ino
    ../examples/design/layoutApp/layoutApp.ino
    ../examples/design/layoutDashboard/layoutDashboard.ino
    ../examples/system/localLambda/localLambda.ino
    ../examples/system/localPortal/localPortal.ino
    ../examples/system/localPortal2/localPortal2.ino
    ../examples/design/multiPageButtons/multiPageButtons.ino
    ../examples/design/multiPageTabLinks/multiPageTabLinks.ino
    ../examples/design/multiPageTabs/multiPageTabs.ino
    ../examples/design/multiPageTabsManual/multiPageTabsManual.ino
    ../examples/demos/objects/objects.ino
    ../examples/demos/objTest/objTest.ino
    ../examples/demos/onlineCheck/onlineCheck.ino
    ../examples/system/OTAcustom/OTAcustom.ino
    ../examples/system/OTAupdate/OTAupdate.ino
    ../examples/demos/pageTitle/pageTitle.ino
    ../examples/demos/plotAjax/plotAjax.ino
    ../examples/demos/plotStatic/plotStatic.ino
    ../examples/demos/plotStock/plotStock.ino
    ../examples/demos/popupAlert/popupAlert.ino
    ../examples/demos/popupConfirm/popupConfirm.ino
    ../examples/demos/popupPrompt/popupPrompt.ino
    ../examples/system/rtos2core/rtos2core.ino
    ../examples/demos/saveSettings/saveSettings.ino
    ../examples/builds/selectBuild/selectBuild.ino
    ../examples/design/slidersEqual/slidersEqual.ino
    ../examples/demos/systemInfo/systemInfo.ino
    ../examples/design/tables/tables.ino
    ../examples/demos/tableWrandom/tableWrandom.ino
    ../examples/design/ui/ui.ino
    ../examples/design/uiManual/uiManual.ino
    ../examples/projects/webSerial/webSerial.ino
    ../examples/projects/wifiLogin/wifiLogin.ino

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
