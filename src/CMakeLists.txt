cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/favicon.rc")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core)

    qt_add_executable(ISD
        MANUAL_FINALIZATION
        main.cpp
        resources/resources.qrc
        ${app_icon_resource_windows}
        dialogs/profilewindow.h dialogs/profilewindow.cpp dialogs/profilewindow.ui
        dialogs/profilechecker.h dialogs/profilechecker.cpp dialogs/profilechecker.ui

    )
else()
    if(ANDROID)
        add_library(ISD SHARED
            main.cpp
            resources/resources.qrc
        )
    else()
        add_executable(ISD
            main.cpp
            resources/resources.qrc
        )
    endif()
endif()


add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(dialogs)

target_link_libraries(ISD PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
)

target_include_directories(ISD PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ISD)
endif()
set_target_properties(ISD PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>
)

include(GNUInstallDirs)
install(TARGETS ISD
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ISD)
endif()

# --- Automatic DLL Deployment ---
if(WIN32)
    get_target_property(_qt_bin_dir Qt6::Core LOCATION)
    get_filename_component(_qt_bin_dir "${_qt_bin_dir}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET ISD POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:ISD>"
            COMMENT "Running windeployqt to copy dependencies..."
        )
    endif()
endif()
