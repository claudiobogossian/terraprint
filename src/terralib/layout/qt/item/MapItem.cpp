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
#include "../../core/WorldTransformer.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../qt/core/Scene.h"
#include <terralib/qt/widgets/layer/explorer/TreeItem.h>
#include <terralib/qt/widgets/layer/explorer/LayerItem.h>
#include <terralib/maptools/Utils.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QApplication>
#include <QEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPaintEngine>

te::layout::MapItem::MapItem(AbstractItemController* controller)
  : AbstractItem<QGraphicsObject>(controller, false)
  , m_currentEditionMode(0)
  , m_isPrinting(false)
  , m_useQImage(false)
{
  this->setAcceptDrops(true);
}

te::layout::MapItem::~MapItem()
{
}

void te::layout::MapItem::contextUpdated(const ContextObject& context)
{
  EnumModeType* enumMode = Enums::getInstance().getEnumModeType();

  m_isPrinting = false;
  m_useQImage = false;
  if (context.getCurrentMode() == enumMode->getModePrinterPreview())
  {
    m_isPrinting = true;
    return;
  }
  else if (context.getCurrentMode() == enumMode->getModePrinter())
  {
    m_isPrinting = true;
    m_useQImage = true;
    return;
  }

  this->update();
}

void te::layout::MapItem::drawItem(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
  Scene* myScene = dynamic_cast<Scene*>(this->scene());
  if (myScene == 0)
  {
    return;
  }

  const Property& property = m_controller->getProperty("background_color");
  const te::color::RGBAColor& color = property.getValue().toColor();
  QColor qColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

  //we first calculate the size in pixels
  Utils utils = ((Scene*) this->scene())->getUtils();

  QRectF boxMM = boundingRect();
  te::gm::Envelope boxViewport(0, 0, boxMM.width(), boxMM.height());
  boxViewport = utils.viewportBox(boxViewport);

  if (m_isPrinting == true)
  {
    drawMapOnPainter(painter);
  }
  else
  {
    //if for any reason the size has been changed, we recreate the screen pixmap
    QSize sizeInPixels(qRound(boxViewport.getWidth()), qRound(boxViewport.getHeight()));
    if (m_screenCache.size() != sizeInPixels)
    {
      m_screenCache = QPixmap(sizeInPixels);
      m_screenCache.fill(qColor); //this is done to solve a printing problem. For some reason, the transparency is not being considered by the printer in Linux

      drawMapOnDevice(&m_screenCache);
    }
    
    drawPixmap(this->getAdjustedBoundingRect(painter), painter, m_screenCache);
    if (m_screenDraft.isNull() == false)
    {
      drawPixmap(this->getAdjustedBoundingRect(painter), painter, m_screenDraft);
    }
  }
}

void te::layout::MapItem::drawMapOnDevice(QPaintDevice* device)
{
  const Property& pSrid = m_controller->getProperty("srid");
  const Property& pWorldBox = m_controller->getProperty("world_box");
  const Property& pScale = m_controller->getProperty("scale");
  const Property& property = m_controller->getProperty("background_color");

  int srid = pSrid.getValue().toInt();;
  const te::gm::Envelope& envelope = pWorldBox.getValue().toEnvelope();
  double scale = pScale.getValue().toDouble();
  const te::color::RGBAColor& color = property.getValue().toColor();

  //here we render the layers on the given device
  te::qt::widgets::Canvas canvas(device);
  canvas.setBackgroundColor(color);
  canvas.setWindow(envelope.m_llx, envelope.m_lly, envelope.m_urx, envelope.m_ury);
  canvas.clear();

  const Property& pLayerList = m_controller->getProperty("layers");
  const std::list<te::map::AbstractLayerPtr>& layerList = pLayerList.getValue().toLayerList();

  std::list<te::map::AbstractLayerPtr>::const_reverse_iterator it;
  for (it = layerList.rbegin(); it != layerList.rend(); ++it) // for each layer
  {
    it->get()->draw(&canvas, envelope, srid, scale);
  }
}

