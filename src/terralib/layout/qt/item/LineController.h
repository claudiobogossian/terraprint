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
  \file LineController.h
   
  \brief Class that represents line controller.
  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LINE_CONTROLLER_H
#define __TERRALIB_LAYOUT_INTERNAL_LINE_CONTROLLER_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractItemController.h"

// Qt
#include <QPolygonF>

namespace te
{
  namespace gm
  {
    class Geometry;
  }

  namespace layout
  {
    /*!
    \brief Class that represents line controller.
    
    \ingroup layout
    \sa te::layout::AbstractItemController
    */
    class TELAYOUTEXPORT LineController : public AbstractItemController
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVM component
          \param o "Model" part of MVM component
        */ 
        LineController(AbstractItemModel* model, AbstractItemView* view);

        /*!
          \brief Destructor
        */ 
        virtual ~LineController();

        virtual void update(const Subject* subject);

        virtual QPolygonF getQPolygon();

        virtual te::gm::Geometry* getGeometry();

        virtual void setProperties(const te::layout::Properties& properties);

      protected:

        virtual QPolygonF recalculatePolygon();

        virtual void verifyPolygon();

        bool isGeometrySizeChange();


    };
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_LINE_CONTROLLER_H

