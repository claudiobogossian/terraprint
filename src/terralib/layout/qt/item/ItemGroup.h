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
  \file ItemGroup.h
   
   \brief Class that represents the grouping of objects of type QGraphicsItem, MVC components.
   Its coordinate system is the same of scene (millimeters). 
   It is also the son of AbstractItem, so it can become observer of a model (Observable). 
   The group component must be initialized with a position (setPos).

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEMGROUP_H
#define __TERRALIB_LAYOUT_INTERNAL_ITEMGROUP_H

//TerraLib
#include "../../core/Config.h"
#ifndef Q_MOC_RUN
#include "AbstractItem.h"
#endif

// Qt
#include <QGraphicsItem>

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    /*!
    \brief Class that represents the grouping of objects of type QGraphicsItem, MVC components.
        Its coordinate system is the same of scene (millimeters). 
        It is also the son of AbstractItem, so it can become observer of a model (Observable).  
        The group component must be initialized with a position (setPos).

      \ingroup layout

      \sa te::layout::ItemObserver
    */
    class TELAYOUTEXPORT ItemGroup : public AbstractItem
    {
      public:

        ItemGroup(te::layout::ItemInputProxy* itemInputProxy);

        virtual ~ItemGroup();

        /*!
          \brief For any specific drawing, the item must reimplement this function
        */
        virtual void drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        /*!
          \brief Reimplemented from QGraphicsItem to capture changes in the item
         */
        virtual QVariant itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value );

        virtual void addToGroup(QGraphicsItem* item);

        virtual void removeFromGroup(QGraphicsItem* item);

        /*
        \brief Set properties will or not generate an UndoCommand on the stack.

        \param enabled if true will generate an UndoCommand on the stack, false otherwise.

        */
        virtual void setUndoEnabled(bool enabled);

      protected:

        virtual AbstractItemModel* createModel() const;

        virtual AbstractItemController* createController() const;

        /*!
        \brief Reimplemented from AbstractItem<QGraphicsItemGroup>
        */
        virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
        
        virtual bool hasChildrenInResizeMode();

      protected:

        bool m_stacksBehindParent;
        bool m_isSubSelectionAllowed;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_ITEMGROUP_H
