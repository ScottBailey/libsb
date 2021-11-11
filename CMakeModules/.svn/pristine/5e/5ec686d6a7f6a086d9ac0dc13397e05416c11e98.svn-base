# example usage:
#  cmake .. -DCMAKE_TOOLCHAIN_FILE=~/CMakeModules/cross/mingw32.cmake
#
# This will set CMAKE_CROSSCOMPILING true, and may be tested in CMake files
#
# For more info on cross compiling see: http://www.cmake.org/cmake/help/git-master/manual/cmake-toolchains.7.html


# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER i586-mingw32msvc-gcc)
SET(CMAKE_CXX_COMPILER i586-mingw32msvc-g++)
SET(CMAKE_RC_COMPILER i586-mingw32msvc-windres)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  /usr/i586-mingw32msvc $ENV{HOME}/installs/win32 )

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)