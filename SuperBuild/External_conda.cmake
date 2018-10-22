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
set(extProjName conda) #The find_package known name
set(proj      conda) #This local name
set(${extProjName}_REQUIRED_VERSION "")  #If a required version is necessary, then set this, else leave blank

if(UNIX)
	if(MACOSX)
		set(${extProjName}_SCRIPT Miniconda3-latest-MacOSX-x86_64.sh)
		set(${extProjName}_URL https://repo.continuum.io/miniconda/${${extProjName}_SCRIPT})
		set(${extProjName}_URL_HASH 7f7613bf98023f7d6ffe5df53c3a00a0)
	else()
		set(${extProjName}_SCRIPT Miniconda3-latest-Linux-x86_64.sh)
		set(${extProjName}_URL https://repo.continuum.io/miniconda/${${extProjName}_SCRIPT})
		set(${extProjName}_URL_HASH e1045ee415162f944b6aebfe560b8fee)
	endif()

	ExternalProject_Add(conda
	    URL ${${extProjName}_URL}
	    URL_HASH MD5=${${extProjName}_URL_HASH}
	    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/conda
	    DOWNLOAD_NAME ${${extProjName}_SCRIPT}
	    DOWNLOAD_NO_EXTRACT TRUE
	    CONFIGURE_COMMAND ""
	    BUILD_COMMAND ""
	    INSTALL_COMMAND /bin/sh ${CMAKE_BINARY_DIR}/conda/${${extProjName}_SCRIPT} -b -u -p ${CMAKE_BINARY_DIR}/conda-install
    ) 

    set(${extProjName}_DIR ${CMAKE_BINARY_DIR}/conda-install)
	
endif()



