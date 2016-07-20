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
#  Description: Main CMake script for the TerraLib Layout build system.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#          Juan Carlos P. Garrido <juan@dpi.inpe.br>
#          Frederico Augusto T. Bede <frederico.bede@funcate.org.br>
#

cmake_minimum_required(VERSION 2.8.7)

project(terralib_layout)

#
# main variables for the build script
#

# variables that control the system versioning
set(TERRALIB_LAYOUT_VERSION_MAJOR 0)
set(TERRALIB_LAYOUT_VERSION_MINOR 0)
set(TERRALIB_LAYOUT_VERSION_PATCH 4)
set(TERRALIB_LAYOUT_VERSION_STRING "${TERRALIB_LAYOUT_VERSION_MAJOR}.${TERRALIB_LAYOUT_VERSION_MINOR}.${TERRALIB_LAYOUT_VERSION_PATCH}")
set(TERRALIB_LAYOUT_VERSION "0x000004")
set(TERRALIB_LAYOUT_PKG_VERSION ${TERRALIB_LAYOUT_VERSION_MAJOR}.${TERRALIB_LAYOUT_VERSION_MINOR}.${TERRALIB_LAYOUT_VERSION_PATCH})

# variable indicating that it will search for 3rd-party libraries and install them
if(NOT DEFINED TERRALIB_TRACK_3RDPARTY_DEPENDENCIES)
  set(TERRALIB_TRACK_3RDPARTY_DEPENDENCIES 0 CACHE BOOL "If \"on\" it will search for 3rd-party libraries and install them when building the install target (e.g.: make install)")
endif()

# variables storing absolute paths
set(TERRALIB_ABSOLUTE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# variables that control paths
if(NOT DEFINED TERRALIB_DIR_VAR_NAME)
  set(TERRALIB_DIR_VAR_NAME "TERRALIB_HOME" CACHE STRING "Name of an environment variable with the base installation path of TerraLib")
endif()

# variables that control the install location
if(NOT DEFINED TERRALIB_DESTINATION_PLUGINS)
  set(TERRALIB_DESTINATION_PLUGINS "${TERRALIB_BASE_DESTINATION_DIR}share/terralib/plugins")
endif()

if(NOT DEFINED TERRALIB_DESTINATION_LIBRARY)
  set(TERRALIB_DESTINATION_LIBRARY "${TERRALIB_BASE_DESTINATION_DIR}lib")
endif()

if(NOT DEFINED TERRALIB_DESTINATION_RUNTIME)
  set(TERRALIB_DESTINATION_RUNTIME "${TERRALIB_BASE_DESTINATION_DIR}bin")
endif()

if(NOT DEFINED TERRALIB_DESTINATION_ARCHIVE)
  set(TERRALIB_DESTINATION_ARCHIVE "${TERRALIB_BASE_DESTINATION_DIR}lib")
endif()

if(NOT DEFINED TERRALIB_DESTINATION_HEADERS)
  set(TERRALIB_DESTINATION_HEADERS "${TERRALIB_BASE_DESTINATION_DIR}include")
endif()

if(NOT DEFINED TERRALIB_DESTINATION_SHARE)
  set(TERRALIB_DESTINATION_SHARE "${TERRALIB_BASE_DESTINATION_DIR}share")
endif()

if(NOT DEFINED TERRALIB_DESTINATION_UNITTEST)
  set(TERRALIB_DESTINATION_UNITTEST "${TERRALIB_BASE_DESTINATION_DIR}share/terralib/unittest")
endif()

# variable that set the bundle items as writable before install_name_tool tries to change them (for APPLE platforms)
if(APPLE AND TERRALIB_BUILD_AS_BUNDLE AND TERRALIB_TRACK_3RDPARTY_DEPENDENCIES AND NOT DEFINED BU_CHMOD_BUNDLE_ITEMS)
  set(BU_CHMOD_BUNDLE_ITEMS ON CACHE BOOL "If ON, set the bundle items as writable")
endif()


#
# global definitions and includes
#
if(MSVC)
  set(CMAKE_DEBUG_POSTFIX "d")
endif()

add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3 -DBOOST_UBLAS_TYPE_CHECK=0)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)

  if(MINGW)
    add_definitions(-DTE_MINGW -D_WIN32_WINNT=0x601 -fpermissive)
  endif()
elseif(APPLE)
  add_definitions(-ftemplate-depth-1024)
endif()

# Note: automatically link executables with qtmain.lib on Windows when it has Qt dependency.
if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER  2.8.9)
  cmake_policy(SET CMP0020 NEW)
endif()

if(${CMAKE_MAJOR_VERSION} GREATER 2)
  cmake_policy(SET CMP0026 OLD)
  cmake_policy(SET CMP0042 NEW)
  cmake_policy(SET CMP0043 NEW)
endif()

if(${CMAKE_VERSION} VERSION_GREATER "3.1.2")
  cmake_policy(SET CMP0053 OLD)
