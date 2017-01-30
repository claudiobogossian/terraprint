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
  \file GridSettingsController.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_CONTROLLER_H 
#define __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_CONTROLLER_H

// TerraLib
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../../core/property/Property.h"
#include "../../core/Config.h"

// Qt
#include <QStringList>

namespace te
{
  namespace layout
  {
    class AbstractOutsideModel;
    class Scene;
    class AbstractItemView;

    class TELAYOUTEXPORT GridSettingsController : public AbstractOutsideController
    {
      public:

        GridSettingsController(Scene* scene, AbstractOutsideModel* o);

        virtual ~GridSettingsController();
                
        virtual Property getProperty(std::string name);

        virtual QStringList getItemNames(QStringList list, const EnumType* type);
                
        virtual bool containsGrid();

        virtual AbstractItemView* searchGrid();

      protected:

        Scene*                 m_scene;

    };
  }
}

#endif



