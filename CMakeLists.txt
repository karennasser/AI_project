cmake_minimum_required(VERSION 3.16)

project(AI_project_2026 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    AIPlayer.cpp
    AIPlayer.h
    Board.cpp
    Board.h
    gameboardwidget.cpp
    gameboardwidget.h
    GameController.cpp
    GameController.h
    move.cpp
    move.h
    PathFinder.cpp
    PathFinder.h
    Wall.h
    Wall.cpp
    player.cpp
    player.h
    WallValidator.cpp
    WallValidator.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AI_project_2026
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(AI_project_2026 ${PROJECT_SOURCES})
endif()

target_link_libraries(AI_project_2026 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(AI_project_2026 PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE FALSE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AI_project_2026)
endif()