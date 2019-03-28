CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
CMAKE_POLICY(VERSION 2.6)
PROJECT(AutoTract)

SETIFEMPTY( ARCHIVE_DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib/static )
SETIFEMPTY( LIBRARY_DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib )
SETIFEMPTY( RUNTIME_DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin )

SETIFEMPTY(INSTALL_RUNTIME_DESTINATION bin)
SETIFEMPTY(INSTALL_LIBRARY_DESTINATION lib)
SETIFEMPTY(INSTALL_ARCHIVE_DESTINATION lib)

#-----------------------------------------------------------------------------
# Update CMake module path
#------------------------------------------------------------------------------


set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${${PROJECT_NAME}_SOURCE_DIR}/CMake
  ${${PROJECT_NAME}_BINARY_DIR}/CMake
  )

IF(Qt4_SUPPORT)
  add_definitions(-DQT_4_SUPPORT=1)
  find_package(Qt4 COMPONENTS QtCore QtGui QtXml REQUIRED)
  include(${QT_USE_FILE})

ELSE()
  find_package(Qt5 REQUIRED Core Widgets Xml)
  include_directories(${Qt5Widgets_INCLUDES})
  include_directories(${Qt5Xml_INCLUDES})
  add_definitions(${Qt5Widgets_DEFINITIONS})
ENDIF()


#Find SlicerExecutionModel
FIND_PACKAGE(SlicerExecutionModel REQUIRED)
INCLUDE(${SlicerExecutionModel_USE_FILE})

#INCLUDE(${GenerateCLP_USE_FILE})

find_package(QtToCppXML REQUIRED)
include(${QtToCppXML_USE_FILE}) 


find_package(Trafic)

if(Trafic_FOUND)
  
  foreach(Trafic_lib ${Trafic_LIBRARIES})

    get_target_property(Trafic_location ${Trafic_lib} LOCATION_RELEASE)
    if(NOT EXISTS ${Trafic_location})
      message(STATUS "skipping niral_utilities_lib install rule: [${Trafic_location}] does not exist")
      continue()
    endif()
    
    if(EXISTS "${Trafic_location}.xml")
      install(PROGRAMS ${Trafic_location} 
        DESTINATION ${INSTALL_RUNTIME_DESTINATION}
        COMPONENT RUNTIME)

      install(FILES ${Trafic_location}.xml
        DESTINATION ${INSTALL_RUNTIME_DESTINATION}
        COMPONENT RUNTIME)
    else()
      if (${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION)
        install(PROGRAMS ${Trafic_location} 
          DESTINATION ${INSTALL_RUNTIME_DESTINATION}/../ExternalBin
          COMPONENT RUNTIME)      
      else()
        install(PROGRAMS ${Teem_EXECUTABLE_DIRS} 
          DESTINATION ${INSTALL_RUNTIME_DESTINATION}
          COMPONENT RUNTIME)
      endif()
    endif()
  endforeach()

  if(Trafic_PY_DIR)
    install(DIRECTORY ${Trafic_PY_DIR}
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}/Trafic
      COMPONENT RUNTIME)
  endif()

  if(conda_DIR)
    install(DIRECTORY ${conda_DIR}
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}
      USE_SOURCE_PERMISSIONS
      COMPONENT RUNTIME)
  endif()
  
endif()


if(Teem_DIR)
  if (${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION)
    install(PROGRAMS ${Teem_DIR}/../bin/unu 
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}/../ExternalBin
      COMPONENT RUNTIME)
  else()
    install(PROGRAMS ${Teem_DIR}/../bin/unu 
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}
      COMPONENT RUNTIME)
  endif()
endif()

