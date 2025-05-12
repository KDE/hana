cmake_minimum_required(VERSION 3.16)

project(rina VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Qt6 REQUIRED COMPONENTS Quick Widgets)
find_package(FFmpeg REQUIRED)

add_subdirectory(src)
