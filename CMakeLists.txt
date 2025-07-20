cmake_minimum_required(VERSION 3.16)

project(rina VERSION 0.1 LANGUAGES CXX)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src)
set(QML_IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY} CACHE PATH "QML import paths")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(FeatureSummary)

find_package(ECM ${KF6_MIN_VERSION})
set_package_properties(ECM PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/ecm/"
    DESCRIPTION "extra cmake modules")
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Qt6 REQUIRED COMPONENTS Quick Widgets)

find_package(Qt6Core ${QT_MIN_VERSION})
set_package_properties(Qt6Core PROPERTIES TYPE REQUIRED)

find_package(Qt6Gui ${QT_MIN_VERSION})
set_package_properties(Qt6Gui PROPERTIES TYPE REQUIRED)

find_package(Qt6Qml ${QT_MIN_VERSION})
set_package_properties(Qt6Qml PROPERTIES TYPE REQUIRED)

find_package(Qt6Quick ${QT_MIN_VERSION})
set_package_properties(Qt6Quick PROPERTIES TYPE REQUIRED)

find_package(Qt6QuickControls2 ${QT_MIN_VERSION})
set_package_properties(Qt6QuickControls2 PROPERTIES TYPE REQUIRED)

find_package(KF6Config ${KF6_MIN_VERSION})
set_package_properties(KF6Config PROPERTIES
    TYPE REQUIRED URL "https://api.kde.org/frameworks/kconfig/html/index.html")

find_package(KF6Kirigami ${KF6_MIN_VERSION})
set_package_properties(KF6Kirigami PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/frameworks/kirigami/html/index.html")

find_package(FFmpeg)
set_package_properties(FFmpeg PROPERTIES TYPE REQUIRED
    URL "https://ffmpeg.org"
    DESCRIPTION "needed for playlist and mpris thumbnails")

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

add_subdirectory(src)
