cmake_minimum_required(VERSION 3.4)

set(target_name endian_test)
project("${target_name}")

#include the common stuff
include( ./includecommon.cmake REQUIRED )

# Find the libsb package
find_package(libsb REQUIRED)

# Boost!
find_package( Boost REQUIRED)

if( NOT WIN32 AND NATIVE_SIZE EQUAL 64 )
  set( Boost_LIBRARIES ${Boost_LIBRARIES} rt)
endif()

# set our flags
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing" )
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


file(GLOB_RECURSE src_cpp src/*.cpp )
file(GLOB_RECURSE src_h/*.h)

add_executable(${target_name} ${src_cpp} ${src_h} )
set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 14)

target_include_directories( ${target_name} PRIVATE
  src
  ${LIBSB_INCLUDE_DIR}
  )
target_include_directories( ${target_name} PUBLIC SYSTEM
  ${Boost_INCLUDE_DIRS}
  )

target_link_libraries(${target_name}
  ${Boost_LIBRARIES}  #boost
  pthread dl
)