void te::layout::MapItem::drawMapOnPainter(QPainter* painter)
{
  const Property& pSrid = m_controller->getProperty("srid");
  const Property& pWorldBox = m_controller->getProperty("world_box");
  const Property& pScale = m_controller->getProperty("scale");
  const Property& property = m_controller->getProperty("background_color");

  int srid = pSrid.getValue().toInt();;
  const te::gm::Envelope& envelope = pWorldBox.getValue().toEnvelope();
  double scale = pScale.getValue().toDouble();
  const te::color::RGBAColor& color = property.getValue().toColor();

  //here we render the layers on the given device
  painter->save();
  painter->setClipRect(this->getAdjustedBoundingRect(painter));

  int fullDeviceWidth = painter->device()->width();
  int fullDeviceHeight = painter->device()->height();

  //as canvas will transform from World CS to Device CS, we must add a transform in order to make the painter expect drawings in Device CS
  Scene* myScene = dynamic_cast<Scene*>(this->scene());
  Utils utils(myScene, 0);

  QRectF qBoundingRect = boundingRect();
  int deviceWidth = utils.mm2pixel(qBoundingRect.width());
  int deviceHeight = utils.mm2pixel(qBoundingRect.height());

  painter->setViewport(0, 0, deviceWidth, deviceHeight);

  double xScale = qBoundingRect.width() / deviceWidth;
  double yScale = qBoundingRect.height() / deviceHeight;

  QTransform transform;
  transform.translate(0, qBoundingRect.height());
  transform.scale(xScale, -yScale);
  
  
  painter->setTransform(transform, true);

  //then we create the canvas and initialize it
  te::qt::widgets::Canvas canvas(painter);
  canvas.setBackgroundColor(color);
  canvas.setWindow(envelope.m_llx, envelope.m_lly, envelope.m_urx, envelope.m_ury);

  const Property& pLayerList = m_controller->getProperty("layers");
  const std::list<te::map::AbstractLayerPtr>& layerList = pLayerList.getValue().toLayerList();

  std::list<te::map::AbstractLayerPtr>::const_reverse_iterator it;
  for (it = layerList.rbegin(); it != layerList.rend(); ++it) // for each layer
  {
    it->get()->draw(&canvas, envelope, srid, scale, 0);
  }
  painter->restore();
}

QVariant te::layout::MapItem::itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value )
{
  if(change == QGraphicsItem::ItemSceneHasChanged)
  {
    Scene* myScene = dynamic_cast<Scene*>(this->scene());
    if(myScene != 0)
    {
      //contextUpdated(myScene->getContext());

      //we dont want AbstractItem to handle this event, so we 'jump' to its father
      return QGraphicsObject::itemChange(change, value);
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

  m_clickedPointMM = te::gm::Point(event->pos().x(), event->pos().y());
}

void  te::layout::MapItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
  if(m_isEditionMode == false)
  {
    AbstractItem<QGraphicsObject>::mouseMoveEvent(event);
    return;
  }

  te::gm::Coord2D currentCoordMM(event->pos().x(), event->pos().y());
  te::gm::Coord2D clickedCoordMM(m_clickedPointMM.getX(), m_clickedPointMM.getY());

  te::gm::Envelope currentBoxMM(0, 0, boundingRect().width(), boundingRect().height());
  te::gm::Envelope currentBoxPx(0, 0, m_screenCache.width(), m_screenCache.height());

  //sets the transformer from MM to Pixel
  te::layout::WorldTransformer transformer(currentBoxMM, currentBoxPx);
  transformer.setMirroring(true);

  te::gm::Coord2D currentCoordPx = transformer.system1Tosystem2(currentCoordMM);
  te::gm::Coord2D clickedCoordPx = transformer.system1Tosystem2(clickedCoordMM);

  EnumModeType* mode = Enums::getInstance().getEnumModeType();
  if (m_currentEditionMode == mode->getModeMapPan())
  {
    if (m_clickedPointMM.getX() == TE_DOUBLE_NOT_A_NUMBER || m_clickedPointMM.getY() == TE_DOUBLE_NOT_A_NUMBER)
    {
      return;
    }

    double dx = currentCoordPx.getX() - clickedCoordPx.getX();
    double dy = currentCoordPx.getY() - clickedCoordPx.getY();

    QRect rect(0, 0, m_screenCache.width(), m_screenCache.height());
    rect.translate(dx, dy);

    QPixmap& draft = getDraftPixmap();
    draft.fill(Qt::transparent);

    QPainter painter(&draft);
    painter.setOpacity(0.3); // Adjusting transparency feedback.
    painter.drawPixmap(rect, m_screenCache);
    update();
  }
  else if (m_currentEditionMode == mode->getModeMapZoomIn())
  {
    QPoint qCurrentPoint(currentCoordPx.getX(), currentCoordPx.getY());
    QPoint qClickedPoint(clickedCoordPx.getX(), clickedCoordPx.getY());

    QRect rect = QRect(qClickedPoint, qCurrentPoint).normalized();

    QPixmap& draft = getDraftPixmap();
    draft.fill(Qt::transparent);

    // Setups the rubber band style
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(100, 177, 216));
    pen.setWidth(2);
    QBrush brush;
    brush = QColor(100, 177, 216, 80);

    QPainter painter(&draft);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(rect);

    update();
  }
}

