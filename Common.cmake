include(CMakeDependentOption)
#-----------------------------------------------------------------------------
enable_language(C)
enable_language(CXX)
#-----------------------------------------------------------------------------
# Build option(s)
#-----------------------------------------------------------------------------

set(PRIMARY_PROJECT_NAME ${LOCAL_PROJECT_NAME})

option(INSTALL_DEVELOPMENT_${LOCAL_PROJECT_NAME} "Install development support include and libraries for external packages." OFF)
mark_as_advanced(INSTALL_DEVELOPMENT_${LOCAL_PROJECT_NAME})

set(VERSION_MAJOR_ITK 4)
# Set the possible values of ITK major version for cmake-gui
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

set(USE_ITKv4 ON)
#-----------------------------------------------------------------------------
# Update CMake module path
#------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/SuperBuild
  ${CMAKE_MODULE_PATH}
  )

#-----------------------------------------------------------------------------
# Sanity checks
#------------------------------------------------------------------------------
include(PreventInSourceBuilds)
include(PreventInBuildInstalls)
include(SlicerExtensionsConfigureMacros)


include(CMakeParseArguments)

#-----------------------------------------------------------------------------
# Platform check
#-----------------------------------------------------------------------------
set(PLATFORM_CHECK true)
if(PLATFORM_CHECK)
  # See CMake/Modules/Platform/Darwin.cmake)
  # 6.x == Mac OSX 10.2 (Jaguar)
  # 7.x == Mac OSX 10.3 (Panther)
  # 8.x == Mac OSX 10.4 (Tiger)
  # 9.x == Mac OSX 10.5 (Leopard)
  # 10.x == Mac OSX 10.6 (Snow Leopard)
  if (DARWIN_MAJOR_VERSION LESS "9")
    message(FATAL_ERROR "Only Mac OSX >= 10.5 are supported !")
  endif()
endif()

#-----------------------------------------------------------------------------
# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

#-----------------------------------------------------------------------------
if(NOT COMMAND SETIFEMPTY)
  macro(SETIFEMPTY)
    set(KEY ${ARGV0})
    set(VALUE ${ARGV1})
    if(NOT ${KEY})
      set(${ARGV})
    endif()
  endmacro()
endif()

#-----------------------------------------------------------------------------
SETIFEMPTY(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
SETIFEMPTY(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
SETIFEMPTY(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

#-----------------------------------------------------------------------------
SETIFEMPTY(CMAKE_INSTALL_LIBRARY_DESTINATION lib)
SETIFEMPTY(CMAKE_INSTALL_ARCHIVE_DESTINATION lib)
SETIFEMPTY(CMAKE_INSTALL_RUNTIME_DESTINATION bin)

#-------------------------------------------------------------------------
# Augment compiler flags
#-------------------------------------------------------------------------
include(ITKSetStandardCompilerFlags)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_DEBUG_DESIRED_FLAGS}" )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_DEBUG_DESIRED_FLAGS}" )
else() # Release, or anything else
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_RELEASE_DESIRED_FLAGS}" )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_RELEASE_DESIRED_FLAGS}" )
endif()

#-----------------------------------------------------------------------------
# Add needed flag for gnu on linux like enviroments to build static common libs
# suitable for linking with shared object libs.
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  if(NOT "${CMAKE_CXX_FLAGS}" MATCHES "-fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  endif()
  if(NOT "${CMAKE_C_FLAGS}" MATCHES "-fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  endif()
endif()

MARK_AS_ADVANCED(BUILD_TESTING, CMAKE_INSTALL_PREFIX CMAKE_INCLUDE_DIRECTORIES_BEFORE EXTERNAL_BINARY_DIRECTORY EXTERNAL_SOURCE_DIRECTORY FORCE_EXTERNAL_BUILDS)

#-----------------------------------------------------------------------------
# SLICER EXTENSION
#-----------------------------------------------------------------------------
if( ${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION )
  set(EXTENSION_NAME ${LOCAL_PROJECT_NAME} )
  set(MODULE_NAME ${LOCAL_PROJECT_NAME} )
  set(EXTENSION_HOMEPAGE "https://github.com/NIRALUser/AutoTract")
  set(EXTENSION_CATEGORY "Tractography")
  set(EXTENSION_CONTRIBUTORS "Jean-Yves Yang (NIRAL, UNC), Francois Budin (NIRAL, UNC), Juan Carlos prieto (NIRAL, UNC), Adrien Boucaud (NIRAL, UNC), Mateo Lopez (NIRAL, UNC)")
  set(EXTENSION_DESCRIPTION "An automatic tractography tool featuring advanced processing tools to clean fiber tracts after the initial tractography. ")
  set(EXTENSION_ICONURL "https://www.nitrc.org/project/screenshot.php?group_id=948&screenshot_id=935")
  set(EXTENSION_SCREENSHOTURLS "")
  set(EXTENSION_DEPENDS "NA") # Specified as a space separated list or 'NA' if any
  set(EXTENSION_BUILD_SUBDIRECTORY .)
  unsetForSlicer( NAMES QT_QMAKE_EXECUTABLE SlicerExecutionModel_DIR ITK_DIR VTK_DIR CMAKE_C_COMPILER CMAKE_CXX_COMPILER CMAKE_CXX_FLAGS CMAKE_C_FLAGS ITK_LIBRARIES )
  find_package(Slicer REQUIRED)
  include(${Slicer_USE_FILE})
  resetForSlicer( NAMES CMAKE_C_COMPILER CMAKE_CXX_COMPILER CMAKE_CXX_FLAGS CMAKE_C_FLAGS )

  set(INSTALL_RUNTIME_DESTINATION ${Slicer_INSTALL_CLIMODULES_BIN_DIR})
  set(INSTALL_LIBRARY_DESTINATION ${Slicer_INSTALL_CLIMODULES_LIB_DIR})
  set(INSTALL_ARCHIVE_DESTINATION ${Slicer_INSTALL_CLIMODULES_LIB_DIR})

  set( ${PRIMARY_PROJECT_NAME}_DEPENDENCIES ITKv4 SlicerExecutionModel QtToCppXML Trafic)

  set(USE_SYSTEM_niral_utilities ON)
  
  add_definitions(-DSlicer_CLIMODULES_BIN_DIR="${Slicer_CLIMODULES_BIN_DIR}")
else()
  set(INSTALL_RUNTIME_DESTINATION bin)
  set(INSTALL_LIBRARY_DESTINATION lib)
  set(INSTALL_ARCHIVE_DESTINATION lib)

  set(${PRIMARY_PROJECT_NAME}_DEPENDENCIES ITKv4 SlicerExecutionModel QtToCppXML VTK DTIProcess DTIAtlasFiberAnalyzer DTI-Reg ResampleDTIlogEuclidean Trafic)

endif()