cmake_minimum_required(VERSION 3.0.0)

set(target_name accum)
project("${target_name}")

# include the common stuff
include( ./includecommon.cmake REQUIRED )

# Find the libsb package
find_package(libsb REQUIRED)


# Boost!
find_package( Boost REQUIRED date_time thread system chrono)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

if( NOT WIN32 AND NATIVE_SIZE EQUAL 64 )
  set( Boost_LIBRARIES ${Boost_LIBRARIES} rt)
endif()


# The libsb include dir:
include_directories( src )

file(GLOB_RECURSE src_cpp src/*.cpp )
file(GLOB_RECURSE src_h src/*.h)

add_executable(${target_name} ${src_cpp} ${src_h} )
set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 17)

target_include_directories( ${target_name} PUBLIC
  ${LIBSB_INCLUDE_DIR}
  )
target_include_directories(${target_name} PUBLIC SYSTEM
  ${Boost_INCLUDE_DIRS}
  )

target_link_libraries(${target_name}
  ${Boost_LIBRARIES}  #boost
  pthread dl
)
