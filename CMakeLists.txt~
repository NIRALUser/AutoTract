cmake_minimum_required(VERSION 2.8.3)
cmake_policy(VERSION 2.8.3)

project(AutoTract)

set(LOCAL_PROJECT_NAME AutoTract)

message(STATUS "-----------------------------reading CMakelists.txt------------------------------")
# Project version number.
set(${LOCAL_PROJECT_NAME}_VERSION_MAJOR "0")
set(${LOCAL_PROJECT_NAME}_VERSION_MINOR "1")
set(${LOCAL_PROJECT_NAME}_VERSION_PATCH "1")



#-----------------------------------------------------------------------------
# Standalone vs Slicer extension option
#

# This option should be named after the project name, it corresponds to the
# option set to ON when the project is build by the Slicer Extension build
# system.


if( DEFINED Slicer_DIR)
  option(${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION "Build as a Slicer Extension" ON)

  find_package(Slicer REQUIRED)
  include(${Slicer_USE_FILE})

  set(_qt_version "4")
  if (Slicer_REQUIRED_QT_VERSION VERSION_GREATER "4.9")
    set(_qt_version "5")
    option(Qt4_SUPPORT "Build with QT4 support." OFF)
  else()
    option(Qt4_SUPPORT "Build with QT4 support." ON)
  endif()

else()
  option(Qt4_SUPPORT "Build with QT4 support." ON)
endif()



#-----------------------------------------------------------------------------
# Update CMake module path
# We need to update the CMake Module path in this main CMakeLists.txt file
# so that we can include SlicerExtensionsConfigureMacros which is in the current
# ${Project}/CMake folder
#------------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# Update CMake module path
#------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake
  ${CMAKE_SOURCE_DIR}/SuperBuild
  ${CMAKE_MODULE_PATH}
  )


include(${CMAKE_CURRENT_SOURCE_DIR}/Common.cmake)

## NOTE THERE SHOULD BE NO PROJECT STATEMENT HERE!
## This file acts as a simple switch to initiate
## two completely independant CMake build environments.

#-----------------------------------------------------------------------------
# Superbuild Option - Enabled by default
#                   Phase I:  ${LOCAL_PROJECT_NAME}_SUPERBUILD is set to ON, and the
#                             supporting packages defined in "SuperBuild.cmake"
#                             are built.  The last package in "SuperBuild.cmake"
#                             to be built is a recursive call to this
#                             file with ${LOCAL_PROJECT_NAME}_SUPERBUILD explicitly
#                             set to "OFF" to initiate Phase II
#
#                   Phase II: Build the ${LOCAL_PROJECT_NAME}, referencing the support
#                             packages built in Phase I.
#-----------------------------------------------------------------------------


option(${LOCAL_PROJECT_NAME}_SUPERBUILD "Build ${LOCAL_PROJECT_NAME} and the projects it depends on via SuperBuild.cmake." ON)


#-----------------------------------------------------------------------------
# Superbuild script
#-----------------------------------------------------------------------------
if(${LOCAL_PROJECT_NAME}_SUPERBUILD)
  include("${CMAKE_CURRENT_SOURCE_DIR}/SuperBuild.cmake")
  return()
else()
  include("${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_PROJECT_NAME}.cmake")
  return()
endif()

message(FATAL_ERROR "You should never reach this point !")
