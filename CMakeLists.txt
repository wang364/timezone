cmake_minimum_required(VERSION 3.16)

project(timezone VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options("/Zc:__cplusplus")
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(apptimezone
    main.cpp
    TrayIcon.cpp
    TrayIcon.h
    SettingsWindow.cpp
    SettingsWindow.h
    TimezoneWindow.cpp
    TimezoneWindow.h
    CityManager.cpp
    CityManager.h
    resources.qrc
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apptimezone PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptimezone
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apptimezone
    PRIVATE Qt6::Widgets Qt6::Concurrent
)

include(GNUInstallDirs)
install(TARGETS apptimezone
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
