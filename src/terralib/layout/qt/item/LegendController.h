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
  \file LegendController.h
   
  \brief Class that represents legend controller.
  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LEGEND_CONTROLLER1_H
#define __TERRALIB_LAYOUT_INTERNAL_LEGEND_CONTROLLER1_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "terralib/maptools/AbstractLayer.h"

// STL
#include <list>

namespace te
{
  namespace layout
  {
    class AbstractProxyProject;
    class AbstractItemModel;
    class Properties;

    /*!
    \brief Class that represents legend controller.
    
    \ingroup layout
    \sa te::layout::AbstractItemController
    */
    class TELAYOUTEXPORT LegendController : public AbstractItemController
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVM component
          \param o "Model" part of MVM component
        */ 
        LegendController(AbstractItemModel* model, AbstractItemView* view);

        /*!
          \brief Destructor
        */ 
        virtual ~LegendController();

        virtual void update(const Subject* subject);

        virtual std::list<te::map::AbstractLayerPtr> searchLayersFromURI();

        virtual std::list<te::map::AbstractLayerPtr> searchVisibleLayersFromURI();

        virtual void setProperties(const Properties& properties);

      protected:

        virtual Properties checkToUpdate(const AbstractItemModel* subjectModel);

        virtual bool someExistsVisibleList(const std::vector<std::string>& layersNewUri, const std::vector<std::string>& layersVisibleUri);

        virtual AbstractProxyProject* getAbstractProxyProject();

        virtual void resetSettings(Properties& properties);
    };
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_LEGEND_CONTROLLER1_H

