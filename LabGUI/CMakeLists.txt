cmake_minimum_required(VERSION 3.5)
set(CMAKE_PREFIX_PATH "C:/cpp_libs/vcpkg/installed/x64-windows;C:/cpp_libs/vcpkg/installed/x64-windows/lib;C:/Qt/6.7.1/msvc2019_64/lib/cmake/Qt6")

project(LabGUI VERSION 0.1 LANGUAGES CXX)
set(DEPLOY_DIR "C:/Users/mmate/SiMR/Laboratorium-SiMR/LabGUI/deploy")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_BUILD_TYPE Debug)


set(SRC_DIR "C:/Users/mmate/SiMR/Laboratorium-SiMR/LabGUI/src")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(Eigen3_DIR "C:/cpp_libs/vcpkg/buildtrees/eigen3/x64-windows-rel")
set(BOOST_INCLUDE_DIR "C:/cpp_libs/boost_1_85_0")
set(BOOST_IOSTREAMS_LIB "C:/cpp_libs/boost_1_85_0/stage/lib/libboost_iostreams-vc143-mt-gd-x64-1_85.lib")
set(BOOST_IOSTREAMS_LIB_DEPLOY "C:/cpp_libs/boost_1_85_0/stage/lib/libboost_iostreams-vc143-mt-x64-1_85.lib")
set(BOOST_FILESYSTEM_LIB "C:/cpp_libs/boost_1_85_0/stage/lib/libboost_filesystem-vc143-mt-gd-x64-1_85.lib")
set(BOOST_FILESYSTEM_LIB_DEPLOY "C:/cpp_libs/boost_1_85_0/stage/lib/libboost_filesystem-vc143-mt-x64-1_85.lib")
set(MATPLOTLIB_DIR "C:/cpp_libs/vcpkg/installed/x64-windows/include")
set(GNUPLOT_DIR "C:/cpp_libs/gnuplot-iostream")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools DataVisualization Graphs Charts OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools DataVisualization Graphs Charts OpenGL)
# find_package(Vulkan REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(Boost REQUIRED iostreams)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(ZLIB REQUIRED)

set(TS_FILES LabGUI_pl_PL.ts)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/interface.cpp
    src/interface.h
    src/mainmenu.cpp
    src/mainmenu.h
    src/glplotwidget.cpp
    src/glplotwidget.h
    src/submenu.cpp
    src/submenu.h
    src/baseinterfacewidget.cpp
    src/baseinterfacewidget.h
)

include_directories(${PROJECT_SOURCES}
                    ${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include/eigen3
                    ${Eigen3_DIR}
                    ${BOOST_INCLUDE_DIR}
                    ${GNUPLOT_DIR}
                    PRIVATE ${OPENGL_INCLUDE_DIR}
                    )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LabGUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET LabGUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(LabGUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(LabGUI
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(LabGUI
                        PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                        PRIVATE Qt${QT_VERSION_MAJOR}::DataVisualization
                        PRIVATE Qt${QT_VERSION_MAJOR}::Graphs
                        PRIVATE Qt${QT_VERSION_MAJOR}::Charts
                        PRIVATE Qt${QT_VERSION_MAJOR}::OpenGL
                        ${OPENGL_gl_LIBRARY}
                        PRIVATE OpenGL::GL
                        PRIVATE OpenGL::GLU
                        # PRIVATE Vulkan::Vulkan
                        PRIVATE Eigen3::Eigen
                        ${BOOST_IOSTREAMS_LIB}
                        ${BOOST_IOSTREAMS_LIB_DEPLOY}
                        ${BOOST_FILESYSTEM_LIB}
                        ${BOOST_FILESYSTEM_LIB_DEPLOY}
                        PRIVATE ZLIB::ZLIB)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.LabGUI)
endif()
set_target_properties(LabGUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS LabGUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LabGUI)
endif()
