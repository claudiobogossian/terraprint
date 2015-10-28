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

project(terralib)

#
# main variables for the build script
#

# variables that control the system versioning
set(TERRALIB_LAYOUT_VERSION_MAJOR 0)
set(TERRALIB_LAYOUT_VERSION_MINOR 0)
set(TERRALIB_LAYOUT_VERSION_PATCH 4)
set(TERRALIB_LAYOUT_VERSION_STRING "${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}.${TERRALIB_VERSION_PATCH}")
set(TERRALIB_LAYOUT_VERSION "0x000004")
set(TERRALIB_LAYOUT_LAYOUT_PKG_VERSION ${TERRALIB_VERSION_MAJOR}.${TERRALIB_VERSION_MINOR}.${TERRALIB_VERSION_PATCH})

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

if(NOT DEFINED TERRALIB_DESTINATION_ARCHIVE)
  set(TERRALIB_DESTINATION_ARCHIVE "${TERRALIB_BASE_DESTINATION_DIR}lib")
endif()

if(NOT DEFINED TERRALIB_DESTINATION_HEADERS)
  set(TERRALIB_DESTINATION_HEADERS "${TERRALIB_BASE_DESTINATION_DIR}include")
endif()

if(NOT DEFINED TERRALIB_DESTINATION_SHARE)
  set(TERRALIB_DESTINATION_SHARE "${TERRALIB_BASE_DESTINATION_DIR}share")
endif()

# variable that set the bundle items as writable before install_name_tool tries to change them (for APPLE platforms)
if(APPLE AND TERRALIB_BUILD_AS_BUNDLE AND TERRALIB_TRACK_3RDPARTY_DEPENDENCIES AND NOT DEFINED BU_CHMOD_BUNDLE_ITEMS)
  set(BU_CHMOD_BUNDLE_ITEMS ON CACHE BOOL "If ON, set the bundle items as writable")
endif()


#
# global definitions and includes
#

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

include(terralib-layout_macros.cmake)

#
# first check of libraries
#

find_package ( terralib 5.0.1 )

find_package(Boost 1.53.0 REQUIRED system date_time chrono timer filesystem thread)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "TerraLib: could not find required Boost libraries!")
endif()

find_package(Qt5 5.1 QUIET COMPONENTS Core Gui Widgets PrintSupport)

