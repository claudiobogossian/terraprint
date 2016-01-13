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
  \file MapCompositionItem.h
   
   \brief Class that represents the grouping of objects of type QGraphicsItem, MVC components.
   Its coordinate system is the same of scene (millimeters). 
   It is also the son of AbstractItem, so it can become observer of a model (Observable). 
   The group component must be initialized with a position (setPos).
   This group can be resized. Its internal items not.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_COMPOSITION_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_MAP_COMPOSITION_ITEM_H

//TerraLib
#include "../../core/Config.h"
#include "ItemGroup.h"

// Qt
#include <QGraphicsItem>

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    class AbstractItemView;
    /*!
    \brief Class that represents the grouping of objects of type QGraphicsItem, MVC components.
        Its coordinate system is the same of scene (millimeters). 
        It is also the son of AbstractItem, so it can become observer of a model (Observable).  
        The group component must be initialized with a position (setPos).
        This group can be resized. Its internal items not.

      \ingroup layout

      \sa te::layout::ItemObserver
    */
    class TELAYOUTEXPORT MapCompositionItem : public ItemGroup
    {
      public:

        MapCompositionItem(AbstractItemController* controller, bool invertedMatrix = false);

        virtual ~MapCompositionItem();
        
        /*!
        \brief Reimplemented from QGraphicsItem to capture changes in the item
        */
        virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant & value);

      protected:

        /*!
        \brief Reimplemented from AbstractItem<QGraphicsItemGroup>
        */
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

        virtual void updateObservableSize(AbstractItemView* observableItem);
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_ASSOCIATE_ITEMGROUP_H
