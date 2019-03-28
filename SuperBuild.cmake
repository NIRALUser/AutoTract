
#-----------------------------------------------------------------------------
enable_language(C)
enable_language(CXX)

#-----------------------------------------------------------------------------
enable_testing()
include(CTest)
#-----------------------------------------------------------------------------

set(EXTERNAL_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} CACHE PATH "Select where external packages will be downloaded" )
set(EXTERNAL_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} CACHE PATH "Select where external packages will be compiled and installed" )
#-----------------------------------------------------------------------------
# CMake arguments to pass when build as an extension
#-----------------------------------------------------------------------------

if (${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION)
  #Arguments to pass when reprocessing the top CMakeLists.txt
  set(extension_args "")
  list(APPEND extension_args -DSlicer_DIR:PATH=${Slicer_DIR})
  list(APPEND extension_args -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
else()
  set(extension_args "")
endif()

#-----------------------------------------------------------------------------
# Git protocole option
#-----------------------------------------------------------------------------
option(USE_GIT_PROTOCOL "If behind a firewall turn this off to use http instead." ON)
set(git_protocol "git")
if(NOT USE_GIT_PROTOCOL)
  set(git_protocol "http")
endif()

find_package(Git REQUIRED)

#-----------------------------------------------------------------------------
# Enable and setup External project global properties
#-----------------------------------------------------------------------------
include(ExternalProject)
include(SlicerMacroEmptyExternalProject)
include(SlicerMacroCheckExternalProjectDependency)
include(SlicerMacroGetOperatingSystemArchitectureBitness)

# Compute -G arg for configuring external projects with the same CMake generator:
if(CMAKE_EXTRA_GENERATOR)
  set(gen "${CMAKE_EXTRA_GENERATOR} - ${CMAKE_GENERATOR}")
else()
  set(gen "${CMAKE_GENERATOR}")
endif()

#-----------------------------------------------------------------------------
# Superbuild option(s)
#-----------------------------------------------------------------------------


if (${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION)
  set(USE_SYSTEM_LIBS ON)
else()
  set(USE_SYSTEM_LIBS OFF)   
endif()



option(USE_SYSTEM_ITK "Build using an externally defined version of ITK" ${USE_SYSTEM_LIBS})
option(USE_SYSTEM_SlicerExecutionModel "Build using an externally defined version of SlicerExecutionModel"  ${USE_SYSTEM_LIBS})
option(USE_SYSTEM_QtToCppXML "Build using an externally defined version of QtToCppXMLeq" OFF)
option(USE_SYSTEM_VTK "Build using an externally defined version of VTK" ${USE_SYSTEM_LIBS})
option(USE_SYSTEM_Trafic "Build using an externally defined version of Trafic" OFF)
option(USE_SYSTEM_DTI-Reg "Build using an externally defined version of DTI-Reg" OFF)
option(USE_SYSTEM_ResampleDTIlogEuclidean "Build using an externally defined version of ResampleDTIlogEuclidean" OFF)
option(USE_SYSTEM_DTIAtlasFiberAnalyzer "Build using an externally defined version of DTIAtlasFiberAnalyzer" OFF)
option(USE_SYSTEM_DTIProcess "Build using an externally defined version of DTIProcess" OFF)

set(EXTERNAL_PROJECT_BUILD_TYPE "Release" CACHE STRING "Default build type for support libraries")

IF(Qt4_SUPPORT)
  set( ${PRIMARY_PROJECT_NAME}_DEPENDENCIES Qt4 ${${PRIMARY_PROJECT_NAME}_DEPENDENCIES})
ELSE()
  set( ${PRIMARY_PROJECT_NAME}_DEPENDENCIES Qt5 ${${PRIMARY_PROJECT_NAME}_DEPENDENCIES})
ENDIF()

#-----------------------------------------------------------------------------
# Define Superbuild global variables
#-----------------------------------------------------------------------------

# This variable will contain the list of CMake variable specific to each external project
# that should passed to ${CMAKE_PROJECT_NAME}.
# The item of this list should have the following form: <EP_VAR>:<TYPE>
# where '<EP_VAR>' is an external project variable and TYPE is either BOOL, STRING, PATH or FILEPATH.
# TODO Variable appended to this list will be automatically exported in ${PRIMARY_PROJECT_NAME}Config.cmake,
# prefix '${PRIMARY_PROJECT_NAME}_' will be prepended if it applies.
set(${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_VARS)

# The macro '_expand_external_project_vars' can be used to expand the list of <EP_VAR>.
set(${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_ARGS) # List of CMake args to configure ${PROJECT_NAME}
set(${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_VARNAMES) # List of CMake variable names

# Convenient macro allowing to expand the list of EP_VAR listed in ${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_VARS
# The expanded arguments will be appended to the list ${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_ARGS
# Similarly the name of the EP_VARs will be appended to the list ${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_VARNAMES.
macro(_expand_external_project_vars)
  set(${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_ARGS "")
  set(${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_VARNAMES "")
  foreach(arg ${${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_VARS})
    string(REPLACE ":" ";" varname_and_vartype ${arg})
    set(target_info_list ${target_info_list})
    list(GET varname_and_vartype 0 _varname)
    list(GET varname_and_vartype 1 _vartype)
    list(APPEND ${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_ARGS -D${_varname}:${_vartype}=${${_varname}})
    list(APPEND ${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_VARNAMES ${_varname})
  endforeach()
endmacro()


set(COMMON_EXTERNAL_PROJECT_ARGS ${${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_ARGS})
set(extProjName ${PRIMARY_PROJECT_NAME})
set(proj        ${PRIMARY_PROJECT_NAME})

List( LENGTH ${PRIMARY_PROJECT_NAME}_DEPENDENCIES dependencies_size )
if( dependencies_size GREATER 0 )
  SlicerMacroCheckExternalProjectDependency(${proj})
endif()

foreach(dependency ${${PRIMARY_PROJECT_NAME}_DEPENDENCIES})
  list(APPEND ${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_ARGS -D${dependency}_DIR:PATH=${${dependency}_DIR})
endforeach()
#------------------------------------------------------------------------------
# Configure and build ${PROJECT_NAME}
#------------------------------------------------------------------------------
set(proj ${PRIMARY_PROJECT_NAME})
ExternalProject_Add(${proj}
  DEPENDS ${${PRIMARY_PROJECT_NAME}_DEPENDENCIES}
  DOWNLOAD_COMMAND ""
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  BINARY_DIR ${PRIMARY_PROJECT_NAME}-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${extension_args}
    ${${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_ARGS}
    -D${PRIMARY_PROJECT_NAME}_SUPERBUILD:BOOL=OFF    #NOTE: VERY IMPORTANT reprocess top level CMakeList.txt
    -DQt4_SUPPORT:BOOL=${Qt4_SUPPORT}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/${PRIMARY_PROJECT_NAME}-install
    -Dniral_utilities_DIR:PATH=${niral_utilities_DIR}
  )

## Force rebuilding of the main subproject every time building from super structure
ExternalProject_Add_Step(${proj} forcebuild
    COMMAND ${CMAKE_COMMAND} -E remove
    ${CMAKE_CURRENT_BUILD_DIR}/${proj}-prefix/src/${proj}-stamp/${proj}-build
    DEPENDEES configure
    DEPENDERS build
    ALWAYS 1
  )

if( ${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION )
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR}/AutoTract-build;${EXTENSION_NAME};ALL;/")
  include(${Slicer_EXTENSION_CPACK})
endif()