if(NOT Qt5_FOUND)
  find_package(Qt4 4.6.3 QUIET COMPONENTS QtCore QtGui)
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
					${terralib_INCLUDE_DIRS}/terralib/addressgeocoding
					${terralib_INCLUDE_DIRS}/terralib/addressgeocoding/qt
					${terralib_INCLUDE_DIRS}/terralib/addressgeocoding/qt/ui
					${terralib_INCLUDE_DIRS}/terralib/ado
					${terralib_INCLUDE_DIRS}/terralib/annotationtext
					${terralib_INCLUDE_DIRS}/terralib/antigrain
					${terralib_INCLUDE_DIRS}/terralib/attributefill
					${terralib_INCLUDE_DIRS}/terralib/attributefill/qt
					${terralib_INCLUDE_DIRS}/terralib/attributefill/qt/ui
					${terralib_INCLUDE_DIRS}/terralib/binding
					${terralib_INCLUDE_DIRS}/terralib/binding/swig
					${terralib_INCLUDE_DIRS}/terralib/binding/swig/common
					${terralib_INCLUDE_DIRS}/terralib/binding/swig/java
					${terralib_INCLUDE_DIRS}/terralib/binding/swig/lua
					${terralib_INCLUDE_DIRS}/terralib/binding/swig/python
					${terralib_INCLUDE_DIRS}/terralib/binding/swig/r
					${terralib_INCLUDE_DIRS}/terralib/binding/v8
					${terralib_INCLUDE_DIRS}/terralib/binding/v8/common
					${terralib_INCLUDE_DIRS}/terralib/binding/v8/jsi
					${terralib_INCLUDE_DIRS}/terralib/binding/v8/jsi/dataaccess
					${terralib_INCLUDE_DIRS}/terralib/binding/v8/jsi/datatype
					${terralib_INCLUDE_DIRS}/terralib/binding/v8/jsi/geometry
					${terralib_INCLUDE_DIRS}/terralib/binding/v8/jsi/platform
					${terralib_INCLUDE_DIRS}/terralib/binding/v8/plugin
					${terralib_INCLUDE_DIRS}/terralib/binding/vm
					${terralib_INCLUDE_DIRS}/terralib/cellspace
					${terralib_INCLUDE_DIRS}/terralib/classification
					${terralib_INCLUDE_DIRS}/terralib/color
					${terralib_INCLUDE_DIRS}/terralib/common
					${terralib_INCLUDE_DIRS}/terralib/common/progress
					${terralib_INCLUDE_DIRS}/terralib/common/URI
					${terralib_INCLUDE_DIRS}/terralib/datatype
					${terralib_INCLUDE_DIRS}/terralib/datatype/serialization
					${terralib_INCLUDE_DIRS}/terralib/datatype/serialization/xml
					${terralib_INCLUDE_DIRS}/terralib/edit
					${terralib_INCLUDE_DIRS}/terralib/edit/qt
					${terralib_INCLUDE_DIRS}/terralib/edit/qt/core
					${terralib_INCLUDE_DIRS}/terralib/edit/qt/core/command
					${terralib_INCLUDE_DIRS}/terralib/edit/qt/tools
					${terralib_INCLUDE_DIRS}/terralib/edit/qt/ui
					${terralib_INCLUDE_DIRS}/terralib/fe
					${terralib_INCLUDE_DIRS}/terralib/fe/serialization
					${terralib_INCLUDE_DIRS}/terralib/fe/serialization/xml
					${terralib_INCLUDE_DIRS}/terralib/gdal
					${terralib_INCLUDE_DIRS}/terralib/geometry
					${terralib_INCLUDE_DIRS}/terralib/geometry/serialization
					${terralib_INCLUDE_DIRS}/terralib/geometry/serialization/xml
					${terralib_INCLUDE_DIRS}/terralib/gml
					${terralib_INCLUDE_DIRS}/terralib/graph
					${terralib_INCLUDE_DIRS}/terralib/graph/builder
					${terralib_INCLUDE_DIRS}/terralib/graph/cache
					${terralib_INCLUDE_DIRS}/terralib/graph/core
					${terralib_INCLUDE_DIRS}/terralib/graph/drivers
					${terralib_INCLUDE_DIRS}/terralib/graph/drivers/datasource
					${terralib_INCLUDE_DIRS}/terralib/graph/functions
					${terralib_INCLUDE_DIRS}/terralib/graph/graphs
					${terralib_INCLUDE_DIRS}/terralib/graph/iterator
					${terralib_INCLUDE_DIRS}/terralib/graph/loader
					${terralib_INCLUDE_DIRS}/terralib/graph/maptools
					${terralib_INCLUDE_DIRS}/terralib/grib
					${terralib_INCLUDE_DIRS}/terralib/idl
					${terralib_INCLUDE_DIRS}/terralib/idl/rp
					${terralib_INCLUDE_DIRS}/terralib/maptools
					${terralib_INCLUDE_DIRS}/terralib/maptools/serialization
					${terralib_INCLUDE_DIRS}/terralib/maptools/serialization/xml
					${terralib_INCLUDE_DIRS}/terralib/memory
					${terralib_INCLUDE_DIRS}/terralib/metadata
					${terralib_INCLUDE_DIRS}/terralib/metadata/iso19115
					${terralib_INCLUDE_DIRS}/terralib/metadata/serialization
					${terralib_INCLUDE_DIRS}/terralib/metadata/serialization/xml
					${terralib_INCLUDE_DIRS}/terralib/mnt
					${terralib_INCLUDE_DIRS}/terralib/mnt/core
					${terralib_INCLUDE_DIRS}/terralib/mnt/plugin
					${terralib_INCLUDE_DIRS}/terralib/mnt/qt
					${terralib_INCLUDE_DIRS}/terralib/mnt/qt/ui
					${terralib_INCLUDE_DIRS}/terralib/ogr
					${terralib_INCLUDE_DIRS}/terralib/plugin
					${terralib_INCLUDE_DIRS}/terralib/qt
					${terralib_INCLUDE_DIRS}/terralib/qt/af
					${terralib_INCLUDE_DIRS}/terralib/qt/af/connectors
					${terralib_INCLUDE_DIRS}/terralib/qt/af/events
					${terralib_INCLUDE_DIRS}/terralib/qt/af/settings
					${terralib_INCLUDE_DIRS}/terralib/qt/af/settings/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/af/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/designer
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/addressgeocoding
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/attributefill
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/cellspace
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/cellspace/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/ado
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/ado/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/gdal
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/gdal/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/geofile
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/geofile/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/mysql
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/mysql/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/ogr
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/ogr/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/pgis
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/pgis/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/sqlite
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/sqlite/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/terralib4
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/terralib4/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/wcs
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/wcs/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/wfs
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/wfs/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/wms
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/datasource/wms/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/edit
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/mnt
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/mobile
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/mobile/core
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/mobile/core/form
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/mobile/qt
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/mobile/qt/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/mobile/utils
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/rp
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/sa
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/st
					${terralib_INCLUDE_DIRS}/terralib/qt/plugins/vp
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/canvas
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/canvas/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/charts
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/charts/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/classification
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/classification/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/colorbar
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/colorbar/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/dataset
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/dataset/explorer
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/dataset/selector
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/dataset/selector/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/datasource
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/datasource/connector
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/datasource/core
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/datasource/explorer
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/datasource/selector
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/datasource/selector/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/exchanger
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/exchanger/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/externalTable
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/externalTable/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/help
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/layer
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/layer/explorer
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/layer/info
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/layer/search
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/layer/search/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/layer/selector
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/layer/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/layer/utils
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/layer/utils/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/mapdisplay
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/plugin
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/plugin/builder
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/plugin/builder/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/plugin/manager
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/plugin/manager/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/progress
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/property
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/property/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/propertybrowser
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/query
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/query/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/raster
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/raster/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/rp
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/rp/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/se
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/se/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/srs
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/srs/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/st
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/st/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/table
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/table/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/tools
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/utils
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/utils/ui
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/vector
					${terralib_INCLUDE_DIRS}/terralib/qt/widgets/vector/ui
					${terralib_INCLUDE_DIRS}/terralib/raster
					${terralib_INCLUDE_DIRS}/terralib/raster/serialization
					${terralib_INCLUDE_DIRS}/terralib/raster/serialization/xml
					${terralib_INCLUDE_DIRS}/terralib/rp
					${terralib_INCLUDE_DIRS}/terralib/rp/radar
					${terralib_INCLUDE_DIRS}/terralib/sa
					${terralib_INCLUDE_DIRS}/terralib/sa/core
					${terralib_INCLUDE_DIRS}/terralib/sa/qt
					${terralib_INCLUDE_DIRS}/terralib/sa/qt/ui
					${terralib_INCLUDE_DIRS}/terralib/sam
					${terralib_INCLUDE_DIRS}/terralib/sam/kdtree
					${terralib_INCLUDE_DIRS}/terralib/sam/rtree
					${terralib_INCLUDE_DIRS}/terralib/se
					${terralib_INCLUDE_DIRS}/terralib/se/serialization
					${terralib_INCLUDE_DIRS}/terralib/se/serialization/xml
					${terralib_INCLUDE_DIRS}/terralib/sqlite
					${terralib_INCLUDE_DIRS}/terralib/srs
					${terralib_INCLUDE_DIRS}/terralib/st
					${terralib_INCLUDE_DIRS}/terralib/st/core
					${terralib_INCLUDE_DIRS}/terralib/st/core/coverage
					${terralib_INCLUDE_DIRS}/terralib/st/core/event
					${terralib_INCLUDE_DIRS}/terralib/st/core/interpolator
					${terralib_INCLUDE_DIRS}/terralib/st/core/observation
					${terralib_INCLUDE_DIRS}/terralib/st/core/timeseries
					${terralib_INCLUDE_DIRS}/terralib/st/core/trajectory
					${terralib_INCLUDE_DIRS}/terralib/st/loader
					${terralib_INCLUDE_DIRS}/terralib/st/maptools
					${terralib_INCLUDE_DIRS}/terralib/statistics
					${terralib_INCLUDE_DIRS}/terralib/statistics/core
					${terralib_INCLUDE_DIRS}/terralib/statistics/qt
					${terralib_INCLUDE_DIRS}/terralib/statistics/qt/ui
					${terralib_INCLUDE_DIRS}/terralib/stmemory
					${terralib_INCLUDE_DIRS}/terralib/terralib4
					${terralib_INCLUDE_DIRS}/terralib/tools
					${terralib_INCLUDE_DIRS}/terralib/tools/dscopy
					${terralib_INCLUDE_DIRS}/terralib/tools/dscopy/console
					${terralib_INCLUDE_DIRS}/terralib/tools/dscopy/core
					${terralib_INCLUDE_DIRS}/terralib/tools/dscopy/qt
					${terralib_INCLUDE_DIRS}/terralib/tools/dsinfo
					${terralib_INCLUDE_DIRS}/terralib/tools/dsinfo/console
					${terralib_INCLUDE_DIRS}/terralib/tools/dsinfo/core
					${terralib_INCLUDE_DIRS}/terralib/tools/rastermanager
					${terralib_INCLUDE_DIRS}/terralib/tools/rastermanager/console
					${terralib_INCLUDE_DIRS}/terralib/tools/rastermanager/core
					${terralib_INCLUDE_DIRS}/terralib/tools/rastermanager/qt
					${terralib_INCLUDE_DIRS}/terralib/vp
					${terralib_INCLUDE_DIRS}/terralib/vp/qt
					${terralib_INCLUDE_DIRS}/terralib/vp/qt/ui
					${terralib_INCLUDE_DIRS}/terralib/wcs
					${terralib_INCLUDE_DIRS}/terralib/wfs
					${terralib_INCLUDE_DIRS}/terralib/wms
					${terralib_INCLUDE_DIRS}/terralib/wms/qt
					${terralib_INCLUDE_DIRS}/terralib/wms/serialization
					${terralib_INCLUDE_DIRS}/terralib/wms/serialization/xml
					${terralib_INCLUDE_DIRS}/terralib/xerces
					${terralib_INCLUDE_DIRS}/terralib/xlink
					${terralib_INCLUDE_DIRS}/terralib/xlink/serialization
					${terralib_INCLUDE_DIRS}/terralib/xlink/serialization/xml
					${terralib_INCLUDE_DIRS}/terralib/xml
					${terralib_INCLUDE_DIRS}/terralib/xsd
					${terralib_INCLUDE_DIRS}/terralib/xsd/serialization
					${terralib_INCLUDE_DIRS}/terralib/xsd/serialization/xml
					${terralib_INCLUDE_DIRS}/terralib/dataaccess
					${terralib_INCLUDE_DIRS}/terralib/dataaccess/dataset
					${terralib_INCLUDE_DIRS}/terralib/dataaccess/datasource
					${terralib_INCLUDE_DIRS}/terralib/dataaccess/query
					${terralib_INCLUDE_DIRS}/terralib/dataaccess/serialization
					${terralib_INCLUDE_DIRS}/terralib/dataaccess/serialization/xml
					${terralib_INCLUDE_DIRS}/terralib/dataaccess/utils
					${terralib_INCLUDE_DIRS}/terralib/postgis
					${terralib_INCLUDE_DIRS}/terraview
					${terralib_INCLUDE_DIRS}/terraview/config
					${terralib_INCLUDE_DIRS}/terraview/events
					${terralib_INCLUDE_DIRS}/terraview/settings
					${terralib_INCLUDE_DIRS}/terraview/settings/ui
					${terralib_INCLUDE_DIRS}/terraview/ui
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
# process TERRALIB configuration files
#

