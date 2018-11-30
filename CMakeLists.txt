# 3.8.0 required for cxx_std_17
cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)
project(Microverse VERSION 0.10.8 LANGUAGES C CXX)

# Sets the install directories defined by GNU
include(GNUInstallDirs)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Looks for a appropriate threads package for this platform
find_package(Acid QUIET)
if(NOT ACID_FOUND)
	add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/Acid)
else()
	set(ACID_FOUND true)
endif()

# Microverse sources directory
add_subdirectory(Sources)
