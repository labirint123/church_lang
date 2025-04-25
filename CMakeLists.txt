cmake_minimum_required(VERSION 3.16)
project(church_lang VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
 
set(PROJECT_SOURCES
    main.cpp
    LimitedTextEdit.cpp
)

add_executable(church_lang ${PROJECT_SOURCES})

target_link_libraries(church_lang
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
)

add_custom_command(TARGET church_lang
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:church_lang>"
)
