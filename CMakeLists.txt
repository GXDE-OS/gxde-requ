cmake_minimum_required(VERSION 3.16)
project(gxde-requ VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS DBus Widgets)

qt_add_executable(gxde-requ WIN32 MACOSX_BUNDLE
    main.cpp
    requ.cpp requ.h
    widget.cpp widget.h
)
target_compile_definitions(gxde-requ PRIVATE
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(gxde-requ PRIVATE
    Qt::Core
    Qt::Gui
)

if(LINUX)
    target_link_libraries(gxde-requ PRIVATE
        Qt::DBus
    )
endif()

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(gxde-requ PRIVATE
        Qt::Widgets
    )
endif()

install(TARGETS gxde-requ
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
