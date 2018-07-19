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
find_package(Qt4 COMPONENTS QtCore REQUIRED)
include(${QT_USE_FILE})

QT4_ADD_RESOURCES(RES_SOURCES resources.qrc)
QT4_WRAP_CPP(QtProject_HEADERS_MOC AutoTractWindow.h AutoTractDerivedWindow.h MainScriptThread.h)
QT4_WRAP_UI(UI_FILES AutoTract.ui)

list( APPEND
srcfiles
${QtProject_HEADERS_MOC}
${UI_FILES}
)
QtToCppXML( SOURCES srcfiles NAME AutoTract UIXML_FILE AutoTract.ui PREFIXES soft_ para_ test)

SEMMacroBuildCLI(
	NAME AutoTract
	EXECUTABLE_ONLY
	INCLUDE_DIRECTORIES ${QT_INCLUDE_DIR} ${ITK_INCLUDE_DIR}
	LINK_DIRECTORIES ${QT_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS}
	TARGET_LIBRARIES ${QT_LIBRARIES} ${ITK_LIBRARIES} 
	ADDITIONAL_SRCS ${srcfiles} ${RES_SOURCES}
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
	INSTALL_RUNTIME_DESTINATION bin
	INSTALL_LIBRARY_DESTINATION lib
	INSTALL_ARCHIVE_DESTINATION lib
  )