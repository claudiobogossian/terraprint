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
  \file terraprint/layout/Version.h

  \brief Utility class for system versioning.
*/

#ifndef __TERRALIB_LAYOUT_CORE_INTERNAL_VERSION_H
#define __TERRALIB_LAYOUT_CORE_INTERNAL_VERSION_H

// TerraPrint
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    class TELAYOUTEXPORT Version
    {
      public:

        static int majorNumber();

        static int minorNumber();

        static int patchNumber();

        static std::string buildDate();

        static std::string asString();

        static int asInt();

      private:

        Version();

        ~Version();
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_LAYOUT_CORE_INTERNAL_VERSION_H



