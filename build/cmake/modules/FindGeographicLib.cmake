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
#  Description: Find GeographicLib - find GeographicLib include directory and libraries.
#
#  GeographicLib_INCLUDE_DIR - where to find GeographicLib.h.
#  GeographicLib_LIBRARY     - where to find GeographicLib libraries.
#  GeographicLib_FOUND       - True if GeographicLib found.
#
#

if(UNIX)

  find_path(GeographicLib_INCLUDE_DIR MagneticModel.hpp
            PATHS /usr
                  /usr/local
                  /usr/local/GeographicLib
            PATH_SUFFIXES include
                          GeographicLib
                          include/GeographicLib
                          lib/GeographicLib.framework/Headers)

  find_library(GeographicLib_LIBRARY
               NAMES GeographicLib
               PATHS /usr
                     /usr/local
                     /usr/local/Geographic-i
               PATH_SUFFIXES lib
                             lib/GeographicLib.framework)

elseif(WIN32)

  find_path(GeographicLib_INCLUDE_DIR
            NAMES MagneticModel.hpp
            PATH_SUFFIXES  include/GeographicLib)

  find_library(GeographicLib_LIBRARY
               NAMES Geographic-i
               PATH_SUFFIXES lib)

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(GeographicLib DEFAULT_MSG GeographicLib_LIBRARY GeographicLib_INCLUDE_DIR)

mark_as_advanced(GeographicLib_INCLUDE_DIR GeographicLib_LIBRARY)
