#
#  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of the TerraLib - a Framework for building GIS enabled applications.
#
#  TerraLib is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  TerraLib is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with TerraLib. See COPYING. If not, write to
#  TerraLib Team at <terralib-team@terralib.org>.
#
#
#  Description: Auxiliary macros and functions.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

MACRO(TeInstallPlugins plugins location)

  set(_files)

  foreach(plugin ${plugins})
    get_target_property(_loc ${plugin} LOCATION)
    list(APPEND _files ${_loc})
  endforeach()
  
	install(FILES ${_files}
           DESTINATION "${TERRALIB_BASE_DESTINATION_DIR}qtplugins/${location}"
           CONFIGURATIONS Release
           COMPONENT runtime)
  
ENDMACRO(TeInstallPlugins)


MACRO(TeInstallQt5Plugins)
  find_package(Qt5 COMPONENTS Sql Svg PrintSupport)

# Installing image plugins
  set(_plugins Qt5::QGifPlugin Qt5::QICOPlugin Qt5::QJpegPlugin Qt5::QMngPlugin Qt5::QTiffPlugin)
  TeInstallPlugins("${_plugins}" "imageformats")
  
# Installing svg plugins
  set(_plugins Qt5::QSvgPlugin Qt5::QSvgIconPlugin)
  TeInstallPlugins("${_plugins}" "iconengines")
    
# Installing sql plugins
  set(_plugins Qt5::QSQLiteDriverPlugin)
  TeInstallPlugins("${_plugins}" "sqldrivers")

# Installing printer support plugin
  if(WIN32)
    set(_plugins Qt5::QWindowsPrinterSupportPlugin)
    TeInstallPlugins("${_plugins}" "printsupport")
  endif()
  
# Installing platform plugins
  if(WIN32)
    set(_plugins Qt5::QWindowsIntegrationPlugin Qt5::QMinimalIntegrationPlugin)
    TeInstallPlugins("${_plugins}" "platforms")
  elseif(APPLE)
    set(_plugins Qt5::QCocoaIntegrationPlugin Qt5::QMinimalIntegrationPlugin)
    TeInstallPlugins("${_plugins}" "platforms")
  endif()

#  if(APPLE)
#    install (TARGETS
#      Qt5::QWindowsIntegrationPlugin
#  	  RUNTIME
#	  DESTINATION qtplugins/platforms
#	  COMPONENT runtime
#    )
 # endif()
  
ENDMACRO(TeInstallQt5Plugins)

#
# Macro installQtPlugins
#
# Description: Installs the required Qt plugins.
#
# param plugs List of the names of plugins to be installed.
#
MACRO(TeInstallQtPlugins plgs)

  if ("${QT_PLUGINS_DIR}" STREQUAL "")
    set (QT_PLUGINS_DIR "${Qt5_DIR}/../../../plugins")
  endif()

  set (_regex_exp "")

  set( _first TRUE)

  foreach(plg ${plgs})
    if(NOT _first)
      set (_regex_exp ${_regex_exp}|${plg})
    else()
      set (_regex_exp ${plg})
      set (_first FALSE)
    endif()
  endforeach()

  set (_regex_exp "(${_regex_exp})?(${CMAKE_SHARED_LIBRARY_SUFFIX})$")

  set (_dest "${TERRALIB_BASE_DESTINATION_DIR}qtplugins")

if(QT4_FOUND)
  set (_plugin_dirs "imageformats;iconengines;sqldrivers")
else()
    set (_plugin_dirs "imageformats;iconengines;sqldrivers;platforms")
endif()

  foreach(_plugin_dir ${_plugin_dirs})

    install (DIRECTORY ${QT_PLUGINS_DIR}/${_plugin_dir}
             DESTINATION ${_dest}  COMPONENT runtime
             FILES_MATCHING REGEX "${_regex_exp}")
  endforeach()

ENDMACRO(TeInstallQtPlugins)

#Add library path to RPATH (This will make this project library to find automatically it's dependencies)
#Verify if the lib is already on LD or not
MACRO(addExternalLibraryToRPATH LIBRARY)
	IF(UNIX)
		get_filename_component(LIBRARY_PATH ${LIBRARY} DIRECTORY)
		LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES ${LIBRARY_PATH} isSystemDir)
		IF("${isSystemDir}" STREQUAL "-1")
	    	list (APPEND CMAKE_INSTALL_RPATH ${LIBRARY_PATH})
			MESSAGE(STATUS " -- Adding ${LIBRARY} to RPATH")
		ENDIF("${isSystemDir}" STREQUAL "-1")
	ENDIF()
ENDMACRO(addExternalLibraryToRPATH)

#Add a list of libraries to RPATH (This will make this project library to find automatically it's dependencies)
#Verify if the lib is already on LD or not
MACRO(addExternalLibrariesToRPATH LIBRARY_LIST)
  IF(UNIX)
	  FOREACH(LIBRARY ${LIBRARY_LIST})
		string(FIND ${LIBRARY} ".so" isShared)
		IF(NOT "${isShared}" EQUAL "-1")
			addExternalLibraryToRPATH(${LIBRARY})	
		ENDIF()
	  ENDFOREACH()
  ENDIF()
ENDMACRO(addExternalLibrariesToRPATH)

MACRO(GenerateTranslationQt5 translation_dir project_name qm_files)
  set(_ts_file_name ${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations/${project_name}_pt_br.ts)
  set(qm_file ${terralib_DIR}/share/terralib/translations/${project_name}_pt_br.qm)

  add_custom_command(
    TARGET ${project_name}
	POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E make_directory "${terralib_DIR}/share/terralib/translations/"
	  COMMAND ${Qt5_LUPDATE_EXECUTABLE} 
	  ARGS -I ${translation_dir} -recursive ${translation_dir} -ts "${_ts_file_name}"
	  COMMAND ${Qt5_LRELEASE_EXECUTABLE} 
	  ARGS "${_ts_file_name}" -qm "${qm_file}"
	  COMMENT "Generating ${project_name} translations"
  )
  message ("terralib_dir: ${terralib_DIR}")
  list(APPEND ${qm_files} ${qm_file})
ENDMACRO(GenerateTranslationQt5)

MACRO(GenerateTranslationQt4 translation_dir project_name qm_files)
  set(_ts_file_name ${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/translations/${project_name}_pt_br.ts)
  set(qm_file ${CMAKE_BINARY_DIR}/translations/${project_name}_pt_br.qm)

  add_custom_command(
    TARGET ${project_name}
	POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/translations"
	  COMMAND ${QT_LUPDATE_EXECUTABLE} 
	  ARGS -I ${translation_dir} -recursive ${translation_dir} -ts "${_ts_file_name}"
	  COMMAND ${QT_LRELEASE_EXECUTABLE} 
	  ARGS "${_ts_file_name}" -qm "${qm_file}"
	  COMMENT "Generating ${project_name} translations"
  )
  
  list(APPEND ${qm_files} ${qm_file})
ENDMACRO(GenerateTranslationQt4)