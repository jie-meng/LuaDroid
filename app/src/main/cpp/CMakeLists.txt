# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.4.1)

# Specifies a library name, specifies whether the library is STATIC or
# SHARED, and provides relative paths to the source code. You can
# define multiple libraries by adding multiple add.library() commands,
# and CMake builds them for you. When you build your app, Gradle
# automatically packages shared libraries with your APK.

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -DLUA_ANSI")

add_library(# Specifies the name of the library.
            luax

            # Sets the library as a shared library.
            SHARED

            # Provides a relative path to your source file(s).
            luax.cpp
            lua/lapi.c
            lua/ldo.c
            lua/linit.c
            lua/lparser.c
            lua/lstrlib.c
            lua/lutf8lib.c
            lua/lcorolib.c
            lua/liolib.c
            lua/loadlib.c
            lua/ltable.c
            lua/lua.hpp
            lua/lvm.c
            lua/lauxlib.c
            lua/lctype.c
            lua/ldump.c
            lua/llex.c
            lua/lobject.c
            lua/luac.c
            lua/lfunc.c
            lua/lstate.c
            lua/ltablib.c
            lua/lzio.c
            lua/lbaselib.c
            lua/ldblib.c
            lua/lopcodes.c
            lua/ltm.c
            lua/lbitlib.c
            lua/ldebug.c
            lua/lgc.c
            lua/lmathlib.c
            lua/lstring.c
            lua/lundump.c
            lua/lcode.c
            lua/lmem.c
            lua/loslib.c
lua/lua.c)

# Include libraries needed for luax lib
target_link_libraries(luax
                   android
                   log)