void  te::layout::MapItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
  if(m_isEditionMode == false)
  {
    AbstractItem<QGraphicsObject>::mouseReleaseEvent(event);
    m_clickedPointMM = te::gm::Point();
    return;
  }

  MapController* mapController = dynamic_cast<MapController*>(m_controller);
  if (mapController == 0)
  {
    m_clickedPointMM = te::gm::Point();
    return;
  }

  const Property& pWorldBox = m_controller->getProperty("world_box");
  const te::gm::Envelope& worldBox = pWorldBox.getValue().toEnvelope();

  te::gm::Coord2D m_clickedCoordMM(m_clickedPointMM.getX(), m_clickedPointMM.getY());
  te::gm::Coord2D currentCoordMM(event->pos().x(), event->pos().y());

  te::gm::Envelope currentBoxMM(0, 0, boundingRect().width(), boundingRect().height());

  //sets the transformer from MM to World
  te::layout::WorldTransformer transformer(currentBoxMM, worldBox);
  transformer.setMirroring(false);
  te::gm::Coord2D currentCoordWorld = transformer.system1Tosystem2(currentCoordMM);
  te::gm::Coord2D clickedCoordWorld = transformer.system1Tosystem2(m_clickedCoordMM);

  te::gm::Point currentPointWorld(currentCoordWorld.getX(), currentCoordWorld.getY());
  te::gm::Point clickedPointWorld(clickedCoordWorld.getX(), clickedCoordWorld.getY());

  EnumModeType* mode = Enums::getInstance().getEnumModeType();
  if (m_currentEditionMode == mode->getModeMapZoomIn())
  {
    if (currentPointWorld.equals(&clickedPointWorld) == true)
    {
      mapController->zoom(currentPointWorld, true);
    }
    else
    {
      double x1 = std::min(currentPointWorld.getX(), clickedPointWorld.getX());
      double y1 = std::min(currentPointWorld.getY(), clickedPointWorld.getY());
      double x2 = std::max(currentPointWorld.getX(), clickedPointWorld.getX());
      double y2 = std::max(currentPointWorld.getY(), clickedPointWorld.getY());
      te::gm::Envelope box(x1, y1, x2, y2);
      mapController->zoom(box);
    }
  }
  else if (m_currentEditionMode == mode->getModeMapZoomOut())
  {
    mapController->zoom(currentPointWorld, false);
  }
  else if (m_currentEditionMode == mode->getModeMapPan())
  {
    if (m_clickedPointMM.getX() == TE_DOUBLE_NOT_A_NUMBER || m_clickedPointMM.getY() == TE_DOUBLE_NOT_A_NUMBER)
    {
      m_clickedPointMM = te::gm::Point();
      return;
    }

    double dx = currentCoordWorld.getX() - clickedCoordWorld.getX();
    double dy = currentCoordWorld.getY() - clickedCoordWorld.getY();

    mapController->pan(dx, dy);
  }

  m_screenDraft = QPixmap();
  m_clickedPointMM = te::gm::Point();
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

  MapController* mapController = dynamic_cast<MapController*>(m_controller);
  if (mapController == 0)
  {
    return;
  }

  bool zoomIn = true;
  if (event->delta() < 0)
  {
    zoomIn = false;
  }

  const Property& pWorldBox = m_controller->getProperty("world_box");
  const te::gm::Envelope& worldBox = pWorldBox.getValue().toEnvelope();

  te::gm::Coord2D coordMM(event->pos().x(), event->pos().y());
  te::gm::Envelope boxMM(0, 0, boundingRect().width(), boundingRect().height());

  //sets the transformer from MM to World
  te::layout::WorldTransformer transformer(boxMM, worldBox);
  transformer.setMirroring(false);

  te::gm::Coord2D coordWorld = transformer.system1Tosystem2(coordMM);

  te::gm::Point pointWorld(coordWorld.getX(), coordWorld.getY());
  mapController->zoom(pointWorld, zoomIn);
}

