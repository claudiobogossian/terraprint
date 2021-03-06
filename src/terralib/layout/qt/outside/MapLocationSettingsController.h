/*  Copyright (C) 2001-2016 National Institute For Space Research (INPE) - Brazil.

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
\file MapLocationSettingsController.h

\brief

\ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_LOCATION_MAP_SETTINGS_CONTROLLER_H 
#define __TERRALIB_LAYOUT_LOCATION_MAP_SETTINGS_CONTROLLER_H

// TerraLib
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../../core/property/Property.h"
#include "../../core/Config.h"
#include "../core/Scene.h"

namespace te
{
  namespace layout
  {
    class AbstractOutsideModel;
    class MapLayerChoiceOutside;
    class AbstractProxyProject;


    class TELAYOUTEXPORT MapLocationSettingsController : public AbstractOutsideController
    {
    public:

      MapLocationSettingsController(Scene * scene, AbstractProxyProject * proxy, QWidget* parent, AbstractOutsideModel* o);

      virtual ~MapLocationSettingsController();
      virtual Property getProperty(std::string name);
      virtual MapLayerChoiceOutside* getMapLayerChoice();
      virtual QStringList getItemNames(QStringList list, const EnumType* type);
    
    private:
      Scene * m_scene;
      AbstractProxyProject * m_proxy;
      QWidget* m_parent;
    };
  }
}

#endif