cmake_minimum_required(VERSION 3.16)
project(church_lang VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
    main.cpp
    LimitedTextEdit.cpp
)

if(WIN32)
    add_executable(church_lang WIN32 ${PROJECT_SOURCES})
else()
    add_executable(church_lang ${PROJECT_SOURCES})
endif()

target_link_libraries(church_lang
    PRIVATE
        Qt6::Widgets
        Qt6::Multimedia
)

add_custom_command(TARGET church_lang
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:church_lang>/res"
)
