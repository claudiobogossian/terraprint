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
  \file LegendChoiceController.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LEGEND_CHOICE_CONTROLLER_H 
#define __TERRALIB_LAYOUT_INTERNAL_LEGEND_CHOICE_CONTROLLER_H

#include "../core/pattern/mvc/AbstractOutsideController.h"
#include "../core/pattern/proxy/AbstractProxyProject.h"
#include "../qt/core/Scene.h"
#include "../core/Config.h"

// TerraLib
#include <terralib/maptools/AbstractLayer.h>

// STL
#include <list>

// Qt
class QStringList;

namespace te
{
  namespace layout
  {
    class AbstractOutsideModel;

    class TELAYOUTEXPORT LegendChoiceController : public AbstractOutsideController
    {
      public:

        LegendChoiceController(Scene * scene, AbstractProxyProject * proxy, AbstractOutsideModel* o);

        virtual ~LegendChoiceController();

        virtual Property getProperty(std::string name);

        virtual std::list<te::map::AbstractLayerPtr> getlistLayers();

        virtual std::list<te::map::AbstractLayerPtr> searchLayers(const std::string& name = "");

        virtual std::list<te::map::AbstractLayerPtr> getSelectedlayers();

        virtual std::string searchLayerToURI(te::map::AbstractLayerPtr layer);

        virtual QStringList getItemNames(const QStringList& list, const EnumType* type);

      private:

        Scene * m_scene;
        AbstractProxyProject * m_proxy;
    };
  }
}

#endif