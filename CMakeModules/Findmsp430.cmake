#
# Package to support MSP430 micro-controller cbuild and flash
#
# Copyright 2013 Scott Bailey
# License: TBD
#
#
# TODO
#   Note the output of 'msp430-gcc --target-help' and add options as required
#
# Functions/macros defined by this module:
#
#   msp430_mcu(mcu)
#     Sets up flags to compile for the target mcu.
#     e.g. msp430_mcu(msp430f2013)
#     No override available.
#
#   msp430_default_mcu(mcu)
#     Sets up flags to compile for the target mcu.
#     e.g. msp430_default_mcu(msp430f2013)
#     Override by defining MCU on the command line or elsewhere (e.g. -DMCU=msp430f2013)
#
#   msp430_default_attached()
#     Sets up flags to compile for the target mcu that is currently attached.
#     Override by defining MCU on the command line or elsewhere (e.g. -DMCU=msp430f2013)
#
#   msp430_query_attached()
#     Queries the attached device and reports its type in MCU_ATTACHED.
#     If nothing found, unset()s MCU_ATTACHED.
#
#   msp430_disable_watchdog()
#     modifies CMAKE_C_FLAGS and CMAKE_CXX_FLAGS to disable the watchdog timer
#
# Variables (CMake) that this module consumes:
#
#   MCU  This allows the user to override the msp430_default_mcu() function  (e.g. 'cmake <...> -DMCU=msp430f2013')
#
# Variables defined by this module:
#
#   MCU_TARGET    the targeted mcu of this build
#   MCU_FLAG      the flag passed to the compiler for builds (e.g. '-mmcu=msp430f2013')
#   MCU_ATTACHED  the currently attached MCU (when msp430_query_attached() is called)
#
# Variables modified by this moudule:
#
#   CMAKE_C_FLAGS    updated with MCU_FLAG
#   CMAKE_CXX_FLAGS  updated with MCU_FLAG




if( NOT CMAKE_TOOLCHAIN_FILE )
  message(FATAL_ERROR "\nA toolchain file must be defined!\n"
    "(i.e. cmake -DCMAKE_TOOLCHAIN_FILE=~/CMakeModules/cross/msp430.cmake .. )\n"
    "you might need to completely remove your build directory for that to take effect.\n" )
endif()
# example usage of including the toolchain:
#cmake -DCMAKE_TOOLCHAIN_FILE=../presets/msp430_cross.cmake ..


#
# flash related functions
#
FIND_PROGRAM(MSPDEBUG  mspdebug
  DOC "mspdebug is the programmer"
)

if(NOT MSPDEBUG)
  message(WARNING "Failed to find mspdebug flash programmer.")
endif()

# Flash a file to your device!
function(msp430_flash name_)
  add_custom_target(flash)
  add_dependencies(flash ${name_})

  add_custom_command(TARGET flash POST_BUILD
    COMMAND ${MSPDEBUG} rf2500 \"prog ${name_}\"
    )
endfunction()

add_custom_target(reset)
add_custom_command(TARGET reset POST_BUILD
    COMMAND  echo "Reset NYI"
)

#
# MCU related functions
#
function(msp430_mcu mcu_)
  # local scope
  string(TOLOWER "${mcu_}" mcu_)
  set(MCU_FLAG "-mmcu=${mcu_}" )
  # pass these out into the parent scope:
  set(MCU_TARGET ${mcu_} PARENT_SCOPE)
  set(MCU_FLAG "-mmcu=${mcu_}" PARENT_SCOPE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MCU_FLAG}" PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MCU_FLAG}" PARENT_SCOPE)
  message(STATUS "msp430 mcu: ${mcu_}")
endfunction()

macro(msp430_default_mcu mcu_)
  if(NOT MCU_TARGET)
    message(STATUS "Selecting a default mcu.  Override with -DMCU=<mcu>")
    msp430_mcu(${mcu_})
  endif()
endmacro()

macro(msp430_default_attached)
  if(NOT MCU_TARGET)
    message(STATUS "Attempting to select attached mcu as defaut...  (Override with -DMCU=<mcu>)")
    msp430_query_attached(mcu_)
    if(mcu_)
      msp430_mcu(${mcu_})
    else()
      message(WARNING "\tNo device found!")
    endif()
  endif()
endmacro()

if(NOT MCU_TARGET)
  if(MCU)
    msp430_mcu(${MCU})
  endif()
endif()


# query attached device!
#  sets MCU_ATTACHED
function(msp430_query_attached target_)

  unset(${target_})

  execute_process(
    COMMAND ${MSPDEBUG} rf2500 exit
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    TIMEOUT 5    # in seconds
    RESULT_VARIABLE result_
    OUTPUT_VARIABLE output_
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
    )

  if( result_ EQUAL 0 ) # success
    string(REGEX MATCH "Device[:] [Mm][Ss][Pp]430[a-zA-Z][0-9][0-9][0-9][0-9]" temp1_ ${output_} )  # msp430.DDDD
    if( temp1_ )
      string(REGEX MATCH "MSP430....." temp2_ ${temp1_})
      if( temp2_ )
        string(TOLOWER "${temp2_}" temp2_)
        set(${target_} ${temp2_} PARENT_SCOPE)
      endif()
    endif()
  endif()

endfunction()


function(msp430_disable_watchdog)
  set(DISABLE_FLAG "-mdisable-watchdog")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DISABLE_FLAG}" PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DISABLE_FLAG}" PARENT_SCOPE)
endfunction()
