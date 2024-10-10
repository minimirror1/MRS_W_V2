cmake_minimum_required(VERSION 3.16)

project(gui_v2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 프로젝트 소스 및 리소스 파일 설정
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        resources.qrc # 리소스 파일을 소스에 추가
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(gui_v2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(gui_v2 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(gui_v2
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Qt 위젯 라이브러리와 링크
target_link_libraries(gui_v2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# macOS 및 Windows 번들 관련 설정
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.gui_v2)
endif()
set_target_properties(gui_v2 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 설치 정보
include(GNUInstallDirs)
install(TARGETS gui_v2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Qt 6 이상에서 필요 시 최종 설정
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gui_v2)
endif()
