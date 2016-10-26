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
  \file BalloonItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_BALLOON_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_BALLOON_ITEM_H

// TerraLib
#ifndef Q_MOC_RUN
#include "AbstractItem.h"
#endif
#include "TextItem.h"
#include "../../core/Config.h"

#include <QGraphicsItem>

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    /*!
    \brief Class that represents a graphic Balloon. 
        Its coordinate system is the same of scene (millimeters). 
        He is also the son of AbstractItem, so it can become observer of a model (Observable). 

      \ingroup layout

      \sa te::layout::AbstractItem
    */
    class TELAYOUTEXPORT BalloonItem : public TextItem
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        BalloonItem(AbstractItemController* controller);

        /*!
          \brief Destructor
         */
        virtual ~BalloonItem();

        virtual QRectF boundingRect() const;

      protected:

        virtual void drawItem ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
        virtual void drawRectangleBalloon(QPainter * painter);
        virtual void drawRoundedRectangleBalloon(QPainter * painter);
        virtual void drawEllipseBalloon(QPainter * painter);
        void setPainterParameters(QPainter* painter);
        void setBalloonDirection(QPainterPath& qpainterpath);
    };

  }
}

#endif


