cmake_minimum_required(VERSION 3.5)
project(MigrationAssistance VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC_SEARCH_PATHS pages)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    pages/mainwindow.ui
    src/findDisk/findDisk.cpp
    src/findDisk/findDisk.h
    src/mount/mount.cpp
    src/mount/mount.h
    pages/diskchosePage/diskChoosePage.cpp
    pages/diskchosePage/diskChoosePage.h
    
    resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MigrationAssistance
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(MigrationAssistance ${PROJECT_SOURCES}



    )
endif()

target_include_directories(MigrationAssistance PRIVATE
    ${CMAKindDiskindDiskE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mount
    ${CMAKE_CURRENT_SOURCE_DIR}/src/findDisk
    ${CMAKE_CURRENT_SOURCE_DIR}/pages
    ${CMAKE_CURRENT_SOURCE_DIR}/pages/diskchosePage
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(MigrationAssistance PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(MigrationAssistance PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MigrationAssistance)
endif()
