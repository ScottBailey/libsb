cmake_minimum_required(VERSION 3.0.0)

set(target_name fs)
project("${target_name}")

#include the common stuff
include( ./includecommon.cmake REQUIRED )

# Find the libsb package
find_package(libsb REQUIRED)


file(GLOB_RECURSE src_cpp src/*.cpp )
file(GLOB_RECURSE src_h src/*.h)

add_executable(${target_name} ${src_cpp} ${src_h} )
set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 17)

target_include_directories( ${target_name} PRIVATE
  src
  ${LIBSB_INCLUDE_DIR}
  )
