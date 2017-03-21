# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.4.1)

# Specifies a library name, specifies whether the library is STATIC or
# SHARED, and provides relative paths to the source code. You can
# define multiple libraries by adding multiple add.library() commands,
# and CMake builds them for you. When you build your app, Gradle
# automatically packages shared libraries with your APK.

set(LIB_NAME luax)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_LIST_DIR}/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/*.c")

add_library(# Specifies the name of the library.
            ${LIB_NAME}

            # Sets the library as a shared library.
            SHARED

            # Provides a relative path to your source
            ${SRCS})

# Include libraries needed for luax lib
target_link_libraries(${LIB_NAME} android log)
