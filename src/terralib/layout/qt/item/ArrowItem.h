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
  \file ArrowItem.h
   
   \brief Class that represents a graphic Arrow. 
      Its coordinate system is the same of scene (millimeters). 
      This class is also son of AbstractItem, so it can become observer of a model (Observable). 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ARROW_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_ARROW_ITEM_H

// TerraLib
#ifndef Q_MOC_RUN
#include "AbstractItem.h"
#endif
#include "../../core/Config.h"

#include <QGraphicsItem>

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    /*!
    \brief Class that represents a graphic Arrow. 
        Its coordinate system is the same of scene (millimeters). 
        He is also the son of AbstractItem, so it can become observer of a model (Observable). 
    
      \ingroup layout

      \sa te::layout::ObjectItem
    */
    class TELAYOUTEXPORT ArrowItem : public AbstractItem
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        ArrowItem(te::layout::ItemInputProxy* itemInputProxy);
        
        /*!
          \brief Destructor
         */
        virtual ~ArrowItem();

       protected:

         virtual AbstractItemModel* createModel() const;

         
         virtual void drawItem ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        /*!
          \brief Drawing method of a Right Arrow

          \param canvas
          \param utils
          \param box
         */
        virtual void drawRightArrow(QPainter * painter);

        /*!
          \brief Drawing method of a Right Arrow

          \param canvas
          \param utils
          \param box
         */
        virtual void drawLeftArrow(QPainter * painter);
        /*!
          \brief Drawing method of a Double Arrow

          \param canvas
          \param utils
          \param box
         */
        virtual void drawDoubleArrow(QPainter * painter);
    };
  }
}

#endif
