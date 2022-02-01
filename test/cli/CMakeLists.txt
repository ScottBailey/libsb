cmake_minimum_required(VERSION 3.0.0)

# Specifies the name of the project.
set(target_name cli)
project("${target_name}")

#include the common stuff and find the libsb package
set(LIBSB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
include( ./includecommon.cmake REQUIRED )
find_package(libsb REQUIRED)

# set our flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing" )
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


file(GLOB_RECURSE src_cpp src/*.cpp )
file(GLOB_RECURSE src_h src/*.h)

add_executable(${target_name} ${src_cpp} ${src_h} )
set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 17)
target_include_directories(${target_name} PRIVATE
  src
)
target_link_libraries(${target_name}
  libsb
  pthread dl
)
