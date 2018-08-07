CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
CMAKE_POLICY(VERSION 2.6)
PROJECT(AutoTract)

#-----------------------------------------------------------------------------
# SLICER EXTENSION
#-----------------------------------------------------------------------------
if( ${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION )
  set(EXTENSION_NAME ${LOCAL_PROJECT_NAME} )
  set(MODULE_NAME ${LOCAL_PROJECT_NAME} )
  set(EXTENSION_HOMEPAGE "https://github.com/NIRALUser/AutoTract")
  set(EXTENSION_CATEGORY "Tractography")
  set(EXTENSION_CONTRIBUTORS "Jean-Yves Yang, Francois Budin, Juan Carlos prieto (UNC), Adrien Boucaud (UNC), Mateo Lopez (UNC)")
  set(EXTENSION_DESCRIPTION "An automatic tractography tool featuring advanced processing tools to clean fiber tracts after the initial tractography. ")
  set(EXTENSION_ICONURL "https://www.nitrc.org/project/screenshot.php?group_id=948&screenshot_id=935")
  set(EXTENSION_SCREENSHOTURLS "")
  set(EXTENSION_DEPENDS "NA") # Specified as a space separated list or 'NA' if any
  set(EXTENSION_BUILD_SUBDIRECTORY .)
  unsetForSlicer( NAMES QT_QMAKE_EXECUTABLE SlicerExecutionModel_DIR ITK_DIR VTK_DIR CMAKE_C_COMPILER CMAKE_CXX_COMPILER CMAKE_CXX_FLAGS CMAKE_C_FLAGS ITK_LIBRARIES )
  find_package(Slicer REQUIRED)
  include(${Slicer_USE_FILE})
  resetForSlicer( NAMES CMAKE_C_COMPILER CMAKE_CXX_COMPILER CMAKE_CXX_FLAGS CMAKE_C_FLAGS )
endif()

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
  ${${PROJECT_NAME}_SOURCE_DIR}/CMake
  ${${PROJECT_NAME}_BINARY_DIR}/CMake
  ${CMAKE_MODULE_PATH}
  )



IF(Qt4_SUPPORT)
  add_definitions(-DQT_4_SUPPORT=1)
  find_package(Qt4 COMPONENTS QtCore QtGui QtXml REQUIRED)
  include(${QT_USE_FILE})

ELSE()
  find_package(Qt5 REQUIRED Core Widgets Xml)
  include_directories(${Qt5Widgets_INCLUDES})
  include_directories(${Qt5Xml_INCLUDES})
ENDIF()


#Find SlicerExecutionModel
FIND_PACKAGE(SlicerExecutionModel REQUIRED)
INCLUDE(${SlicerExecutionModel_USE_FILE})
#INCLUDE(${GenerateCLP_USE_FILE})

find_package(QtToCppXML REQUIRED)
include(${QtToCppXML_USE_FILE}) 



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

ADD_SUBDIRECTORY(src)

IF(BUILD_TESTING)
  include(CTest)
  ADD_SUBDIRECTORY(Testing)
ENDIF(BUILD_TESTING)


if( ${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION )
  message(STATUS "--------------------------------SETTING CPACK_INSTALL_CMAKE_PROJECTS----------------------------------")
  configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ImportAutoTract.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/ImportAutoTract.cmake)
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${EXTENSION_NAME};ALL;/")
  include(${Slicer_EXTENSION_CPACK})
endif()