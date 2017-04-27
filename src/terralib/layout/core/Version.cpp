/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terraprint/layout/Version.cpp

  \brief Utility class for system versioning.
*/

// TerraPrint
#include "Version.h"

#include "../terraprint_version.h"

// STL
#include <cassert>

int te::layout::Version::majorNumber()
{
  return TERRALIB_LAYOUT_VERSION_MAJOR;
}

int te::layout::Version::minorNumber()
{
  return TERRALIB_LAYOUT_VERSION_MINOR;
}

int te::layout::Version::patchNumber()
{
  return TERRALIB_LAYOUT_VERSION_PATCH;
}

std::string te::layout::Version::buildDate()
{
  assert(__DATE__ " " __TIME__);
  return std::string(__DATE__ " " __TIME__);
}

std::string te::layout::Version::asString()
{
  assert(TERRALIB_LAYOUT_VERSION_STRING);
  return std::string(TERRALIB_LAYOUT_VERSION_STRING);
}

int te::layout::Version::asInt()
{
  return TERRALIB_LAYOUT_VERSION;
}

