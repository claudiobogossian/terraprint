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
  \file MapItem.h
   
   \brief This class is a proxy MapDisplay. This makes it possible to add a MapDisplay as item of a scene. 
   This object is of type QGraphicsProxyWidget. He have a directly interaction by user. 
   His transformation matrix is inverted, that is, the inverse of the matrix of the scene, so its coordinate system is screen (pixel), 
   but its position in the scene remains in millimeters.
   This is also son of AbstractItem, so it can become observer of a model (Observable). "View" part of MVC component.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_MAP_ITEM_H

// TerraLib
#ifndef Q_MOC_RUN
#include "AbstractItem.h"
#endif
#include "../../core/Config.h"

// Qt
#include <QGraphicsObject>
#include <QCursor>

class QVariant;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class Pan;
      class ZoomWheel;
      class AbstractTool;
    }
  }

  namespace layout
  {
    class AbstractItemController;
    class EnumType;

    /*!
    \brief This class is a proxy MapDisplay. This makes it possible to add a MapDisplay as item of a scene. 
    This object is of type QGraphicsProxyWidget. He have a directly interaction by user. 
    His transformation matrix is inverted, that is, the inverse of the matrix of the scene, so its coordinate system is screen (pixel), 
    but its position in the scene remains in millimeters.
    Drawing starting point is llx, lly.
    He is also the son of AbstractItem, so it can become observer of a model (Observable). "View" part of MVC component.
    
    \ingroup layout

    \sa te::layout::AbstractItem
  */
    class TELAYOUTEXPORT MapItem : public AbstractItem<QGraphicsObject>
    {
      Q_OBJECT //for slots/signals

      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */
        MapItem(AbstractItemController* controller);

        /*!
          \brief Destructor
        */ 
        virtual ~MapItem();

        virtual bool changeCurrentTool(EnumType* tool);

        virtual bool removeCurrentTool();

        virtual void doRefresh();

        virtual void redraw();

        virtual void recompose();

      protected:

        /*!
          \brief For any specific drawing, the item must reimplement this function
        */
        virtual void drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );


        /*!
        \brief Draws the map in the given device
        */
        virtual void drawMapOnDevice(QPaintDevice* device);

        /*!
        \brief Draws the map in the given painter
        */
        virtual void drawMapOnPainter(QPainter* painter);

        /*!
        \brief Draws the layers using the given canvas
        */
        virtual void drawLayers(te::qt::widgets::Canvas* canvas, const te::gm::Envelope& envelope);

        /*!
          \brief Reimplemented from QGraphicsItem to capture changes in the item
        */
        virtual QVariant itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value );

        /*!
           \brief Reimplemented from QGraphicsProxyWidget
        */
        virtual void  mousePressEvent ( QGraphicsSceneMouseEvent * event );

        /*!
            \brief Reimplemented from QGraphicsProxyWidget
          */
        virtual void  mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

        /*!
            \brief Reimplemented from QGraphicsProxyWidget
          */
        virtual void  mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsProxyWidget
        */
        virtual void  dragEnterEvent ( QGraphicsSceneDragDropEvent * event );

        /*!
          \brief Reimplemented from QGraphicsProxyWidget
         */
        virtual void  dropEvent ( QGraphicsSceneDragDropEvent * event );
        
        /*!
          \brief Reimplemented from QGraphicsProxyWidget
        */
        virtual void  wheelEvent(QGraphicsSceneWheelEvent * event);

        virtual void enterEditionMode();

        virtual void leaveEditionMode();

        virtual QCursor createCursor(std::string pathIcon);

        virtual QPixmap& getDraftPixmap();

      protected:

        EnumType*                       m_currentEditionMode;
        QPixmap                         m_screenCache;
        QPixmap                         m_screenDraft;
        te::gm::Point                   m_clickedPointMM;
    };
  }
}

#endif


