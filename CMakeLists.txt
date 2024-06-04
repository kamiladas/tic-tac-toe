cmake_minimum_required(VERSION 3.5)

project(TicTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)
find_package(Boost REQUIRED)

add_executable(TicTacToe
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    findgamedialog.cpp
    findgamedialog.h
    findgamedialog.ui
)

target_link_libraries(TicTacToe Qt5::Widgets Qt5::Network Boost::boost)

set_target_properties(TicTacToe PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