configure_file(${CMAKE_SOURCE_DIR}/terralib-layout-config-pkg.cmake.in
               ${CMAKE_BINARY_DIR}/pkg/terralib-layout-config.cmake @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/terralib-layout-config-version.cmake.in
               ${CMAKE_BINARY_DIR}/terralib-layout-config-version.cmake @ONLY)

configure_file(${CMAKE_SOURCE_DIR}/terralib-layout-config.cmake.in
               ${CMAKE_BINARY_DIR}/terralib-layout-config.cmake @ONLY)

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
# install and targets export
#

file(GLOB TERRALIB_HDR_FILES ${TERRALIB_ABSOLUTE_ROOT_DIR}/src/terralib/*.h)

install(FILES ${CMAKE_BINARY_DIR}/terralib_layout_buildconfig.h
              ${CMAKE_BINARY_DIR}/terralib_layout_version.h
              ${TERRALIB_HDR_FILES}
        DESTINATION ${TERRALIB_DESTINATION_HEADERS}/terralib COMPONENT devel)

install(FILES ${terralib_BINARY_DIR}/pkg/terralib-layout-config.cmake
              ${terralib_BINARY_DIR}/terralib-layout-config-version.cmake
        DESTINATION ${TERRALIB_DESTINATION_LIBRARY}/cmake/terralib-layout-${TERRALIB_PKG_VERSION} COMPONENT devel)

#install(EXPORT terralib-targets
#        DESTINATION ${TERRALIB_DESTINATION_LIBRARY}/cmake/terralib-layout-${TERRALIB_PKG_VERSION}
#        FILE terralib-layout-exports.cmake
#        COMPONENT devel)

install(DIRECTORY ${TERRALIB_ABSOLUTE_ROOT_DIR}/share/terralib/config
        DESTINATION ${TERRALIB_DESTINATION_SHARE}/terralib COMPONENT runtime)

#
# CPack
#

set(CPACK_SOURCE_INSTALLED_DIRECTORIES "${TERRALIB_ABSOLUTE_ROOT_DIR};/")

configure_file(${terralib_SOURCE_DIR}/terralib-layout-cpack-options.cmake.in
               ${terralib_BINARY_DIR}/terralib-layoutCPackOptions.cmake @ONLY)

set(CPACK_PROJECT_CONFIG_FILE ${terralib_BINARY_DIR}/terralib-layout-CPackOptions.cmake)

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
    
                    
                    
