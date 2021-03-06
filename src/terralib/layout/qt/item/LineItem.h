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
  \file LineItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LINE_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_LINE_ITEM_H

// TerraLib
#ifndef Q_MOC_RUN
#include "AbstractItem.h"
#endif

// Qt 
#include <QGraphicsItem>

class QPen;
class QGraphicsSceneMouseEvent;

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    class AbstractItemModel;

    class LineItem : public AbstractItem
    {
      public:

        LineItem(te::layout::ItemInputProxy* itemInputProxy);

        virtual ~LineItem ();

      protected:

        virtual AbstractItemModel* createModel() const;

        virtual AbstractItemController* createController() const;

        virtual void drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        virtual QPen searchStyle();
    };
  }
}

#endif
