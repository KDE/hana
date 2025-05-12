cmake_minimum_required(VERSION 3.16)

project(rina VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

add_subdirectory(src)
