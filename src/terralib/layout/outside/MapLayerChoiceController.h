/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file MapLayerChoiceOutsideController.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_LAYER_CHOICE_CONTROLLER_H
#define __TERRALIB_LAYOUT_INTERNAL_MAP_LAYER_CHOICE_CONTROLLER_H

// TerraLib
#include "../core/pattern/mvc/AbstractOutsideController.h"
#include "../core/Config.h"
#include "../qt/core/Scene.h"
#include "../core/pattern/proxy/AbstractProxyProject.h"

namespace te
{
  namespace layout
  {
    class AbstractOutsideModel;
    class AbstractProxyProject;
    class Property;
    class Scene;
    

    class TELAYOUTEXPORT MapLayerChoiceController : public AbstractOutsideController
    {
      public:

        MapLayerChoiceController(Scene * scene, AbstractProxyProject * proxy, AbstractOutsideModel* o);

        virtual ~MapLayerChoiceController();
        virtual Property getProperty(std::string name);
        virtual   std::list<te::map::AbstractLayerPtr> getlistLayers();
        virtual   std::list<te::map::AbstractLayerPtr> searchLayers();
        virtual   std::list<te::map::AbstractLayerPtr> getSelectedlayers();

      private:
        Scene * m_scene;
        AbstractProxyProject * m_proxy;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_MAP_LAYER_CHOICE_CONTROLLER_H
