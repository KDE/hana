#
# SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause
#

cmake_minimum_required(VERSION 3.16)

cmake_policy(SET CMP0048 NEW)
project(hana VERSION 0.1 LANGUAGES CXX)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src)
set(QML_IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY} CACHE PATH "QML import paths")

set(QT_MIN_VERSION 6.6.0)
set(KF6_MIN_VERSION 6.6.0)

include(FeatureSummary)

# -------------------------------
#
# -- Qt6 dependencies
#
# -------------------------------
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

# -------------------------------
#
# -- KF6 dependencies
#
# -------------------------------
find_package(ECM ${KF6_MIN_VERSION})
set_package_properties(ECM PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/ecm/"
    DESCRIPTION "extra cmake modules")
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(KF6Config ${KF6_MIN_VERSION})
set_package_properties(KF6Config PROPERTIES
    TYPE REQUIRED URL "https://api.kde.org/kconfig-index.html")

find_package(KF6CoreAddons ${KF6_MIN_VERSION})
set_package_properties(KF6CoreAddons PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/kcoreaddons-index.html")

find_package(KF6I18n ${KF6_MIN_VERSION})
set_package_properties(KF6I18n PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/ki18n-index.html")

find_package(KF6KIO ${KF6_MIN_VERSION})
set_package_properties(KF6KIO PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/kio-index.html")

find_package(KF6Kirigami ${KF6_MIN_VERSION})
set_package_properties(KF6Kirigami PROPERTIES TYPE REQUIRED
    URL "https://api.kde.org/kirigami-index.html")

find_package(FFmpeg)
set_package_properties(FFmpeg PROPERTIES TYPE REQUIRED
    URL "https://ffmpeg.org"
    DESCRIPTION "needed for thumbnail generation")

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

include(KDEInstallDirs)
include(ECMInstallIcons)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMSetupVersion)
ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX HANA
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/hana-version.h"
)

ki18n_install(po)

add_subdirectory(data)
add_subdirectory(src)
