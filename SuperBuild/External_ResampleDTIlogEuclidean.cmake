if( NOT EXTERNAL_SOURCE_DIRECTORY )
  set( EXTERNAL_SOURCE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/ExternalSources )
endif()
if( NOT EXTERNAL_BINARY_DIRECTORY )
  set( EXTERNAL_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
endif()

# Make sure this file is included only once
get_filename_component(CMAKE_CURRENT_LIST_FILENAME ${CMAKE_CURRENT_LIST_FILE} NAME_WE)
if(${CMAKE_CURRENT_LIST_FILENAME}_FILE_INCLUDED)
  return()
endif()
set(${CMAKE_CURRENT_LIST_FILENAME}_FILE_INCLUDED 1)

# Include dependent projects if any
set(extProjName ResampleDTIlogEuclidean) #The find_package known name
set(proj ${extProjName})              #This local name

#if(${USE_SYSTEM_${extProjName}})
#  unset(${extProjName}_DIR CACHE)
#endif()

# Sanity checks
if(DEFINED ${extProjName}_DIR AND NOT EXISTS ${${extProjName}_DIR})
  message(FATAL_ERROR "${extProjName}_DIR variable is defined but corresponds to non-existing directory (${${extProjName}_DIR})")
endif()

# Set dependency list
set(${proj}_DEPENDENCIES ITKv4)

SlicerMacroCheckExternalProjectDependency(${proj})

if(NOT ( DEFINED "USE_SYSTEM_${extProjName}" AND "${USE_SYSTEM_${extProjName}}" ) )

  ### --- Project specific additions here
  set(${proj}_CMAKE_OPTIONS
      -DITK_DIR:PATH=${ITK_DIR}
      -DUSE_SYSTEM_ITK:BOOL=ON
      -DSlicerExecutionModel_DIR:PATH=${SlicerExecutionModel_DIR}
      -DUSE_SYSTEM_SlicerExecutionModel:BOOL=ON
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/${proj}-install
    )
  ### --- End Project specific additions
  set(${proj}_REPOSITORY "${git_protocol}://github.com/NIRALUser/ResampleDTIlogEuclidean.git")
  set(${proj}_GIT_TAG release)
  ExternalProject_Add(${proj}
    GIT_REPOSITORY ${${proj}_REPOSITORY}
    GIT_TAG ${${proj}_GIT_TAG}
    SOURCE_DIR ${EXTERNAL_SOURCE_DIRECTORY}/${proj}
    BINARY_DIR ${EXTERNAL_BINARY_DIRECTORY}/${proj}-build
    LOG_CONFIGURE 0  # Wrap configure in script to ignore log output from dashboards
    LOG_BUILD     0  # Wrap build in script to to ignore log output from dashboards
    LOG_TEST      0  # Wrap test in script to to ignore log output from dashboards
    LOG_INSTALL   0  # Wrap install in script to to ignore log output from dashboards
    ${cmakeversion_external_update} "${cmakeversion_external_update_value}"
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS
      -DBUILD_EXAMPLES:BOOL=OFF
      -DBUILD_TESTING:BOOL=OFF
      ${${proj}_CMAKE_OPTIONS}
    DEPENDS
      ${${proj}_DEPENDENCIES}
    )
  set(${extProjName}_DIR ${EXTERNAL_BINARY_DIRECTORY}/${proj}-install)

else()
  set(${extProjName}_DIR "" CACHE FILEPATH "Path to install directory of ResampleDTIlogEuclidean")
  SlicerMacroEmptyExternalProject(${proj} "${${proj}_DEPENDENCIES}")
endif()

list(APPEND ${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_VARS ${extProjName}_DIR:PATH)