void te::layout::MapItem::enterEditionMode()
{
  AbstractItem<QGraphicsObject>::enterEditionMode();

  if(parentItem() != 0)
  {
    parentItem()->setHandlesChildEvents(false);
  }

  EnumModeType* mode = Enums::getInstance().getEnumModeType();
  changeCurrentTool(mode->getModeMapPan());
}

void te::layout::MapItem::leaveEditionMode()
{
  AbstractItem<QGraphicsObject>::leaveEditionMode();

  //we now uninstall the visualization tools from the map display and no more events will be forward to it
  removeCurrentTool();
}

void te::layout::MapItem::doRefresh()
{
  m_screenCache = QPixmap();
}

bool te::layout::MapItem::changeCurrentTool(EnumType* tool)
{
  //here we define the current edition mode, and each mode will now handle the mouse/keyboard events
  //this modes are used in the edition mode

  const Property& pFixedScale = m_controller->getProperty("fixed_scale");
  bool fixedScale = pFixedScale.getValue().toBool();

  EnumModeType* mode = Enums::getInstance().getEnumModeType();

  removeCurrentTool();

  QCursor toolCursor;
  m_currentEditionMode = mode->getModeNone();

  //If the fixed scale is TRUE, we must ensure that the scale will not be changed by the tools
  //For this reason, only the pan tool is allowed
  bool result = false;
  if (tool == mode->getModeMapPan())
  {
    m_currentEditionMode = tool;
    toolCursor = createCursor("layout-map-pan");
    result = true;
  }

  if (fixedScale == false)
  {
    if (tool == mode->getModeMapZoomIn())
    {
      m_currentEditionMode = tool;
      toolCursor = createCursor("layout-map-zoom-in");
      result = true;
    }
    else if (tool == mode->getModeMapZoomOut())
    {
      m_currentEditionMode = tool;
      toolCursor = createCursor("layout-map-zoom-out");
      result = true;
    }
    else if (tool == mode->getModeMapRecompose())
    {
      recompose();
      result = true;
    }
  }

  if (result == false)
  {
    return false;
  }

  this->setCursor(toolCursor);

  if (parentItem() != 0)
  {
    parentItem()->setHandlesChildEvents(false);
  }

  return true;
}

bool te::layout::MapItem::removeCurrentTool()
{
  EnumModeType* mode = Enums::getInstance().getEnumModeType();
  
  m_currentEditionMode = mode->getModeNone();
  this->setCursor(Qt::ArrowCursor);

  m_screenDraft = QPixmap();

  return true;
}

QCursor te::layout::MapItem::createCursor(std::string pathIcon)
{
  QString qPathIcon = ItemUtils::convert2QString(pathIcon);

  QIcon ico(QIcon::fromTheme(qPathIcon));

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

QPixmap& te::layout::MapItem::getDraftPixmap()
{
  //we first calculate the size in pixels
  Utils utils = ((Scene*) this->scene())->getUtils();

  QRectF boxMM = boundingRect();
  te::gm::Envelope boxViewport(0, 0, boxMM.width(), boxMM.height());
  boxViewport = utils.viewportBox(boxViewport);

  QSize sizeInPixels(qRound(boxViewport.getWidth()), qRound(boxViewport.getHeight()));

  if (m_screenDraft.size() != sizeInPixels)
  {
    m_screenDraft = QPixmap(sizeInPixels);
  }

  return m_screenDraft;
}


void te::layout::MapItem::redraw()
{
  doRefresh();
  update();
}

void te::layout::MapItem::recompose()
{
  if (m_controller == 0)
  {
    return;
  }

  MapController* mapController = dynamic_cast<MapController*>(m_controller);
  if (mapController == 0)
  {
    return;
  }

  mapController->recomposeExtent();
}
