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
  \file NorthSettingsConfigProperties.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_NORTH_SETTINGS_CONFIG_PROPERTIES_H
#define __TERRALIB_LAYOUT_INTERNAL_NORTH_SETTINGS_CONFIG_PROPERTIES_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    class TELAYOUTEXPORT NorthSettingsConfigProperties
    {
      public:

        NorthSettingsConfigProperties();

        virtual ~NorthSettingsConfigProperties();

        std::string getBtnColor();

        std::string getDoubleSpinBoxNorthWidth();

        std::string getDoubleSpinBoxNorthHeight();

        std::string getCbNorth();

      protected:

        std::string m_btnColor;

        std::string m_doubleSpinBoxNorthWidth;

        std::string m_doubleSpinBoxNorthHeight;

        std::string m_cbNorth;
    };
  }
}

#endif
