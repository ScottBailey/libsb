# example usage:
#  cmake .. -DCMAKE_TOOLCHAIN_FILE=~/CMakeModules/cross/mingw-w64.cmake
#
# This will set CMAKE_CROSSCOMPILING true, and may be tested in CMake files
#
# For more info on cross compiling see: http://www.cmake.org/cmake/help/git-master/manual/cmake-toolchains.7.html


# the name of the target operating system
set(CMAKE_SYSTEM_NAME Windows)
set(TOOLCHAIN_PREFIX i686-w64-mingw32)

# cross compilers to use for C and C++
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)

# here is where the target environment located
set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX} $ENV{HOME}/installs/win32 )

# search where?
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
