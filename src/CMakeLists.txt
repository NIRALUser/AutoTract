
message(STATUS "----------------reading src/CMakeLists.txt-----------------------")


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
Classification.cxx
)
set(CMAKE_VERBOSE_MAKEFILE ON)

IF(Qt4_SUPPORT)
  find_package(Qt4 COMPONENTS QtCore REQUIRED)
  include(${QT_USE_FILE})

  QT4_ADD_RESOURCES(RES_SOURCES resources.qrc)
  QT4_WRAP_CPP(QtProject_HEADERS_MOC AutoTractWindow.h AutoTractDerivedWindow.h MainScriptThread.h)
  QT4_WRAP_UI(UI_FILES AutoTract.ui)
ELSE()
  find_package(Qt5 REQUIRED Core Widgets)
  include_directories(${Qt5Widgets_INCLUDES})

  QT5_ADD_RESOURCES(RES_SOURCES resources.qrc)
  QT5_WRAP_CPP(QtProject_HEADERS_MOC AutoTractWindow.h AutoTractDerivedWindow.h MainScriptThread.h)
  QT5_WRAP_UI(UI_FILES AutoTract.ui)

  set(QT_INCLUDE_DIR ${Qt5Widgets_INCLUDE_DIRS})
  set(QT_LIBRARY_DIRS ${Qt5Widgets_LIBRARY_DIRS})
  set(QT_LIBRARIES ${Qt5Widgets_LIBRARIES})
ENDIF()




list( APPEND
srcfiles
${QtProject_HEADERS_MOC}
${UI_FILES}
)
QtToCppXML( SOURCES srcfiles NAME AutoTract UIXML_FILE AutoTract.ui PREFIXES soft_ para_ test)


SEMMacroBuildCLI(
	NAME AutoTract
	INCLUDE_DIRECTORIES ${QT_INCLUDE_DIR} ${ITK_INCLUDE_DIR} ${QtToCppXML_INCLUDE_DIR}
	LINK_DIRECTORIES ${QT_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS} ${QtToCppXML_LIBRARY_DIRS}
	TARGET_LIBRARIES ${QT_LIBRARIES} ${ITK_LIBRARIES} ${QtToCppXML_LIBRARIES} 
	ADDITIONAL_SRCS ${srcfiles} ${RES_SOURCES}
	RUNTIME_OUTPUT_DIRECTORY ${ARCHIVE_DESTINATION}
	LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_DESTINATION}
	ARCHIVE_OUTPUT_DIRECTORY ${RUNTIME_DESTINATION}
	INSTALL_RUNTIME_DESTINATION ${INSTALL_ARCHIVE_DESTINATION}
	INSTALL_LIBRARY_DESTINATION ${INSTALL_LIBRARY_DESTINATION}
	INSTALL_ARCHIVE_DESTINATION ${INSTALL_RUNTIME_DESTINATION}
)