if(NOT ${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION)

  if(ANTs_DIR)
    set(ants_tools
      ANTS
      WarpImageMultiTransform)

    foreach(ants_bin ${ants_tools})
      
        install(PROGRAMS ${ANTs_DIR}/bin/${ants_bin}
          DESTINATION ${INSTALL_RUNTIME_DESTINATION}
          COMPONENT RUNTIME)

    endforeach()
  endif()

  
  find_package(DTIProcess)
  if(DTIProcess_FOUND)

    install(PROGRAMS ${DTIProcess_dtiprocess_EXECUTABLE} 
        DESTINATION ${INSTALL_RUNTIME_DESTINATION}
        COMPONENT RUNTIME)
    
    if(EXISTS "${DTIProcess_dtiprocess_EXECUTABLE}.xml")
      install(FILES ${DTIProcess_dtiprocess_EXECUTABLE}.xml
        DESTINATION ${INSTALL_RUNTIME_DESTINATION}
        COMPONENT RUNTIME)
    endif()

    install(PROGRAMS ${DTIProcess_fiberprocess_EXECUTABLE} 
        DESTINATION ${INSTALL_RUNTIME_DESTINATION}
        COMPONENT RUNTIME)
    
    if(EXISTS "${DTIProcess_fiberprocess_EXECUTABLE}.xml")
      install(FILES ${DTIProcess_fiberprocess_EXECUTABLE}.xml
        DESTINATION ${INSTALL_RUNTIME_DESTINATION}
        COMPONENT RUNTIME)
    endif()
    
    
  endif()

  
  find_package(DTI-Reg)
  if(DTI-Reg_FOUND)

    foreach(dtir_lib ${DTI-Reg_LIBRARIES})

      get_target_property(dtir_location ${dtir_lib} LOCATION_RELEASE)
      if(NOT EXISTS ${dtir_location})
        message(STATUS "skipping niral_utilities_lib install rule: [${dtir_location}] does not exist")
        continue()
      endif()

      install(PROGRAMS ${dtir_location} 
          DESTINATION ${INSTALL_RUNTIME_DESTINATION}
          COMPONENT RUNTIME)
      
      if(EXISTS "${dtir_location}.xml")
        install(FILES ${dtir_location}.xml
          DESTINATION ${INSTALL_RUNTIME_DESTINATION}
          COMPONENT RUNTIME)
      endif()
    endforeach()
    
  endif()

  find_package(niral_utilities)
  if(niral_utilities_FOUND)

    foreach(niral_utilities_lib ${niral_utilities_LIBRARIES})

      get_target_property(niral_utilities_location ${niral_utilities_lib} LOCATION_RELEASE)
      if(NOT EXISTS ${niral_utilities_location})
        message(STATUS "skipping niral_utilities_lib install rule: [${niral_utilities_location}] does not exist")
        continue()
      endif()
      
      if(EXISTS "${niral_utilities_location}.xml")
        install(PROGRAMS ${niral_utilities_location} 
          DESTINATION ${INSTALL_RUNTIME_DESTINATION}
          COMPONENT RUNTIME)

        install(FILES ${niral_utilities_location}.xml
          DESTINATION ${INSTALL_RUNTIME_DESTINATION}
          COMPONENT RUNTIME)
      else()
        install(PROGRAMS ${niral_utilities_location} 
          DESTINATION ${INSTALL_RUNTIME_DESTINATION}
          COMPONENT RUNTIME)      
      endif()
    endforeach()
    
  endif()


  if(DTIAtlasFiberAnalyzer_DIR)
    install(PROGRAMS ${DTIAtlasFiberAnalyzer_DIR}/bin/FiberPostProcess
          DESTINATION ${INSTALL_RUNTIME_DESTINATION}
          COMPONENT RUNTIME)

    install(FILES ${DTIAtlasFiberAnalyzer_DIR}/bin/FiberPostProcess.xml
          DESTINATION ${INSTALL_RUNTIME_DESTINATION}
          COMPONENT RUNTIME)
  endif()

  find_package(ITKTransformTools)

  if(ITKTransformTools_DIR)
    get_target_property(ITKTransformTools_location ITKTransformTools LOCATION_RELEASE)

    install(PROGRAMS ${ITKTransformTools_location} 
            DESTINATION ${INSTALL_RUNTIME_DESTINATION}
            COMPONENT RUNTIME)  
  endif()

  if(ResampleDTIlogEuclidean_DIR)
    install(PROGRAMS ${ResampleDTIlogEuclidean_DIR}/bin/ResampleDTIlogEuclidean 
            DESTINATION ${INSTALL_RUNTIME_DESTINATION}
            COMPONENT RUNTIME)  

    install(PROGRAMS ${ResampleDTIlogEuclidean_DIR}/bin/ResampleDTIlogEuclidean.xml
            DESTINATION ${INSTALL_RUNTIME_DESTINATION}
            COMPONENT RUNTIME) 
  endif()

endif()


set(ITK_IO_MODULES_USED
ITKIOImageBase
ITKIONRRD
ITKIOCSV
ITKIOGIPL
ITKIOHDF5
ITKIOIPL
ITKIOImageBase
ITKIOLSM
ITKIOMRC
ITKIOMesh
ITKIOMeta
ITKIONIFTI
ITKIONRRD
ITKIORAW
ITKIOVTK
)
#Find ITK
find_package(ITK 4.7 REQUIRED COMPONENTS
  ITKCommon
  ITKIOImageBase
  ITKImageFunction
  ITKVTK
  ${ITK_IO_MODULES_USED}
)

include(${ITK_USE_FILE})


option(BUILD_TESTING "Build the testing tree" ON)

if(UNIX)

  SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
  SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  SET(CMAKE_INSTALL_RPATH "../lib")
  SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  
  get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
  get_target_property(QtGui_location Qt5::Gui LOCATION)
  get_target_property(QtCore_location Qt5::Core LOCATION)

  install(FILES ${QtWidgets_location} ${QtGui_location} ${QtCore_location}
      DESTINATION lib
      COMPONENT RUNTIME)

endif()

ADD_SUBDIRECTORY(src)

IF(BUILD_TESTING)
  include(CTest)
  ADD_SUBDIRECTORY(Testing)
ENDIF(BUILD_TESTING)

if( ${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION )
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${EXTENSION_NAME};ALL;/")
  include(${Slicer_EXTENSION_CPACK})
endif()

