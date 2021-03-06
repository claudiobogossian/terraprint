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
  \file TextGridSettingsController.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEXT_GRID_SETTINGS_CONTROLLER_H 
#define __TERRALIB_LAYOUT_INTERNAL_TEXT_GRID_SETTINGS_CONTROLLER_H

// TerraLib
#include "../core/pattern/mvc/AbstractOutsideController.h"
#include "../core/property/Property.h"
#include "../core/Config.h"

namespace te
{
  namespace dt
  {
    class AbstractData;
  }

  namespace layout
  {
    class AbstractOutsideModel;

    class TELAYOUTEXPORT TextGridSettingsController : public AbstractOutsideController
    {
      public:

        TextGridSettingsController(AbstractOutsideModel* o);

        virtual ~TextGridSettingsController();

        virtual Property updateProperty();

        virtual void addUpdateProperty(std::string name, te::dt::AbstractData* absData, te::layout::EnumType* dataType);

        virtual void addUpdateTextGridProperty(Property subProperty);
        
        virtual void clearUpdate();

        virtual Property getProperty(std::string name);

      protected:

        Property m_update;
        Property m_textGridSettings;
    };
  }
}

#endif

