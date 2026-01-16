# CMakeLists.txt
# treasure: Treasure's Demonstration Library
# Copyright (C) 2026 Jeffrey M. Engelmann

cmake_minimum_required(VERSION 3.31)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(treasure LANGUAGES C CXX VERSION 0.1.0.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Require Windows
if(NOT WIN32)
    message(FATAL_ERROR "Only Windows builds are supported")
endif()

# Find dependencies
find_package(wil CONFIG REQUIRED)

# Header only library
add_library(treasure INTERFACE)
add_library(treasure::treasure ALIAS treasure)

target_link_libraries(treasure INTERFACE WIL::WIL)

# Define a file set for the public header
target_sources(treasure
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "include"
    FILES "include/treasure/treasure.h")

# Installation commands
install(
    TARGETS treasure
    EXPORT treasureTargets
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/treasure")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/treasureConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/treasureConfig.cmake"
    INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}")

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/treasureConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/treasureConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/treasureConfigVersion.cmake"
    DESTINATION "${CONFIG_INSTALL_DIR}")

install(
    EXPORT treasureTargets
    NAMESPACE treasure::
    DESTINATION "${CONFIG_INSTALL_DIR}")

# Build tests?
if(BUILD_TESTING)
    include(CTest)
    enable_testing()
    add_subdirectory("test")
endif()

###############################################################################
