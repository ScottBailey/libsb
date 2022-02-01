# Find a user include path
# First determine the user's home path (differs per OS)
if( CMAKE_HOST_SYSTEM_NAME MATCHES "Windows.*" )
    file(TO_CMAKE_PATH "$ENV{HOMEDRIVE}$ENV{HOMEPATH}" USER_HOME_PATH)
else()
    file(TO_CMAKE_PATH "$ENV{HOME}" USER_HOME_PATH)
endif()
# these are possible suffixes below the user's home path
set( USER_HOME_MODULES_SUFFIXES
  CMakeModules
  libsb/CMakeModules
)

# try to find the common.cmake file
find_path(USER_HOME_MODULES_PATH
    common.cmake
  PATHS
    ${LIBSB_ROOT}
    ${USER_HOME_PATH}/.local
    ${USER_HOME_PATH}
    ${USER_HOME_PATH}/bailey
  PATH_SUFFIXES
    ${USER_HOME_MODULES_SUFFIXES}
  NO_DEFAULT_PATH
  NO_CMAKE_ENVIRONMENT_PATH
  NO_CMAKE_PATH
  NO_SYSTEM_ENVIRONMENT_PATH
  NO_CMAKE_SYSTEM_PATH
  NO_CMAKE_FIND_ROOT_PATH
)
# Test to see if it exists:
if( NOT USER_HOME_MODULES_PATH )
    message(FATAL_ERROR
      "Could not find the common.cmake file in your user home directory.\n"
      "Home directory: ${USER_HOME_PATH}\n"
      "Suffixes: ${USER_HOME_MODULES_SUFFIXES}\n"
    )
endif()
message(STATUS "USER_HOME_MODULES_PATH: ${USER_HOME_MODULES_PATH}")
# set the cmake modules path!
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR} ${USER_HOME_MODULES_PATH} ${CMAKE_MODULE_PATH})

# add any extra locations...
if( WIN32 )
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} )
else()
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/share/scripts)
endif()

include( common REQUIRED )
