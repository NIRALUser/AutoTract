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

find_package(Qt4 COMPONENTS QtCore QtGui QtXml REQUIRED)
include(${QT_USE_FILE})

find_package(QtToCppXML REQUIRED)
include(${QtToCppXML_USE_FILE})

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


set(srcfiles
AutoTract.cxx 
AutoTractWindow.cxx
AutoTractDerivedWindow.cxx
Pipeline.cxx
MainScriptThread.cxx
Script.cxx
Registration.cxx
TractPopulationProcess.cxx
SingleTractProcess.cxx
MaskCreation.cxx
)

QT4_WRAP_CPP(QtProject_HEADERS_MOC AutoTractWindow.h AutoTractDerivedWindow.h MainScriptThread.h)
QT4_WRAP_UI(UI_FILES AutoTract.ui)

# Use the include path and library for Qt that is used by VTK.
#INCLUDE_DIRECTORIES(
#  ${CMAKE_SOURCE_DIR}
#  ${CMAKE_BINARY_DIR}
#  ${QT_INCLUDE_DIR}
#  ${AutoTract_BINARY_DIR}
#  ${AutoTract_SOURCE_DIR}
#)

list( APPEND
srcfiles
${QtProject_HEADERS_MOC}
${UI_FILES}
)
QtToCppXML( SOURCES srcfiles NAME AutoTract UIXML_FILE AutoTract.ui PREFIXES soft_ para_ )

SEMMacroBuildCLI(
  NAME AutoTract
  EXECUTABLE_ONLY
  ADDITIONAL_SRCS ${srcfiles}
  TARGET_LIBRARIES ${QT_LIBRARIES} ${ITK_LIBRARIES}
  LINK_DIRECTORIES ${QT_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS}
  INCLUDE_DIRECTORIES ${QT_INCLUDE_DIR} ${ITK_INCLUDE_DIR}
  )
#add_executable( AutoTract ${srcfiles} ${RESOURCES} AutoTract_CLP)
#target_link_libraries(AutoTract ${QT_LIBRARIES})

option(BUILD_TESTING "Build the testing tree" ON)

IF(BUILD_TESTING)
  include(CTest)
  ADD_SUBDIRECTORY(Testing)
ENDIF(BUILD_TESTING)
