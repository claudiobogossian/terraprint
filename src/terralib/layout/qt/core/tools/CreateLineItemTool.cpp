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
#include "CreateLineItemTool.h"
#include "../View.h"
#include "../Scene.h"
#include "../BuildGraphicsItem.h"
#include "../../item/LineItem.h"
#include "../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../core/enum/Enums.h"
#include "terralib/geometry/Point.h"

// STL
#include <string>

// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QUndoStack>

te::layout::CreateLineItemTool::CreateLineItemTool(View* view, EnumType* itemType, QObject* parent)
  : AbstractLayoutTool(view, parent)
  ,m_item(0)
  ,m_itemType(itemType)
{
  setCursor(Qt::ArrowCursor);

  if (!m_itemType)
  {
    m_itemType = Enums::getInstance().getEnumObjectType()->getLineItem();
  }
}

te::layout::CreateLineItemTool::~CreateLineItemTool()
{
}

bool te::layout::CreateLineItemTool::mousePressEvent(QMouseEvent* e)
{
  if (!m_view)
    return false;

  Scene* scne = dynamic_cast<Scene*>(m_view->scene());
  if (!scne)
    return false;

  //Roll back the default tool cursor
  m_view->viewport()->setCursor(m_cursor);

  if (e->button() != Qt::LeftButton)
  {
    scne->getUndoStack()->endMacro();
    m_coords.clear();
    m_view->resetDefaultConfig(true);
    return true;
  }

  createItem();

  QPointF scenePos = m_view->mapToScene(e->pos());
  te::gm::Point p(scenePos.x(), scenePos.y());
  m_coords.push_back(p);
  setGeometry(); // update properties with undo/redo
  m_view->viewport()->update();
  return true;
}

bool te::layout::CreateLineItemTool::mouseMoveEvent(QMouseEvent* e)
{
  return false;
}

bool te::layout::CreateLineItemTool::mouseReleaseEvent(QMouseEvent* e)
{
  return false;
}

bool te::layout::CreateLineItemTool::keyPressEvent( QKeyEvent* keyEvent )
{
  Scene* sc = dynamic_cast<Scene*>(m_view->scene());

  if(keyEvent->key() == Qt::Key_Escape)
  {
    if (sc)
    {
      sc->getUndoStack()->endMacro();
    }
    
    m_coords.clear();
    m_view->resetDefaultConfig(true);
  }
  return true;
}

// The best would be to have a method of drawing lines and only update the item with the geometry at the end
void te::layout::CreateLineItemTool::setGeometry()
{
  AbstractItem* itemView = dynamic_cast<AbstractItem*> (m_item);

  // create lineString only to find the lowerleft
  te::gm::LineString* lineString = new te::gm::LineString(m_coords.size(), te::gm::LineStringType);
  for (unsigned int i = 0; i < m_coords.size(); i++)
  {
    lineString->setPointN(i, m_coords[i]);
  }
  te::gm::Coord2D lowerLeft = lineString->getMBR()->getLowerLeft();

  // update position
  Properties properties = createProperties(lowerLeft);
  itemView->setProperties(properties);

  // Update position of points according to the new item position
  for (unsigned int i = 0; i < m_coords.size(); i++)
  {
    QPointF point(m_coords[i].getX(), m_coords[i].getY());
    QPointF point2Item = m_item->mapFromScene(point);
    lineString->setPointN(i, te::gm::Point(point2Item.x(), point2Item.y()));
  }
  
  // update geometry
  properties = createProperties(lineString);
  itemView->setProperties(properties);
}

te::layout::Properties te::layout::CreateLineItemTool::createProperties(te::gm::LineString* lineString)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  Properties properties;
  {
    Property property(0);
    property.setName("geometry");
    property.setValue<te::gm::Geometry*>(lineString, dataType->getDataTypeGeometry());
    properties.addProperty(property);
  }

  return properties;
}

te::layout::Properties te::layout::CreateLineItemTool::createProperties(const te::gm::Coord2D& coord)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  Properties properties;
  {
    Property property(0);
    property.setName("x");
    property.setValue(coord.getX(), dataType->getDataTypeDouble());
    properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("y");
    property.setValue(coord.getY(), dataType->getDataTypeDouble());
    properties.addProperty(property);
  }
  return properties;
}

void te::layout::CreateLineItemTool::createItem()
{
  Scene* sc = dynamic_cast<Scene*>(m_view->scene());
  if (!sc)
  {
    return;
  }

  if (m_coords.empty())
  {
    std::string label = m_itemType->getLabel();

    sc->getUndoStack()->beginMacro("Create line item or derived child");
    // create a new item
    BuildGraphicsItem buildItem(sc);
    m_item = buildItem.createItem(m_itemType);
  }
}

