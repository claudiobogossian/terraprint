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
  \file SVGItem.h
   
   \brief Class that represents a graphic Point. 
      Its coordinate system is the same of scene (millimeters). 
      This is also son of ItemObserver and ObjectItem, so it can become observer of a model (Observable). 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SVG_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_SVG_ITEM_H

// TerraLib
#include "../../core/Config.h"
#ifndef Q_MOC_RUN
#include "AbstractItem.h"
#endif

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    class AbstractItemModel;
    /*!
    \brief Class that represents a graphic SVG. 
        Its coordinate system is the same of scene (millimeters). 
        He is also the son of AbstractItem, so it can become observer of a model (Observable). 

      \ingroup layout

      \sa te::layout::AbstractItem
    */
    class TELAYOUTEXPORT SVGItem : public AbstractItem
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        SVGItem(te::layout::ItemInputProxy* itemInputProxy);

        /*!
          \brief Destructor
         */
        virtual ~SVGItem();

      protected:

        virtual AbstractItemModel* createModel() const;

        virtual AbstractItemController* createController() const;

        virtual void drawItem ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    };
  }
}

#endif
