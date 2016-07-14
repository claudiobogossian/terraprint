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
  \file TextController.h
   
  \brief Class that represents text controller.
  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_CONTROLLER_H
#define __TERRALIB_LAYOUT_INTERNAL_MAP_CONTROLLER_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractItemController.h"

#include "terralib/maptools/AbstractLayer.h"

namespace te
{
  namespace layout
  {
    class AbstractItemModel;
    class AbstractProxyProject;
    class Property;
    class Properties;
    /*!
    \brief Class that represents text controller.
    
    \ingroup layout
    \sa te::layout::AbstractItemController
    */
    class TELAYOUTEXPORT MapController : public AbstractItemController
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        MapController(AbstractItemModel* model);

        /*!
          \brief Destructor
        */ 
        virtual ~MapController();

        virtual void recomposeExtent();

        virtual void zoom(const te::gm::Point& pointWorld, bool zoomIn);

        virtual void zoom(const te::gm::Envelope& box);

        virtual void pan(double dx, double dy);

        /*!
          \brief Method called by the view to inform that new layers have been added
        */
        virtual void addLayers(const std::list<te::map::AbstractLayerPtr>& layerList);

        virtual void setProperty(const Property& property);

        virtual void setProperties(const Properties& properties);

        virtual std::list<te::map::AbstractLayerPtr> getIntersection(const std::list<te::map::AbstractLayerPtr>& listA, const std::list<te::map::AbstractLayerPtr>& listB) const;
        
        virtual void validateItem();

      protected:

        virtual Property syncLayersFromURIs(const Property& property);

        virtual Property syncURIsFromLayers(const Property& property);

        virtual bool syncLayersProperties(Properties& properties);

        virtual bool syncSridAndEnvelope(Properties& properties);

        virtual bool syncMapSizeProperties(Properties& properties);

        virtual bool syncMapScaleProperties(Properties& properties);

        virtual AbstractProxyProject* getAbstractProxyProject();

    };
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_MAP_CONTROLLER_H
