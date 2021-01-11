cmake_minimum_required(VERSION 3.5)

project(edu-DirMonitor-Client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(edu-DirMonitor-Client SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(edu-DirMonitor-Client
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    src/client.cpp
    src/client.h
  )
endif()

target_link_libraries(edu-DirMonitor-Client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

find_package(nlohmann_json 3.2.0 REQUIRED)
target_link_libraries(edu-DirMonitor-Client PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(edu-DirMonitor-Client PRIVATE pthread)
