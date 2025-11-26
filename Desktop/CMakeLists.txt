cmake_minimum_required(VERSION 3.22)
project(Desktop LANGUAGES CXX)

set(DESKTOP_SOURCES
    main.cpp
    mainwindow.cpp
)

set(DESKTOP_HEADERS
    mainwindow.h
)

set(DESKTOP_FORMS
    mainwindow.ui
)

# Enable AUTOMOC and AUTOUIC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql Network)

add_executable(Desktop ${DESKTOP_SOURCES} ${DESKTOP_HEADERS} ${DESKTOP_FORMS})

target_link_libraries(Desktop PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::Network
    Backend
)

# QCustomPlot
target_include_directories(Desktop PRIVATE
    ${CMAKE_SOURCE_DIR}/Backend
    ${CMAKE_SOURCE_DIR}/Backend/Modules
    ${CMAKE_SOURCE_DIR}/Desktop/QCustomPlot
)
target_link_libraries(Desktop PRIVATE
    ${CMAKE_SOURCE_DIR}/Desktop/QCustomPlot/qcustomplotd2.lib
)
