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

// TerraLib
#include "CreateItemTool.h"

#include "../../../core/enum/EnumDataType.h"
#include "../../../core/enum/Enums.h"
#include "../View.h"
#include "../Scene.h"
#include "../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../core/pattern/mvc/AbstractItemController.h"
#include "../BuildGraphicsItem.h"
#include "../../../core/property/SharedProperties.h"
#include "../../item/MapItem.h"
#include "../../../core/enum/Enums.h"

// Qt
#include <QtGui/QMouseEvent>

te::layout::CreateItemTool::CreateItemTool(View* view, EnumType* itemType, QObject* parent)
  : RubberBandTool(view, parent),
  m_itemType(itemType)
{
  setCursor(Qt::ArrowCursor);
}

te::layout::CreateItemTool::~CreateItemTool()
{
}

bool te::layout::CreateItemTool::mousePressEvent(QMouseEvent* e)
{
  if (!m_itemType)
    return false;

  if (e->button() != Qt::LeftButton)
    return false;

  m_started = true;
  m_rect = QRectF();

  return RubberBandTool::mousePressEvent(e);
}

bool te::layout::CreateItemTool::mouseMoveEvent(QMouseEvent* e)
{
  if (!m_itemType)
    return false;

  if (!m_started)
    return false;

  return RubberBandTool::mouseMoveEvent(e);
}

bool te::layout::CreateItemTool::mouseReleaseEvent(QMouseEvent* e)
{
  if (!m_itemType)
    return false;

  m_started = false;

  if (!m_view)
    return false;

  //Roll back the default tool cursor
  m_view->viewport()->setCursor(m_cursor);

  if (e->button() != Qt::LeftButton)
    return false;

  Scene* sc = dynamic_cast<Scene*>(m_view->scene());
  if (!sc)
    return false;

  View* view = dynamic_cast<View*>(m_view);
  if (!view)
    return false;

  if (m_rubberBand == 0)
  {
    return false;
  }

  QRect rect = m_rubberBand->geometry().normalized();

  RubberBandTool::mouseReleaseEvent(e);

  //Conversion to world coordinates
  QPolygonF poly = m_view->mapToScene(rect);

  QRectF bounding = poly.boundingRect();
  
  // initial point
  te::gm::Coord2D coord(bounding.x(), bounding.y());

  double width = bounding.width();
  double height = bounding.height();

  if (width == 0. && height == 0.)
  {
    QPointF scenePos = m_view->mapToScene(m_origin);
    coord.x = scenePos.x();
    coord.y = scenePos.y();
  }

  QSize size = verifyToleranceSize(width, height);

  // create a new item
  BuildGraphicsItem buildItem(sc);
  QGraphicsItem* item = 0;

  if (m_properties.getProperties().size() > 0)
  {
    Properties props = m_properties;
    addProperties(props, size, coord);
    item = buildItem.buildItem(props);
  }
  else
  {
    item = buildItem.createItem(m_itemType, coord, size.width(), size.height());
  }

  if (!item)
    return false;

  connectItemWithLastMapItem(item, sc);

  m_view->resetDefaultConfig(true);

  return true;
}

void te::layout::CreateItemTool::connectItemWithLastMapItem(QGraphicsItem* item, Scene* sc)
{
  SharedProperties sharedProps;
  AbstractItemView* itemView = dynamic_cast<AbstractItemView*> (item);
  if(itemView->getProperties().contains(sharedProps.getItemObserver()) == false)
  {
    return;
  }

  std::string itemName = "";
  ItemUtils utils(sc);
  std::vector<MapItem*> mapList = utils.getMapItemList();
  for (size_t i = 0; i < mapList.size(); ++i)
  {
    MapItem* mapItem = mapList[i];
    if (mapItem == item)
    {
      continue;
    }

    itemName = te::layout::Property::GetValueAs<std::string>(mapItem->getProperty("name"));

    //we do not break in order to find the last Map on the list
  }

  if (itemName.empty() == true)
  {
    return;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  Property property(0);
  property.setName(sharedProps.getItemObserver());
  property.setValue(itemName, dataType->getDataTypeItemObserver());
  itemView->setProperty(property);
}

void te::layout::CreateItemTool::setProperties(const te::layout::Properties& properties)
{
  m_properties = properties;
}

te::layout::Properties te::layout::CreateItemTool::getProperties()
{
  return m_properties;
}

void te::layout::CreateItemTool::reset()
{
  m_properties.clear();
}

void te::layout::CreateItemTool::addProperties(te::layout::Properties& properties, const QSize& size, const te::gm::Coord2D& coord)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  QSize newSize = size;
  te::gm::Coord2D newCoord = coord;

  if(!properties.contains("x"))
  {
    Property property(0);
    property.setName("x");
    property.setValue(newCoord.getX(), dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  if (!properties.contains("y"))
  {
    Property property(0);
    property.setName("y");
    property.setValue(newCoord.getY(), dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  if (!properties.contains("width"))
  {
    Property property(0);
    property.setName("width");
    property.setValue(double(newSize.width()), dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  if (!properties.contains("height"))
  {
    Property property(0);
    property.setName("height");
    property.setValue(double(newSize.height()), dataType->getDataTypeDouble());
    properties.addProperty(property);
  }
}