endif()


include_directories(${CMAKE_BINARY_DIR})


#
# Build will generate files into a single folder
#
if((CMAKE_GENERATOR STREQUAL Xcode) OR MSVC)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
else()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

#
# global configurations
#

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER MATCHES ".*clang"))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic ")
endif()


#
# including CMake's auxiliary files
#

include(CMakeDependentOption)


#
# including TerraLib auxiliary macros and functions
#

include(terralib_layout_macros.cmake)

#
# first check of libraries
#

find_package ( terralib 5.0.1 )

find_package(Boost 1.53.0 REQUIRED system date_time chrono timer filesystem thread unit_test_framework)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "TerraLib: could not find required Boost libraries!")
endif()

find_package(Qt5 5.1 QUIET COMPONENTS Core Gui Widgets PrintSupport)

if(NOT Qt5_FOUND)
  find_package(Qt4 4.6.3 QUIET COMPONENTS QtCore QtGui QtSvg)
else()
  find_package(Qt5LinguistTools QUIET)
endif()

if(Qt5_FOUND OR QT4_FOUND)
  find_package(Qwt)
  find_package(QtPropertyBrowser)
endif()

if(NOT QT_QCOLLECTIONGENERATOR_EXECUTABLE)
  find_program(QT_QCOLLECTIONGENERATOR_EXECUTABLE
               NAME qcollectiongenerator
               PATHS /usr
                     /usr/local
                     ${Qt5_DIR}/../../..
               PATH_SUFFIXES bin)
endif()

find_package(CppUnit)

#
# TerraLib including folders (Adicionado temporariamente todos os paths da terralib5, pois os arquivos do layout incluiam os paths relativos pois estava
#na mesma buildtree da TL5. Agora não é mais, aguardando força tarefa para mudar arquivos)
#


include_directories(${terralib_DIR}
          ${terralib_INCLUDE_DIRS}
          ${terralib_INCLUDE_DIRS}/terralib
          )


#
# some variables needed in config file
#

if(Boost_FOUND)
  set(TERRALIB_BOOST_ENABLED ON)
endif()

if(GNUGETTEXT_FOUND)

  set(TERRALIB_GNU_GETTEXT_ENABLED ON)

  if(NOT DEFINED TERRALIB_TRANSLATOR_ENABLED)
    set(TERRALIB_TRANSLATOR_ENABLED 1 CACHE BOOL "Enable translator support?")
  endif()

else()

  if(DEFINED TERRALIB_TRANSLATOR_ENABLED)
    set(TERRALIB_TRANSLATOR_ENABLED 0 CACHE BOOL "Enable translator support?" FORCE)
  endif()

endif()

if(GNUICONV_FOUND)

  set(TERRALIB_GNU_ICONV_ENABLED ON)

  if(NOT DEFINED TERRALIB_CHARENCODING_ENABLED)
    set(TERRALIB_CHARENCODING_ENABLED 1 CACHE BOOL "Enable character encoding support?")
  endif()

else()

  if(DEFINED TERRALIB_CHARENCODING_ENABLED)
    set(TERRALIB_CHARENCODING_ENABLED 0 CACHE BOOL "Enable character encoding support?" FORCE)
  endif()

endif()

if(LOG4CXX_FOUND)
  set(TERRALIB_APACHE_LOG4CXX_ENABLED ON)

  if(NOT DEFINED TERRALIB_LOGGER_ENABLED)
    set(TERRALIB_LOGGER_ENABLED 1 CACHE BOOL "Enable logger?")
  endif()

else()

  if(DEFINED TERRALIB_LOGGER_ENABLED)
    set(TERRALIB_LOGGER_ENABLED 0 CACHE BOOL "Enable logger?" FORCE)
  endif()

endif()

if(QT4_FOUND)
  set(TERRALIB_QT4_ENABLED ON)
  set(TERRALIB_QT_ENABLED ON)
endif()

if(Qt5_FOUND)
  set(TERRALIB_QT5_ENABLED ON)
  set(TERRALIB_QT_ENABLED ON)
endif()

if(QTPROPERTYBROWSER_FOUND)
  set(TERRALIB_QTPROPERTYBROWSER_ENABLED ON)
endif()

if(QWT_FOUND)
  set(TERRALIB_QWT_ENABLED ON)
endif()

#
# build options
#
# warning: please, do not change the order below!
#



#CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_LAYOUT_ENABLED "Build Layout module?" ON "TERRALIB_QT_ENABLED;TERRALIB_MOD_COLOR_ENABLED;TERRALIB_MOD_DATAACCESS_ENABLED;TERRALIB_MOD_MAPTOOLS_ENABLED;TERRALIB_MOD_MEMORY_ENABLED;TERRALIB_MOD_PLUGIN_ENABLED;TERRALIB_MOD_QT_WIDGETS_ENABLED;TERRALIB_MOD_SRS_ENABLED" OFF)

#CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_QT_PLUGINS_LAYOUT_ENABLED "Build Layout plugin?" ON "TERRALIB_QT_ENABLED;TERRALIB_MOD_LAYOUT_ENABLED;TERRALIB_MOD_QT_APF_ENABLED" OFF)

option(TERRALIB_MOD_LAYOUT_ENABLED "Build Layout module?" ON)

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_QT_PLUGINS_LAYOUT_ENABLED "Build Layout plugin?" ON "TERRALIB_MOD_LAYOUT_ENABLED" OFF)

#
# build options for the Layout Module Unit Test
#

CMAKE_DEPENDENT_OPTION(TERRALIB_MOD_LAYOUT_UNITTEST_ENABLED "Build the unit test for the layout module?" ON "TERRALIB_MOD_LAYOUT_ENABLED;TERRALIB_MOD_QT_PLUGINS_LAYOUT_ENABLED" OFF)

#
# process TERRALIB configuration files
#

configure_file(${CMAKE_SOURCE_DIR}/terralib_layout-config-pkg.cmake.in
               ${CMAKE_BINARY_DIR}/pkg/terralib_layout-config.cmake @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/terralib_layout-config-version.cmake.in
               ${CMAKE_BINARY_DIR}/terralib_layout-config-version.cmake @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/terralib_layout-config.cmake.in
               ${CMAKE_BINARY_DIR}/terralib_layout-config.cmake @ONLY)

configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/BuildConfig.h.in
               ${CMAKE_BINARY_DIR}/terralib_layout_buildconfig.h @ONLY)

configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/Version.h.in
               ${CMAKE_BINARY_DIR}/terralib_layout_version.h @ONLY)

configure_file(${TERRALIB_ABSOLUTE_ROOT_DIR}/COPYING ${CMAKE_BINARY_DIR}/COPYING.txt COPYONLY)

#
# build modules
#

# Turn on multi process compilation
if(MSVC)
  add_definitions(/MP)
endif()

if(TERRALIB_MOD_LAYOUT_ENABLED)
  add_subdirectory(terralib_mod_layout)
endif()

if(TERRALIB_MOD_QT_PLUGINS_LAYOUT_ENABLED)
  add_subdirectory(terralib_mod_qt_plugins_layout)
endif()

#
# build unit test
#

if(TERRALIB_MOD_LAYOUT_UNITTEST_ENABLED)
  add_subdirectory(terralib_mod_layout_unittest)
endif()

#
# install and targets export
#

file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/*.h)

install(FILES ${CMAKE_BINARY_DIR}/terralib_layout_buildconfig.h
              ${CMAKE_BINARY_DIR}/terralib_layout_version.h
              ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib COMPONENT devel)

install(FILES ${CMAKE_BINARY_DIR}/pkg/terralib_layout-config.cmake
              ${CMAKE_BINARY_DIR}/terralib_layout-config-version.cmake
        DESTINATION ${TERRALIB_DESTINATION_LIBRARY}/cmake/terralib_layout-${TERRALIB_LAYOUT_PKG_VERSION} COMPONENT devel)

install(EXPORT terralib-targets
        DESTINATION ${TERRALIB_DESTINATION_LIBRARY}/cmake/terralib_layout-${TERRALIB_LAYOUT_PKG_VERSION}
        FILE terralib_layout-exports.cmake
        COMPONENT devel)

#
# CPack
#

set(CPACK_SOURCE_INSTALLED_DIRECTORIES "${TERRALIB_ABSOLUTE_ROOT_DIR};/")

configure_file(${CMAKE_SOURCE_DIR}/terralib_layout-cpack-options.cmake.in
               ${CMAKE_BINARY_DIR}/terralib_layoutCPackOptions.cmake @ONLY)

set(CPACK_PROJECT_CONFIG_FILE ${terralib_BINARY_DIR}/terralib_layout-CPackOptions.cmake)

include(CPack)

# defining installation types, components and groups
cpack_add_install_type(full DISPLAY_NAME "Complete")
cpack_add_install_type(minimalistic DISPLAY_NAME "Minimalistic")

cpack_add_component_group(general_group
                          DISPLAY_NAME "Applications"
                          DESCRIPTION "TerraLib ..."
                          EXPANDED)

cpack_add_component_group(devel_group
                          DISPLAY_NAME "Development Tools"
                          DESCRIPTION "Development Tools: header files, libraries and cmake stuffs"
                          EXPANDED)

cpack_add_component(runtime
                    DISPLAY_NAME "Runtime Libraries and Executables"
                    DESCRIPTION "Install only necessary shared libraries (or DLLs) and executables"
                    GROUP general_group
                    INSTALL_TYPES full minimalistic)

cpack_add_component(devel
                    DISPLAY_NAME "Development package"
                    DESCRIPTION "Install header files, cmake stuffs and additional shared libraries"
                    GROUP devel_group
                    INSTALL_TYPES full minimalistic)

#
# Uninstall target
#

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)



add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
