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
  \file MapItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapItem.h"
#include "MapController.h"

#include "../core/ItemUtils.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../qt/core/Scene.h"
#include "terralib/qt/widgets/canvas/MapDisplay.h"
#include "terralib/qt/widgets/layer/explorer/TreeItem.h"
#include "terralib/qt/widgets/layer/explorer/LayerItem.h"
#include "terralib/qt/widgets/tools/Pan.h"
#include "terralib/qt/widgets/tools/ZoomWheel.h"
#include "terralib/qt/widgets/tools/Zoom.h"
#include "terralib/qt/widgets/tools/ZoomArea.h"
#include "terralib/qt/widgets/tools/ZoomClick.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QApplication>
#include <QEvent>
#include <QWheelEvent>
#include <QMouseEvent>

te::layout::MapItem::MapItem(AbstractItemController* controller, bool invertedMatrix)
  : AbstractItem<QGraphicsObject>(controller, invertedMatrix)
  , m_mapDisplay(0)
  , m_currentTool(0)
  , m_zoomWheel(0)
  , m_tileSize(2048)
{
  this->setAcceptDrops(true);

  m_mapDisplay = new te::qt::widgets::MapDisplay();
  m_mapDisplay->setAcceptDrops(true);
  m_mapDisplay->setBackgroundColor(Qt::transparent);
  m_mapDisplay->setResizeInterval(0);
  m_mapDisplay->setMouseTracking(true);

  this->prepareGeometryChange();
  m_mapDisplay->resize(10, 10);

  connect(m_mapDisplay, SIGNAL(extentChanged()), this, SLOT(extentChanged()));
}

te::layout::MapItem::~MapItem()
{
  delete m_mapDisplay;
}

te::qt::widgets::MapDisplay* te::layout::MapItem::getMapDisplay()
{
  return m_mapDisplay;
}

void te::layout::MapItem::contextUpdated(const ContextObject& context)
{
  int zoom = context.getZoom();
  ((MapController *) m_controller)->setZoom(zoom);

  //AbstractScene* scene = ((AbstractItem<QGraphicsItem>*) this)->getScene();
  Utils utils = ((Scene*) this->scene())->getUtils();

  QRectF boxMM = boundingRect();

  te::gm::Envelope box(0, 0, boxMM.width(), boxMM.height());
  box = utils.viewportBox(box);

  m_mapDisplay->setOverrideDPI(context.getDpiX(), context.getDpiY());

  QSize currentSize = m_mapDisplay->size();
  QSize newSize(qRound(box.getWidth()), qRound(box.getHeight()));
  if(currentSize != newSize)
  {
    this->prepareGeometryChange();
    m_mapDisplay->resize(newSize);
  }
}

void te::layout::MapItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  //due to problems in QT, which is generating lines between the tiles, we are temporarely disabling the tiles algorithm
  /*if (m_mapDisplay->getWidth() > (unsigned int)m_tileSize || m_mapDisplay->getHeight() > (unsigned int)m_tileSize)
  {
    drawTilesMap(painter);
    return;
  }*/


  QImage image(m_mapDisplay->width(), m_mapDisplay->height(), QImage::Format_ARGB32);
  image.fill(Qt::transparent);

  QPainter localPainter(&image);
  m_mapDisplay->render(&localPainter, QPoint(), QRegion(), QWidget::DrawChildren);
  localPainter.end();

  drawImage(this->getAdjustedBoundingRect(painter), painter, image);
}

QVariant te::layout::MapItem::itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value )
{
  if(change == QGraphicsItem::ItemSceneHasChanged)
  {
    Scene* myScene = dynamic_cast<Scene*>(this->scene());
    if(myScene != 0)
    {
      contextUpdated(myScene->getContext());
    }
  }
  return AbstractItem<QGraphicsObject>::itemChange(change, value);
}

void te::layout::MapItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
  if(m_isEditionMode == false)
  {
    AbstractItem<QGraphicsObject>::mousePressEvent(event);
    return;
  }
  
  QRectF rect = boundingRect();
  QPointF point = event->pos();
  QPointF remappedPoint = remapPointToViewport(point, rect, m_mapDisplay->rect());

  QMouseEvent mouseEvent(QEvent::MouseButtonPress, remappedPoint.toPoint(), event->button(),event->buttons(), event->modifiers());
  QApplication::sendEvent(m_mapDisplay, &mouseEvent);
  event->setAccepted(mouseEvent.isAccepted());

  refresh();
}

