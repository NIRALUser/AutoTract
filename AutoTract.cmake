CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
CMAKE_POLICY(VERSION 2.6)
PROJECT(AutoTract)

#-----------------------------------------------------------------------------
# Update CMake module path
#------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
  ${${PROJECT_NAME}_SOURCE_DIR}/CMake
  ${${PROJECT_NAME}_BINARY_DIR}/CMake
  ${CMAKE_MODULE_PATH}
  )


find_package(QtToCppXML REQUIRED)
include(${QtToCppXML_USE_FILE})



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


