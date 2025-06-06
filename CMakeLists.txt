cmake_minimum_required(VERSION 3.16)

project(crypt-browser VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6 组件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets WebEngineWidgets)

# 自动处理 Qt 的 MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 源文件
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/webviewwindow.cpp
    src/archivemanager.cpp
)

# 头文件
set(HEADERS
    src/mainwindow.h
    src/webviewwindow.h
    src/archivemanager.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 链接 Qt 库
target_link_libraries(${PROJECT_NAME} 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::WebEngineWidgets
)

# 设置包含目录
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Windows 特定设置
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif() 