void  te::layout::MapItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
  if(m_isEditionMode == false)
  {
    AbstractItem<QGraphicsObject>::mouseMoveEvent(event);
    return;
  }

  QRectF rect = boundingRect();
  QPointF point = event->pos();
  QPointF remappedPoint = remapPointToViewport(point, rect, m_mapDisplay->rect());

  QMouseEvent mouseEvent(QEvent::MouseMove, remappedPoint.toPoint(), event->button(),event->buttons(), event->modifiers());
  QApplication::sendEvent(m_mapDisplay, &mouseEvent);
  event->setAccepted(mouseEvent.isAccepted());

  refresh();
}

void  te::layout::MapItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
  if(m_isEditionMode == false)
  {
    AbstractItem<QGraphicsObject>::mouseReleaseEvent(event);
    return;
  }

  QRectF rect = boundingRect();
  QPointF point = event->pos();
  QPointF remappedPoint = remapPointToViewport(point, rect, m_mapDisplay->rect());

  QMouseEvent mouseEvent(QEvent::MouseButtonRelease, remappedPoint.toPoint(), event->button(),event->buttons(), event->modifiers());
  QApplication::sendEvent(m_mapDisplay, &mouseEvent);
  event->setAccepted(mouseEvent.isAccepted());

  refresh();
}

void te::layout::MapItem::dragEnterEvent( QGraphicsSceneDragDropEvent * event )
{
  //Copy the map from layer tree
  Qt::DropActions actions = event->dropAction();
  if(!(actions & Qt::CopyAction))
    return;

  const QMimeData* mime = event->mimeData();
  QString s = mime->data("application/x-terralib;value=\"DraggedItems\"").constData();
  if(s.isEmpty())
    return;

  event->acceptProposedAction();
}

void te::layout::MapItem::dragLeaveEvent( QGraphicsSceneDragDropEvent * event )
{
  AbstractItem<QGraphicsObject>::dragLeaveEvent(event);
}

void te::layout::MapItem::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{
  AbstractItem<QGraphicsObject>::dragMoveEvent(event);
}

void te::layout::MapItem::dropEvent( QGraphicsSceneDragDropEvent * event )
{
  event->setDropAction(Qt::CopyAction);

  QByteArray encodedData = event->mimeData()->data("application/x-terralib;value=\"DraggedItems\"");
  qulonglong dataValue = encodedData.toULongLong();

  std::auto_ptr< std::vector<te::qt::widgets::TreeItem*> > vecItems(reinterpret_cast< std::vector<te::qt::widgets::TreeItem*>* >(dataValue));

  std::list<te::map::AbstractLayerPtr> listLayers;
  for(size_t i = 0 ; i < vecItems->size(); ++i)
  {
    te::qt::widgets::TreeItem* item = vecItems->operator[](i);
    if(item->getType() != "LAYER")
    {
      continue;
    }

    te::qt::widgets::LayerItem* layerItem = dynamic_cast<te::qt::widgets::LayerItem*>(item);
    if(layerItem == 0)
    {
      continue;
    }

    te::map::AbstractLayerPtr layer = layerItem->getLayer();
    listLayers.push_back(layer);
  }

  MapController* mapController = dynamic_cast<MapController*>(m_controller);
  if(mapController != 0)
  {
    mapController->addLayers(listLayers);
  }
}

void te::layout::MapItem::wheelEvent ( QGraphicsSceneWheelEvent * event )
{
  if(m_isEditionMode == false)
  {
    AbstractItem<QGraphicsObject>::wheelEvent(event);
    return;
  }

  QRectF rect = boundingRect();
  QPointF point = event->pos();
  QPointF remappedPoint = remapPointToViewport(point, rect, m_mapDisplay->rect());

  QWheelEvent wheelEvent(remappedPoint.toPoint(), event->delta(),event->buttons(), event->modifiers());
  QApplication::sendEvent(m_mapDisplay, &wheelEvent);
  event->setAccepted(wheelEvent.isAccepted());

  QGraphicsItem::update();
}

