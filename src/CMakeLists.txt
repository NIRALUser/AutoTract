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

list( APPEND
srcfiles
${QtProject_HEADERS_MOC}
${UI_FILES}
)
QtToCppXML( SOURCES srcfiles NAME AutoTract UIXML_FILE AutoTract.ui PREFIXES soft_ para_)

SEMMacroBuildCLI(
	NAME AutoTract
	EXECUTABLE_ONLY
	ADDITIONAL_SRCS ${srcfiles}
	TARGET_LIBRARIES ${QT_LIBRARIES} ${ITK_LIBRARIES}
	LINK_DIRECTORIES ${QT_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS}
	INCLUDE_DIRECTORIES ${QT_INCLUDE_DIR} ${ITK_INCLUDE_DIR}
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
	INSTALL_RUNTIME_DESTINATION bin
	INSTALL_LIBRARY_DESTINATION lib
	INSTALL_ARCHIVE_DESTINATION lib
  )