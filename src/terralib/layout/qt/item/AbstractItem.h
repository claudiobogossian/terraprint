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

// TerraLib
#include "../core/ItemUtils.h"
#include "../../core/Utils.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/AbstractScene.h"
#include "../../core/property/Property.h"
#include "../../core/pattern/singleton/Context.h"
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

class QWidget;

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    class AbstractItemModel;
    /*!
    \brief Abstract class that represents a graphic item.  
      Its coordinate system is the same of scene (millimeters). Knows rotate and resize. Stores a pixmap drawn by model.
      This is also son of AbstractItemView, so it can become observer of a model (Observable). 
      This class will be inherited and will became the view part of the MVC component.
      Who inherits it is required the implementation of updateObserver(ContextItem context) method.
      Drawing starting point is llx, lly.
      Can't add signals and slots in this class because moc(Qt) doesn't support templates.
      \ingroup layout

      \sa te::layout::AbstractItemView
    */
    template <class T>
    class AbstractItem : public T, public AbstractItemView
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */
        AbstractItem(AbstractItemController* controller,  bool invertedMatrix = false);

        /*!
          \brief Destructor
        */
        virtual ~AbstractItem();

        /*!
          \brief Gets the bounding rect of the item, in local coordinate system
        */
        virtual QRectF boundingRect() const;

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

        /*!
        \brief Reimplemented from QGraphicsItem. World coordinates(mm).
        */
        virtual bool contains(const QPointF & point) const;

        virtual void enterEditionMode();

        virtual void leaveEditionMode();

        virtual void prepareGeometryChange();

        virtual te::layout::ItemAction getCurrentAction();
        
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
        \brief Draws the given text in the given location with rotation
        */
        virtual void drawText(const QPointF& point, QPainter* painter, const QFont& font, const std::string& text, int rotate = 0);
        
        /*!
        \brief Draws the given text super script in the given location with rotation
        */
        virtual void drawSuperScriptText(const QPointF& point, QPainter* painter, const QFont& font, const std::string& text, int rotate = 0);

        /*!
        \brief Draws the given image in the given location
        */
        virtual void drawImage(const QRectF& rect, QPainter* painter, const QImage& image);

        /*!
        \brief Draws the given pixmap in the given location
        */
        virtual void drawPixmap(const QRectF& rect, QPainter* painter, const QPixmap& pixmap);

        /*!
          \brief Reimplemented from QGraphicsItem to capture changes in the item
         */
        virtual QVariant itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void hoverMoveEvent( QGraphicsSceneHoverEvent * event );

        virtual bool checkTouchesCorner( const double& x, const double& y );

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
        
        virtual AbstractScene* getScene();

        virtual QRectF qRectToQPolygonMap(QRectF rect);
                
     protected:

        //resize
        QRectF                            m_rect;
        QPixmap                           m_clonePixmap;
        QPointF                           m_initialCoord;
        QPointF                           m_finalCoord;
        LayoutAlign                       m_enumSides;
        te::layout::ItemAction            m_currentAction;
        double                            m_marginResizePrecision; //precision
        double                            m_selectionPointSize;
    };

    template <class T>
    inline te::layout::AbstractItem<T>::AbstractItem(AbstractItemController* controller, bool invertedMatrix)
      : T()
      , AbstractItemView(controller, invertedMatrix)
      , m_enumSides(TPNoneSide)
      , m_currentAction(te::layout::NO_ACTION)
      , m_marginResizePrecision(5.)
      , m_selectionPointSize(10.)
    {
      T::setFlags(QGraphicsItem::ItemIsMovable
        | QGraphicsItem::ItemIsSelectable
        | QGraphicsItem::ItemSendsGeometryChanges
        | QGraphicsItem::ItemIsFocusable);

      //If enabled is true, this item will accept hover events
      T::setAcceptHoverEvents(true);

      m_rect = boundingRect();
    }

    template <class T>
    inline te::layout::AbstractItem<T>::~AbstractItem()
    {
    }

    template <class T>
    inline QRectF te::layout::AbstractItem<T>::boundingRect() const
    {
      if (m_currentAction == te::layout::RESIZE_ACTION)
      {
        return m_rect;
      }

      //models stores size information in item CS. 
      double x = 0.;
      double y = 0.;
      double width = m_controller->getProperty("width").getValue().toDouble();
      double height = m_controller->getProperty("height").getValue().toDouble();

      QRectF boundingRect(0, 0, width, height);
      return boundingRect;
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::refresh()
    {
      T::update();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::contextUpdated(const ContextObject& context)
    {
    }

    template <class T>
    inline double te::layout::AbstractItem<T>::getItemRotation() const
    {
      return T::rotation();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::setItemRotation(double angle)
    {
      QPointF center = boundingRect().center();

      double w = boundingRect().width();
      double h = boundingRect().height();

      if (m_invertedMatrix)
      {
        angle = -angle;
      }

      QTransform currentTransform = T::transform();

      //checks if the is a negative scalling
      //this is usually true form texts, images, etc., due to the inverted CS
      if (currentTransform.m22() < 0)
      {
        angle = angle * -1;
      }

      if (angle == 0.)
        T::setTransformOriginPoint(0, 0);
      else
        T::setTransformOriginPoint(boundingRect().width() / 2., boundingRect().height() / 2.);

      T::setRotation(angle);
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::setItemPosition(double x, double y)
    {
      T::setPos(x, y);
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */)
    {
      Q_UNUSED(option);
      Q_UNUSED(widget);
      if (painter == 0)
      {
        return;
      }

      if ((m_currentAction == te::layout::RESIZE_ACTION || m_currentAction == te::layout::MOVE_ACTION) && m_useResizePixmap)
      {
        drawItemResized(painter, option, widget);
        drawFrame(painter);
        return;
      }

      painter->save();

      //Draws the background
      drawBackground(painter);

      //Draws the item
      drawItem(painter, option, widget);

      //Draws the frame
      drawFrame(painter);

      //Draws the selection
      if (option->state & QStyle::State_Selected || m_subSelected == true)
      {
        drawSelection(painter);
      }

      painter->restore();
    }

    template <class T>
    inline bool te::layout::AbstractItem<T>::contains(const QPointF & point) const
    {
      //te::gm::Coord2D coord(point.x(), point.y());
      return T::contains(point);
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::enterEditionMode()
    {
      if (T::parentItem() != 0)
      {
        T::parentItem()->setHandlesChildEvents(false);
        this->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
      }
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::leaveEditionMode()
    {
      if (T::parentItem() != 0)
      {
        T::parentItem()->setHandlesChildEvents(true);
        this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
      }
    }

    template <class T>
    inline QRectF te::layout::AbstractItem<T>::getAdjustedBoundingRect(QPainter* painter) const
    {
      qreal penWidth = painter->pen().widthF();
      if (painter->pen().style() == Qt::NoPen)
      {
        penWidth = 0.;
      }

      QRectF bRect = boundingRect();

      qreal adj = penWidth / 2.;
      QRectF rectAdjusted = bRect.adjusted(adj, adj, -adj, -adj);

      return rectAdjusted;
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::drawBackground(QPainter * painter)
    {
      if (!painter)
      {
        return;
      }

      const Property& pBackgroundColor = m_controller->getProperty("background_color");
      const te::color::RGBAColor& backgroundColor = pBackgroundColor.getValue().toColor();
      QColor qBackgroundColor(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());

      painter->save();
      painter->setPen(Qt::NoPen);
      painter->setBrush(QBrush(qBackgroundColor));
      painter->setBackground(QBrush(qBackgroundColor));
      painter->setRenderHint(QPainter::Antialiasing, true);

      //gets the adjusted boundigng rectangle based of the painter settings
      QRectF rectAdjusted = getAdjustedBoundingRect(painter);

      //draws the background
      painter->drawRect(rectAdjusted);

      painter->restore();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::drawFrame(QPainter * painter)
    {
      if (!painter)
      {
        return;
      }

      if (m_controller->getProperty("show_frame").getValue().toBool() == false)
      {
        return;
      }

      const Property& pFrameColor = m_controller->getProperty("frame_color");
      const Property& pFrameThickness = m_controller->getProperty("frame_thickness");
      const te::color::RGBAColor& frameColor = pFrameColor.getValue().toColor();
      double frameThickness = pFrameThickness.getValue().toDouble();

      QColor qFrameColor(frameColor.getRed(), frameColor.getGreen(), frameColor.getBlue(), frameColor.getAlpha());

      painter->save();
      QPen pen(qFrameColor, frameThickness, Qt::SolidLine);
      painter->setPen(pen);
      painter->setBrush(Qt::NoBrush);
      painter->setRenderHint(QPainter::Antialiasing, true);

      //gets the adjusted boundigng rectangle based of the painter settings
      QRectF rectAdjusted = getAdjustedBoundingRect(painter);

      //draws the frame
      painter->drawRect(rectAdjusted);

      painter->restore();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::drawSelection(QPainter* painter)
    {
      if (!painter)
      {
        return;
      }

      const Property& pFrameThickness = m_controller->getProperty("frame_thickness");
      double frameThickness = pFrameThickness.getValue().toDouble();

      painter->save();

      int zoom = this->getScene()->getContext().getZoom();

      const QColor fgcolor(0, 255, 0);
      const QColor backgroundColor(0, 0, 0);

      QPen penBackground(backgroundColor, frameThickness, Qt::SolidLine);
      painter->setPen(penBackground);
      painter->setBrush(Qt::NoBrush);

      //gets the adjusted boundigng rectangle based of the painter settings
      QRectF rectAdjusted = boundingRect();
      
      QPen penForeground(fgcolor, frameThickness, Qt::DashLine);
      painter->setPen(penForeground);
      painter->setBrush(Qt::NoBrush);

      QTransform t;
      painter->setTransform(t);
      
      double half = m_selectionPointSize / 2;

      QRectF convertedRect = qRectToQPolygonMap(rectAdjusted);

      qreal penWidth = 3;
      if (painter->pen().style() == Qt::NoPen)
      {
        penWidth = 0.;
      }

      QRectF bRect = convertedRect;

      qreal adj = std::ceil(penWidth / 2.);
      convertedRect = bRect.adjusted(adj, adj, -adj, -adj);

      QPointF topL = convertedRect.topLeft();

      QPointF topR = convertedRect.topRight();

      QPointF bottomR = convertedRect.bottomRight();

      QPointF bottomL = convertedRect.bottomLeft();

      QPointF center = convertedRect.center();

      
      QLineF lineDown(bottomL, bottomR);
      QLineF lineUp(topL, topR);
      QLineF lineRight(bottomL, topL);
      QLineF lineLeft(bottomR, topR);


      penBackground.setWidthF(penWidth);
      painter->setPen(penBackground);
      painter->setBrush(Qt::NoBrush);
      
      painter->drawLine(lineDown);
      painter->drawLine(lineUp);
      painter->drawLine(lineRight);
      painter->drawLine(lineLeft);


      penForeground.setWidthF(penWidth);
      painter->setPen(penForeground);
      painter->setBrush(Qt::NoBrush);

      painter->drawLine(lineDown);
      painter->drawLine(lineUp);
      painter->drawLine(lineRight);
      painter->drawLine(lineLeft);


      QPen pen;
      pen.setColor(QColor(255, 0, 0));
      pen.setStyle(Qt::DotLine);

      QBrush brushEllipse(fgcolor);
      painter->setBrush(fgcolor);

      pen.setWidthF(m_selectionPointSize);
      painter->setPen(pen);
      painter->drawPoint(topL.x()+ half, topL.y() + half);
      painter->drawPoint(topR.x() - half, topR.y() + half);
      painter->drawPoint(bottomR.x() - half, bottomR.y() - half);
      painter->drawPoint(bottomL.x() + half, bottomL.y() - half);
      painter->drawPoint(center.x(), center.y());

      painter->restore();
    }

     template <class T>
     inline QRectF te::layout::AbstractItem<T>::qRectToQPolygonMap(QRectF rect)
     {
       
       QGraphicsScene* scene = this->scene();
       QGraphicsView* view = scene->views().first();

       QRectF convRect = this->mapRectToScene(rect);
       QRectF rectConverted = view->mapFromScene(convRect).boundingRect();

       return rectConverted;
     
     }

    template <class T>
    inline void te::layout::AbstractItem<T>::drawText(const QPointF& point, QPainter* painter, const QFont& font, const std::string& text, int rotate)
    {
      ItemUtils itemUtils = this->getScene()->getItemUtils();

      QString qText = ItemUtils::convert2QString(text);

      QPainterPath textObject = itemUtils.textToVector(qText, font, point, rotate);

      int fontSize = painter->font().pointSize();

      QPen pen = painter->pen();
      pen.setWidthF(0);

      QBrush brush = painter->brush();
      brush.setStyle(Qt::SolidPattern);


      painter->save();

      painter->setPen(pen);
      painter->setRenderHint(QPainter::Antialiasing, true);
      painter->fillPath(textObject, brush);

      painter->restore();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::drawSuperScriptText(const QPointF& point, QPainter* painter, const QFont& font, const std::string& text, int rotate)
    {
      ItemUtils itemUtils = this->getScene()->getItemUtils();

      std::vector<QString> textVect;
      std::vector<QFont> fontVect;

      QFont fontSScript2 = font;
      fontSScript2.setPointSize(fontSScript2.pointSize() / 2);

      fontVect.push_back(fontSScript2);
      fontVect.push_back(font);
      
      int index = (int)text.size() / 2;
      int indexNegative = (int)(text.size() - 1) / 2;

      std::string txtSubstr1 = text.substr(0, indexNegative + 1).c_str();
      QString qTxtSubstr1 = ItemUtils::convert2QString(txtSubstr1);

      std::string txtSubstr2 = text.substr(0, index).c_str();
      QString qTxtSubstr2 = ItemUtils::convert2QString(txtSubstr2);

      std::string txtSubstr3 = text.substr(indexNegative + 1, text.size()).c_str();
      QString qTxtSubstr3 = ItemUtils::convert2QString(txtSubstr3);

      std::string txtSubstr4 = text.substr(index, text.size()).c_str();
      QString qTxtSubstr4 = ItemUtils::convert2QString(txtSubstr4);

      QString txtSScript1(text.at(0) == '-' ? qTxtSubstr1 : qTxtSubstr2);
      QString txtSScript2(text.at(0) == '-' ? qTxtSubstr3 : qTxtSubstr4);

      textVect.push_back(txtSScript1);

      textVect.push_back(txtSScript2);


      QPainterPath textObject = itemUtils.superscriptTextToVector(textVect, fontVect, point, rotate);


      QPen pen = painter->pen();
      pen.setWidthF(0);

      QBrush brush = painter->brush();
      brush.setStyle(Qt::SolidPattern);


      painter->save();

      painter->setPen(pen);
      painter->setRenderHint(QPainter::Antialiasing, true);
      painter->fillPath(textObject, brush);

      painter->restore();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::drawImage(const QRectF& rect, QPainter* painter, const QImage& image)
    {
      painter->save();

      QTransform transform;
      transform.translate(0., rect.y() + rect.height());
      transform.scale(1., -1.);
      transform.translate(0., -rect.y());

      painter->setTransform(transform, true);
      painter->drawImage(rect, image, image.rect());

      painter->restore();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::drawPixmap(const QRectF& rect, QPainter* painter, const QPixmap& pixmap)
    {
      painter->save();

      QTransform transform;
      transform.translate(0., rect.y() + rect.height());
      transform.scale(1., -1.);
      transform.translate(0., -rect.y());

      painter->setTransform(transform, true);
      painter->drawPixmap(rect, pixmap, pixmap.rect());

      painter->restore();
    }

    template <class T>
    inline QVariant te::layout::AbstractItem<T>::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant & value)
    {
      if (change == QGraphicsItem::ItemPositionChange && m_currentAction != te::layout::MOVE_ACTION)
      {
        if (this->isInverted())
        {
          // value is the new position.
          QPointF newPos = value.toPointF();

          double tx = T::transform().dx();
          double ty = T::transform().dy();

          newPos.setX(newPos.x() - tx);
          newPos.setY(newPos.y() - ty);
          return newPos;
        }
      }
      else if (change == QGraphicsItem::ItemPositionHasChanged)
      {
        if (m_currentAction == te::layout::NO_ACTION)
        {
          m_controller->itemPositionChanged(T::pos().x(), T::pos().y());
        }
      }
      else if (change == QGraphicsItem::ItemZValueHasChanged)
      {
        m_controller->itemZValueChanged(T::zValue());
      }

      return T::itemChange(change, value);
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
    {
      if (isEditionMode() == false && m_controller->getProperty("resizable").getValue().toBool())
      {
        checkTouchesCorner(event->pos().x(), event->pos().y());
      }
      T::hoverMoveEvent(event);
    }

    template <class T>
    inline bool te::layout::AbstractItem<T>::checkTouchesCorner(const double& x, const double& y)
    {
      bool result = true;
      QRectF bRect = boundingRect();

      QRectF remapRect = qRectToQPolygonMap(bRect);

      QPointF ll = remapRect.bottomLeft();//bRect.bottomLeft();
      QPointF lr = remapRect.bottomRight();//bRect.bottomRight();
      QPointF tl = remapRect.topLeft();//bRect.topLeft();
      QPointF tr = remapRect.topRight();//bRect.topRight();

      QGraphicsScene* scene = this->scene();
      QGraphicsView* view = scene->views().first();

      QPointF mousePoint(x, y);

      QPointF convPoint = this->mapToScene(mousePoint);
      QPointF remapedPoint = view->mapFromScene(convPoint);



      Utils utils = this->getScene()->getUtils();
      double marginPixel =  utils.mm2pixel(m_marginResizePrecision);
      
      double w = marginPixel;
      double h = marginPixel;
      double half = marginPixel / 2.;

      QRectF smallLeftTopRect(remapRect.topLeft().x(), remapRect.topLeft().y() - half, w, h); // left-top
      QRectF smallRightTopRect(remapRect.topRight().x() - half, remapRect.topRight().y() - half, w, h); // right-top
      QRectF smallLeftBottomRect(remapRect.bottomLeft().x(), remapRect.bottomLeft().y() - half, w, h); // left-bottom
      QRectF smallRightBottomRect(remapRect.bottomRight().x() - half, remapRect.bottomRight().y() - half, w, h); // right-bottom

      QRectF leftRect(smallLeftTopRect.topLeft(), smallLeftBottomRect.bottomRight());
      QRectF rightRect(smallRightTopRect.topLeft(), smallRightBottomRect.bottomRight());
      QRectF topRect(smallLeftTopRect.topLeft(), smallRightTopRect.bottomRight());
      QRectF bottomRect(smallLeftBottomRect.topLeft(), smallRightBottomRect.bottomRight());



           
      QPointF checkPoint(remapedPoint.x(), remapedPoint.y());

      Property pKeepAspect = m_controller->getProperty("keep_aspect");

      bool keepAspect = pKeepAspect.getValue().toBool();

      if (smallLeftTopRect.contains(checkPoint))
      {
        T::setCursor(Qt::SizeFDiagCursor);
        m_enumSides = TPTopLeft;
      }
      else if (smallRightTopRect.contains(checkPoint))
      {
        T::setCursor(Qt::SizeBDiagCursor);
        m_enumSides = TPTopRight;
      }
      else if (smallLeftBottomRect.contains(checkPoint))
      {
        T::setCursor(Qt::SizeBDiagCursor);
        m_enumSides = TPLowerLeft;
      }
      else if (smallRightBottomRect.contains(checkPoint))
      {
        T::setCursor(Qt::SizeFDiagCursor);
        m_enumSides = TPLowerRight;
      }
      else if (keepAspect == false && leftRect.contains(checkPoint))
      {
        T::setCursor(Qt::SizeHorCursor);
        m_enumSides = TPLeft;
      }
      else if (keepAspect == false && rightRect.contains(checkPoint))
      {
        T::setCursor(Qt::SizeHorCursor);
        m_enumSides = TPRight;
      }
      else if (keepAspect == false && topRect.contains(checkPoint))
      {
        T::setCursor(Qt::SizeVerCursor);
        m_enumSides = TPTop;
      }
      else if (keepAspect == false && bottomRect.contains(checkPoint))
      {
        T::setCursor(Qt::SizeVerCursor);
        m_enumSides = TPLower;
      }
      else
      {
        T::setCursor(Qt::ArrowCursor);
        m_enumSides = TPNoneSide;
        result = false;
      }

      return result;
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::mousePressEvent(QGraphicsSceneMouseEvent * event)
    {
      //checks if the item is resizable.
      const Property& property = m_controller->getProperty("resizable");
      if (property.getValue().toBool() == true)
      {
        //If so, checks if the resize operation must be started
        bool startResizing = checkTouchesCorner(event->pos().x(), event->pos().y());
        if (startResizing == true)
        {
          m_currentAction = te::layout::RESIZE_ACTION;
          setPixmap();
          m_initialCoord = event->pos();      
          if (m_controller)
          {
            m_controller->beginResize();
          }
        }
      }

      T::mousePressEvent(event);
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
    {
      if (isEditionMode() == true)
      {
        return;
      }

      if (m_currentAction == te::layout::RESIZE_ACTION)
      {
        T::setOpacity(0.5);
        m_finalCoord = event->pos();
        T::prepareGeometryChange();
        m_rect = m_controller->calculateResize(m_enumSides, m_initialCoord, m_finalCoord);
      }
      else
      {
        if (event->buttons() == Qt::LeftButton)
        {
          if(m_currentAction != te::layout::MOVE_ACTION)
            setPixmap();
          T::setOpacity(0.5);
          m_currentAction = te::layout::MOVE_ACTION;
        }

        T::mouseMoveEvent(event);
      }
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
    {
      if (m_currentAction == te::layout::RESIZE_ACTION)
      {
        m_finalCoord = event->pos();
        m_rect = m_controller->resize(m_enumSides, m_initialCoord, m_finalCoord);
        m_rect.moveTo(0, 0);
        T::setOpacity(1.);
      }
      else if (m_currentAction == te::layout::MOVE_ACTION)
      {
        T::setOpacity(1.);
        m_controller->itemPositionChanged(T::pos().x(), T::pos().y());
      }

      m_currentAction = te::layout::NO_ACTION;

      T::mouseReleaseEvent(event);
    }
    
    template <class T>
    inline void te::layout::AbstractItem<T>::drawItemResized(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
    {
      painter->save();
      painter->setClipRect(boundingRect());
      painter->drawPixmap(boundingRect(), m_clonePixmap, m_clonePixmap.rect());
      painter->restore();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::setPixmap()
    {
      Utils utils = this->getScene()->getUtils();

      QRectF itemBounding = boundingRect();
      te::gm::Envelope box(0, 0, itemBounding.width(), itemBounding.height());
      box = utils.viewportBox(box);

      m_clonePixmap = QPixmap(box.getWidth(), box.getHeight());
      m_clonePixmap.fill(Qt::transparent);

      QPainter p(&m_clonePixmap);

      double resX = box.getWidth() / itemBounding.width();
      double resY = box.getHeight() / itemBounding.height();

      QTransform transform;
      transform.scale(resX, -resY);
      transform.translate(-itemBounding.bottomLeft().x(), -itemBounding.bottomLeft().y());
      p.setTransform(transform);

      QStyleOptionGraphicsItem opt;
      this->drawItem(&p, &opt, 0);

      if (!T::childItems().isEmpty())
      {
        drawSelection(&p);
      }

      p.end();

      QImage image = m_clonePixmap.toImage();
      image = image.mirrored();
      m_clonePixmap = QPixmap::fromImage(image);
    }
    
    template <class T>
    inline te::layout::AbstractScene* te::layout::AbstractItem<T>::getScene()
    {
      QGraphicsScene* scene = T::scene();
      if (scene == 0)
      {
        return 0;
      }

      AbstractScene* myScene = dynamic_cast<AbstractScene*>(scene);
      if (myScene == 0)
      {
        return 0;
      }
      return myScene;
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::prepareGeometryChange()
    {
      T::prepareGeometryChange();
    }

    template <class T>
    inline te::layout::ItemAction te::layout::AbstractItem<T>::getCurrentAction()
    {
      return m_currentAction;
    }
  } // end namespace layout
} // end namespace te

#endif //__TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ITEM_H