void te::layout::MapItem::enterEditionMode()
{
  AbstractItem<QGraphicsObject>::enterEditionMode();

  //we now install the visualization tools in the map display and forward all the mouse and keyboards events to it
  if(m_zoomWheel == 0)
  {
    m_zoomWheel = new te::qt::widgets::ZoomWheel(m_mapDisplay, 1.25, false);
    this->setCursor(Qt::OpenHandCursor);
  }

  m_mapDisplay->installEventFilter(m_zoomWheel);
  
  if(parentItem() != 0)
  {
    parentItem()->setHandlesChildEvents(false);
  }
}

void te::layout::MapItem::leaveEditionMode()
{
  AbstractItem<QGraphicsObject>::leaveEditionMode();

  //we now uninstall the visualization tools from the map display and no more events will be forward to it
  if(m_zoomWheel != 0)
  {
    m_mapDisplay->removeEventFilter(m_zoomWheel);
    delete m_zoomWheel;
    m_zoomWheel = 0;
  }

  this->setCursor(Qt::ArrowCursor);
}

QPointF te::layout::MapItem::remapPointToViewport(const QPointF& point, const QRectF& item, const QRectF& widget) const
{
  double resX = widget.width() / item.width();
  double resY = widget.height() / item.height();

  QMatrix matrix;
  matrix.scale(resX, -resY);
  matrix.translate(-item.bottomLeft().x(), -item.bottomLeft().y());

  QPointF remappedPoint = matrix.map(point);
  return remappedPoint;
}

void te::layout::MapItem::extentChanged()
{
  MapController* mapController = dynamic_cast<MapController*>(m_controller);
  if(mapController != 0)
  {
    mapController->extentChanged(m_mapDisplay->getExtent(), m_mapDisplay->getScale(), m_mapDisplay->getSRID());
  }
}

void te::layout::MapItem::resized()
{
  Scene* myScene = dynamic_cast<Scene*>(this->scene());
  if(myScene != 0)
  {
    contextUpdated(myScene->getContext());
  }
}

void te::layout::MapItem::doRefresh()
{
  m_mapDisplay->refresh();
}

void te::layout::MapItem::drawTilesMap(QPainter* painter)
{
  if (!painter)
  {
    return;
  }

  Scene* myScene = dynamic_cast<Scene*>(scene());
  if (myScene == 0)
  {
    return;
  }

  const ContextObject& context = myScene->getContext();

  
  int numTilesX = (int)std::ceil(m_mapDisplay->getWidth() / (double)m_tileSize);
  int numTilesY = (int)std::ceil(m_mapDisplay->getHeight() / (double)m_tileSize);

  int tileWidth = (int) std::ceil(float (m_mapDisplay->getWidth() / (double)numTilesX));
  int tileHeight = (int) std::ceil(float (m_mapDisplay->getHeight() / (double)numTilesY));
  if (tileWidth > tileHeight)
  {
    tileHeight = tileWidth;
  }
  else
  {
    tileWidth = tileHeight;
  }

  double worldTileWidth = m_mapDisplay->getExtent().getWidth() / (double)numTilesX;
  double worldTileHeight = m_mapDisplay->getExtent().getHeight() / (double)numTilesY;
  if (worldTileWidth > worldTileHeight)
  {
    worldTileHeight = worldTileWidth;
  }
  else
  {
    worldTileWidth = worldTileHeight;
  }

  QRectF boundRect = boundingRect();

  double boundingTileWidth = boundRect.width() / (double)numTilesX;
  double boundingTileHeight = boundRect.height() / (double)numTilesY;
  if (boundingTileWidth > boundingTileHeight)
  {
    boundingTileHeight = boundingTileWidth;
  }
  else
  {
    boundingTileWidth = boundingTileHeight;
  }
  
  double extentWidth = m_mapDisplay->getExtent().getLowerLeftX();
  double extendHeight = m_mapDisplay->getExtent().getLowerLeftY();
    
  //we read the current layer list from the model because mapDisplay does not have the getLayers function
  const Property& property = m_controller->getProperty("layers");
  const std::list<te::map::AbstractLayerPtr>& currentLayerList = property.getValue().toLayerList();

  painter->save();

  painter->setClipRect(boundRect);
  
  //first draws all horizontally to each increment vertically
  //as the current CS orientation is botton-top, we must invert the order that the tiles are drawn
  for (int j = numTilesY-1; j >= 0 ; --j) // row
  {
    for (int i = 0; i < numTilesX; ++i) // column
    { 
      //New map display
      te::qt::widgets::MapDisplay* tileMapDisplay = new te::qt::widgets::MapDisplay();
      tileMapDisplay->setBackgroundColor(Qt::transparent);
      tileMapDisplay->setResizeInterval(0);

      tileMapDisplay->setLayerList(currentLayerList);
      tileMapDisplay->setSRID(m_mapDisplay->getSRID(), false);

      double baseWorldX = extentWidth + (i * worldTileWidth);
      double baseWorldY = extendHeight + (j * worldTileHeight);

      te::gm::Envelope currentWorldBox(baseWorldX, baseWorldY, baseWorldX + worldTileWidth, baseWorldY + worldTileHeight);

      tileMapDisplay->setExtent(currentWorldBox, false);
      tileMapDisplay->resize(tileWidth, tileHeight); // resize to tile size
      
      QImage image(tileMapDisplay->width(), tileMapDisplay->height(), QImage::Format_ARGB32);
      image.fill(Qt::transparent);
      image.setDotsPerMeterX(context.getDpiX() / 25.4 * 1000);
      image.setDotsPerMeterY(context.getDpiY() / 25.4 * 1000);

      QPainter localPainter(&image);
      tileMapDisplay->render(&localPainter, QPoint(), QRegion(), QWidget::DrawChildren);
      localPainter.end();

      double baseBoundingX = i * boundingTileWidth;
      double baseBoundingY = j * boundingTileHeight;
      
      QRectF currentBoundingBox(baseBoundingX, baseBoundingY, boundingTileWidth, boundingTileHeight);
     
      drawImage(currentBoundingBox, painter, image);

      if (tileMapDisplay)
      {
        delete tileMapDisplay;
        tileMapDisplay = 0;
      }
    }
  }

  painter->restore();
}

