# example usage:
#  cmake -DCMAKE_TOOLCHAIN_FILE=~/CMakeModules/cross/msp430.cmake ..
# and in your CMakeLists.txt file:
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmcu=msp430g2231")

# Make sure the necessary tools are installed!
#   sudo apt-get install binutils-msp430 gcc-msp430 msp430-libc mspdebug


INCLUDE(CMakeForceCompiler)

# necessary?
set(CMAKE_SYSTEM_NAME "Generic")
#set(CMAKE_SYSTEM_VERSION "???")
set(CMAKE_SYSTEM_PROCESSOR msp430)

# compilers
CMAKE_FORCE_C_COMPILER(msp430-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(msp430-gcc GNU)  # do NOT use g++ since libstdc++.a is NOT available

# root directory
set(CMAKE_FIND_ROOT_PATH /usr/msp430)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# for my own uses
add_definitions(-DBUILD_EMBEDDED_TARGET)
