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
  \file AbstractItem.h
   
   \brief Abstract class that represents a graphic item.  
   Its coordinate system is the same of scene (millimeters). Knows rotate and resize. Stores a pixmap drawn by model.
   This is also son of ItemObserver, so it can become observer of a model (Observable). 
   This class will be inherited and will became the view part of the MVC component.
   Who inherits it is required the implementation of updateObserver(ContextItem context) method.
   Drawing starting point is llx, lly.
   Can't add signals and slots in this class because moc(Qt) doesn't support templates.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ITEM_H

#include "../../core/Config.h"

// TerraLib
#include "../core/ItemUtils.h"
#include "../../core/Utils.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/enum/AbstractType.h"

//Qt
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QRectF>
#include <QPoint>
#include <QVariant>
#include <QEvent>
#include <QGraphicsScene>
#include <QColor>
#include <QGraphicsSceneHoverEvent>
#include <QSize>
#include <QMap>
#include <QToolTip>

//STL
#include <math.h>

class QWidget;
class QUndoCommand;

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    class AbstractItemModel;
    class AbstractScene;
    class Context;
    /*!
    \brief Abstract class that represents a graphic item.  
      Its coordinate system is the same of scene (millimeters). Knows rotate and resize. Stores a pixmap drawn by model.
      This is also son of AbstractItemView, so it can become observer of a model (Observable). 
      This class will be inherited and will became the view part of the MVC component.
      Who inherits it is required the implementation of updateObserver(ContextItem context) method.
      Drawing starting point is llx, lly.
      Can't add signals and slots in this class because moc(Qt) doesn't support templates.
      Each item, at the end of the setProperties method (AbstractItemView), adds a Property Change command to the Undo/Redo stack, via the scene.
      \ingroup layout

      \sa te::layout::AbstractItemView
    */
	class TELAYOUTEXPORT AbstractItem : public QGraphicsItem, public AbstractItemView
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */
        AbstractItem();

        /*!
          \brief Destructor
        */
        virtual ~AbstractItem();

        /*!
          \brief Gets the bounding rect of the item, in local coordinate system
        */
        virtual QRectF boundingRect() const;

        /*!
        \brief This function is called every time the view must be updated
        */
        virtual void refresh();

        virtual void contextUpdated(const ContextObject& context);

        /*!
          \brief Gets the rotation
        */ 
        virtual double getItemRotation() const;

        /*!
          \brief Implemented from AbstractItemView.
        */ 
        virtual void setItemRotation(double rotation);

        /*!
          \brief Implemented from AbstractItemView.
        */
        virtual void setItemPosition(double x, double y);

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        virtual void enterEditionMode();

        virtual void leaveEditionMode();

        virtual te::layout::ItemAction getCurrentAction();

        virtual void prepareGeometryChange();

        /*!
        \brief Implemented from AbstractItemView. Each item, at the end of the setProperties method (AbstractItemView), 
            adds a Property Change command to the Undo/Redo stack, via the scene.
        */
        virtual void addUndoCommandToStack(QUndoCommand* command);

      protected:

        /*!
          \brief  Gets the adjusted boundigned rectangle which considers the current state of the QPen that will be used to draw it. 
                  The returned rect will be in the item coordinate system.
         */
        virtual QRectF getAdjustedBoundingRect(QPainter* painter) const;

        /*!
          \brief For any specific drawing, the item must reimplement this function
         */
        virtual void drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 ) = 0;

        /*!
          \brief Draws the background of the item
         */
        virtual void drawBackground( QPainter* painter );

        /*!
          \brief Draws the frame of the item
         */
        virtual void drawFrame(QPainter* painter);

        /*!
          \brief Draws the selection of the item
         */
        virtual void drawSelection(QPainter* painter);

        /*!
        \brief Draws the feedback of the item that is in edition mode
        */
        virtual void drawEdition(QPainter* painter);
        
        /*!
          \brief Reimplemented from QGraphicsItem to capture changes in the item
         */
        virtual QVariant itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void hoverMoveEvent( QGraphicsSceneHoverEvent * event );

        /*!
        \brief Reimplemented from QGraphicsItem
        */
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

        virtual bool checkTouchesCorner( const double& x, const double& y );

        virtual bool checkRotationArea(const double& x, const double& y);

        virtual void checkTouchesWarningAlert(const double& x, const double& y/*, QPainter * painter*/);

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
        
        virtual void setPixmap();

        virtual void drawItemResized( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
        
        virtual AbstractScene* getScene() const;

        virtual QRectF qRectToQPolygonMap(QRectF rect);
        
        virtual void drawWarningAlert(QPainter * painter);

        virtual bool isZoomAdequateForResize() const;

        virtual bool isZoomAdequateForRotation() const;

     protected:

        //resize
        QRectF                            m_rect;
        QPixmap                           m_clonePixmap;
        QPointF                           m_initialCoord;
        QPointF                           m_finalCoord;
        LayoutAlign                       m_enumSides;
        te::layout::ItemAction            m_currentAction;
        double                            m_marginResizePrecision; //precision
        QPolygonF                         m_polygonWarning;
        bool                              m_isPrinting;
        int                               m_hotPointSizePixels;
        int                               m_selectionLineWidthPixels;
        int                               m_rotationHotPointSizePixels;

    };
  } // end namespace layout
} // end namespace te

#endif //__TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ITEM_H
