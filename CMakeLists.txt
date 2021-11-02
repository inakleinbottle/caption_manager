cmake_minimum_required(VERSION 3.20)

project(caption_manager)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)



find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Sql)




add_executable(caption_manager
        src/main.cpp
        src/caption_manager.cpp
        src/caption_manager.h
        src/caption_manager.ui src/database_functions.h src/database_functions.cpp)


target_link_libraries(caption_manager PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql)