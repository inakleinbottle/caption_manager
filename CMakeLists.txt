cmake_minimum_required(VERSION 3.20)

project(caption_manager)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)



find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Sql)




add_executable(caption_manager
        src/main.cpp
        src/database_manager.cpp
        src/database_manager.h
        src/ImageDelegate.cpp
        src/ImageDelegate.h
        src/CaptionManager.cpp
        src/CaptionManager.h
        src/TagEditorDialog.cpp src/TagEditorDialog.h src/FilterDialog.cpp src/FilterDialog.h)


target_link_libraries(caption_manager PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql)