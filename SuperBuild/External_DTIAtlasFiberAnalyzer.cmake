if( NOT EXTERNAL_SOURCE_DIRECTORY )
  set( EXTERNAL_SOURCE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/ExternalSources )
endif()
if( NOT EXTERNAL_BINARY_DIRECTORY )
  set( EXTERNAL_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
endif()

# Make sure this file is included only once by creating globally unique varibles
# based on the name of this included file.
get_filename_component(CMAKE_CURRENT_LIST_FILENAME ${CMAKE_CURRENT_LIST_FILE} NAME_WE)
if(${CMAKE_CURRENT_LIST_FILENAME}_FILE_INCLUDED)
  return()
endif()
set(${CMAKE_CURRENT_LIST_FILENAME}_FILE_INCLUDED 1)

## External_${extProjName}.cmake files can be recurisvely included,
## and cmake variables are global, so when including sub projects it
## is important make the extProjName and proj variables
## appear to stay constant in one of these files.
## Store global variables before overwriting (then restore at end of this file.)
ProjectDependancyPush(CACHED_extProjName ${extProjName})
ProjectDependancyPush(CACHED_proj ${proj})

# Make sure that the ExtProjName/IntProjName variables are unique globally
# even if other External_${ExtProjName}.cmake files are sourced by
# SlicerMacroCheckExternalProjectDependency
set(extProjName DTIAtlasFiberAnalyzer) #The find_package known name
set(proj        DTIAtlasFiberAnalyzer) #This local name
set(${extProjName}_REQUIRED_VERSION "")  #If a required version is necessary, then set this, else leave blank

#if(${USE_SYSTEM_${extProjName}})
#  unset(${extProjName}_DIR CACHE)
#endif()

# Sanity checks
if(DEFINED ${extProjName}_DIR AND NOT EXISTS ${${extProjName}_DIR})
find_package(${extProjName} ${${extProjName}_REQUIRED_VERSION} REQUIRED)
  message(FATAL_ERROR "${extProjName}_DIR variable is defined but corresponds to non-existing directory (${${extProjName}_DIR})")
endif()

if(NOT ( DEFINED "USE_SYSTEM_${extProjName}" AND "${USE_SYSTEM_${extProjName}}" ) )
  #message(STATUS "${__indent}Adding project ${proj}")
  # Set dependency list
  set(${proj}_DEPENDENCIES ITKv4 VTK SlicerExecutionModel)

  # Include dependent projects if any
  SlicerMacroCheckExternalProjectDependency(${proj})
  # Set CMake OSX variable to pass down the external project
  set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
  if(APPLE)
    list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
      -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
      -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
      -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET})
  endif()
  
  ### --- Project specific additions here
  set(${proj}_CMAKE_OPTIONS
    -DBUILD_TESTING:BOOL=OFF
    -DUSE_SYSTEM_ITK:BOOL=ON
    -DUSE_SYSTEM_VTK:BOOL=ON
    -DUSE_SYSTEM_SlicerExecutionModel:BOOL=ON
    -DITK_DIR=${ITK_DIR}
    -DVTK_DIR=${VTK_DIR}
    -DQtToCppXML_DIR:PATH=${QtToCppXML_DIR}
    -DUSE_SYSTEM_QtToCppXML:BOOL=ON
    -D${extProjName}_SUPERBUILD:BOOL=OFF
    -DDTIAtlasFiberAnalyzer_SuperBuild:BOOL=OFF
    -DSlicerExecutionModel_DIR=${SlicerExecutionModel_DIR}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/${proj}-install
    -DUSE_QT4:BOOL=${Qt4_SUPPORT}
    -DCOMPILE_DTIATLASFIBERANALYZER:BOOL=OFF
    -DCOMPILE_DTITRACTSTAT:BOOL=OFF
    -DCOMPILE_MERGERSTATWITHFIBER:BOOL=OFF
    -DCOMPILE_FIBERCOMPARE:BOOL=OFF
    -DCOMPILE_SCALARFIBERANALYZER:BOOL=OFF
    )

  ### --- End Project specific additions
  set( ${proj}_REPOSITORY ${git_protocol}://github.com/NIRALUser/DTIFiberTractStatistics.git)
  set( ${proj}_GIT_TAG  release )
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
      ${CMAKE_OSX_EXTERNAL_PROJECT_ARGS}
      ${COMMON_EXTERNAL_PROJECT_ARGS}
      ${${proj}_CMAKE_OPTIONS}      
    DEPENDS
      ${${proj}_DEPENDENCIES}    
  )  
  set(${extProjName}_DIR ${EXTERNAL_BINARY_DIRECTORY}/${proj}-install/)
else()
  set(${extProjName}_DIR "" CACHE FILEPATH "Path to install directory of DTIAtlasFiberAnalyzer")
  SlicerMacroEmptyExternalProject(${proj} "${${proj}_DEPENDENCIES}")
endif()

list(APPEND ${CMAKE_PROJECT_NAME}_SUPERBUILD_EP_VARS ${extProjName}_DIR:PATH)

ProjectDependancyPop(CACHED_extProjName extProjName)
ProjectDependancyPop(CACHED_proj proj)
