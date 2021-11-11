# example usage:
#  cmake -DCMAKE_TOOLCHAIN_FILE=~/CMakeModules/cross/ts7390.cmake ..


# necessary!
set(CMAKE_SYSTEM_NAME "Linux")
#set(CMAKE_SYSTEM_VERSION ?)
#set(CMAKE_SYSTEM_PROCESSOR arm)

# compilers
set(CMAKE_C_COMPILER /opt/crosstool/gcc-4.0.1-glibc-2.3.5/arm-unknown-linux-gnu/bin/arm-unknown-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /opt/crosstool/gcc-4.0.1-glibc-2.3.5/arm-unknown-linux-gnu/bin/arm-unknown-linux-gnu-g++)

# root directory
set(CMAKE_FIND_ROOT_PATH /opt/crosstool/gcc-4.0.1-glibc-2.3.5/arm-unknown-linux-gnu/arm-unknown-linux-gnu)
set(CMAKE_FIND_ROOT_PATH /usr/local/linux-arm)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

add_definitions(-DBUILD_EMBEDDED_TARGET)
