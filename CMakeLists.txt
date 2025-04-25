cmake_minimum_required(VERSION 3.16)
project(church_lang VERSION 0.1 LANGUAGES CXX)

# Чтобы не писать вручную qt_wrap_* для каждого .ui/.qrc
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
# Ищем либо Qt6, либо Qt5 (Widgets)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
 
# Указываем все исходники + ресурсник
set(PROJECT_SOURCES
    main.cpp
)

# Обычный add_executable — AUTOMOC/AUTORCC сам сгенерит rcc_*.cpp
add_executable(church_lang ${PROJECT_SOURCES})

target_link_libraries(church_lang
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
)
