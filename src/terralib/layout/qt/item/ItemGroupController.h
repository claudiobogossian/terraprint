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

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEMGROUP_CONTROLLER_H
#define __TERRALIB_LAYOUT_INTERNAL_ITEMGROUP_CONTROLLER_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractItemController.h"

class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class AbstractItemModel;
    class AbstractItemView;
    /*!
    \brief Class that represents text controller.
    
    \ingroup layout
    \sa te::layout::AbstractItemController
    */
    class TELAYOUTEXPORT ItemGroupController : public AbstractItemController
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        ItemGroupController(AbstractItemModel* model, AbstractItemView* view);

        /*!
          \brief Destructor
        */ 
        virtual ~ItemGroupController();

        void addItem(QGraphicsItem* child);

        virtual void setProperties(const Properties& properties);

        /*!
        \brief Method called by the subject to inform changes in the model
        */
        virtual void update(const Subject* subject);

        virtual void removeFromGroup(QGraphicsItem* item);

    protected:
      virtual void scaleChildrenItems(Properties& properties);

      virtual QRectF resizeChildren(QGraphicsItem* qItem, double widthFactor, double heightFactor);

    protected:

      bool m_propagateResize;
      bool m_ignoreChildrenUpdates;
    };
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_ITEMGROUP_CONTROLLER1_H