bool te::layout::MapItem::sceneEventFilter(QGraphicsItem * watched, QEvent * event)
{
  AbstractItem<QGraphicsObject>::sceneEventFilter(watched, event);
  return true;
}

bool te::layout::MapItem::changeCurrentTool(EnumType* tool)
{
  EnumModeType* mode = Enums::getInstance().getEnumModeType();

  if (m_currentTool)
  {
    removeCurrentTool();
  }

  QCursor toolCursor;

  //we now install the visualization tools in the map display and forward all the mouse and keyboards events to it
  if (tool == mode->getModeMapPan())
  {
    toolCursor = createCursor("layout-map-pan");
    m_currentTool = new te::qt::widgets::Pan(m_mapDisplay, toolCursor, toolCursor);
  }

  if (tool == mode->getModeMapZoomIn())
  {
    toolCursor = createCursor("layout-map-zoom-in");
    m_currentTool = new te::qt::widgets::ZoomArea(m_mapDisplay, toolCursor);
  }

  if (tool == mode->getModeMapZoomOut())
  {
    toolCursor = createCursor("layout-map-zoom-out");
    m_currentTool = new te::qt::widgets::ZoomClick(m_mapDisplay, toolCursor, 2.0, te::qt::widgets::Zoom::Out);
  }

  if (m_currentTool)
  {
    m_currentTool->setCursor(toolCursor);
    m_mapDisplay->installEventFilter(m_currentTool);
    this->setCursor(toolCursor);
  }

  if (parentItem() != 0)
  {
    parentItem()->setHandlesChildEvents(false);
  }
  return false;
}

bool te::layout::MapItem::removeCurrentTool()
{
  if (m_currentTool)
  {
    m_mapDisplay->removeEventFilter(m_currentTool);

    delete m_currentTool;
    m_currentTool = 0;

    this->setCursor(Qt::ArrowCursor);
    
    return true;
  }
  return false;
}

QCursor te::layout::MapItem::createCursor(std::string pathIcon)
{
  QIcon ico(QIcon::fromTheme(pathIcon.c_str()));

  //search icon size
  QList<QSize> sizes = ico.availableSizes();
  int maximum = sizes[0].width();
  for (int i = 1; i < sizes.size(); ++i)
  {
    maximum = qMax(maximum, sizes[i].width());
  }

  QSize sz(maximum, maximum);
  QPixmap pixmap = ico.pixmap(sz);

  QCursor cur(pixmap);

  return cur;
}


