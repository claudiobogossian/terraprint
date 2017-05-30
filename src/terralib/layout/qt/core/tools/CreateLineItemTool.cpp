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
#include "../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../core/enum/Enums.h"
#include "../../item/LineItem.h"
#include "../View.h"
#include "../Scene.h"
#include "../RenderGeometries.h"
#include "../BuildGraphicsItem.h"
#include "../pattern/command/AddCoordCommand.h"

// STL
#include <string>

// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QUndoStack>
#include <QPainterPath>
#include <QPixmap>
#include <QPointF>
#include <QPolygonF>
#include <QPen>
#include <QBrush>

te::layout::CreateLineItemTool::CreateLineItemTool(View* view, EnumType* itemType, QObject* parent)
  : AbstractLayoutTool(view, parent)
  , m_item(0)
  , m_itemType(itemType)
{
  setCursor(Qt::ArrowCursor);

  if (!m_itemType)
  {
    m_itemType = Enums::getInstance().getEnumObjectType()->getLineItem();
  }
  
  m_render = new RenderGeometries(view, te::layout::DRAW_LINE);

  init();
}

te::layout::CreateLineItemTool::~CreateLineItemTool()
{
  if (m_render)
  {
    delete m_render;
    m_render = 0;
  }
}

void te::layout::CreateLineItemTool::init()
{
  // Setups the draft style
  QPen pen;
  pen.setStyle(Qt::SolidLine);
  pen.setColor(QColor(100, 177, 216));
  m_render->setPen(pen);

  QBrush brush;
  brush.setColor(QColor(100, 177, 216));
  m_render->setBrush(brush);
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
    finalizeCreation();
    return true;
  }
  
  QPointF scenePos = m_view->mapToScene(e->pos());
  te::gm::Point p(scenePos.x(), scenePos.y());

  std::vector<te::gm::Point> oldCoords(m_coords);

  m_coords.push_back(p);

  // Create Undo/Redo on Stack
  QUndoCommand* command = new AddCoordCommand(m_view, oldCoords, m_coords);
  scne->addUndoStack(command);

  return true;
}

bool te::layout::CreateLineItemTool::mouseMoveEvent(QMouseEvent* e)
{
  return false;
}

bool te::layout::CreateLineItemTool::mouseReleaseEvent(QMouseEvent* e)
{
  /* The AddCoordCommand calls the view's makeDraftPixmapDirty() method
  that warns that the draft needs to be redrawn */
  return false;
}

bool te::layout::CreateLineItemTool::keyPressEvent( QKeyEvent* keyEvent )
{
  Scene* scne = dynamic_cast<Scene*>(m_view->scene());
  if (!scne)
    return false;

  if(keyEvent->key() == Qt::Key_Escape)
  {
    finalizeCreation();
  }
  else if (keyEvent->matches(QKeySequence::Undo))
  {
    scne->getUndoStack()->undo();
    keyEvent->setAccepted(true);
  }
  else if (keyEvent->matches(QKeySequence::Redo))
  {
    scne->getUndoStack()->redo();
    keyEvent->setAccepted(true);
  }
  return true;
}

void te::layout::CreateLineItemTool::setGeometry()
{
  if (!m_item)
    return;

  AbstractItemView* iView = dynamic_cast<AbstractItemView*>(m_item);
  if (!iView)
    return;

  /* First the item must be positioned and have a size, 
  then have a geometry with internal coordinates */

  te::gm::LineString* lineString = new te::gm::LineString(m_coords.size(), te::gm::LineStringType);
  for (unsigned int i = 0; i < m_coords.size(); i++)
  {
    lineString->setPointN(i, m_coords[i]);
  }

  const te::gm::Envelope* mbr = lineString->getMBR();
  te::gm::Coord2D lowerLeft = mbr->getLowerLeft();
  double width = mbr->getWidth();
  double height = mbr->getHeight();
  
  // update position and size
  Properties props = createProperties(lowerLeft, width, height);
  iView->setProperties(props);

  // Update position of points according to the item's internal coordinates
  for (unsigned int i = 0; i < m_coords.size(); i++)
  {
    QPointF point(m_coords[i].getX(), m_coords[i].getY());
    QPointF point2Item = m_item->mapFromScene(point); // item's internal coordinate
    lineString->setPointN(i, te::gm::Point(point2Item.x(), point2Item.y()));
  }
    
  // update geometry
  props = createProperties(lineString);
  iView->setProperties(props);
  m_item->setSelected(true);
}

te::layout::Properties te::layout::CreateLineItemTool::createProperties(te::gm::LineString* lineString)
{
  te::gm::GeometryShrPtr geometry(lineString);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  Properties properties;
  {
    Property property(0);
    property.setName("geometry");
    property.setValue<te::gm::GeometryShrPtr>(geometry, dataType->getDataTypeGeometry());
    properties.addProperty(property);
  }
  return properties;
}

te::layout::Properties te::layout::CreateLineItemTool::createProperties(const te::gm::Coord2D& coord, double width, double height)
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
  {
    Property property(0);
    property.setName("width");
    property.setValue(width, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("height");
    property.setValue(height, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  return properties;
}

void te::layout::CreateLineItemTool::createItem()
{
  Scene* sc = m_view->getScene();
  if (!sc)
  {
    return;
  }

  if (!m_coords.empty())
  {
    std::string label = m_itemType->getLabel();
    sc->getUndoStack()->beginMacro("Create line item or derived child");

    // create a new item
    BuildGraphicsItem buildItem(sc);
    m_item = buildItem.createItem(m_itemType); 
    setGeometry();
    
    sc->getUndoStack()->endMacro();
  }
}

void te::layout::CreateLineItemTool::finalizeCreation()
{
  Scene* scene = dynamic_cast<Scene*>(m_view->scene());
  QPixmap* draftPixmap = m_view->getDraftPixmap();

  if (draftPixmap && scene)
  {
    draftPixmap->fill(Qt::transparent);
    createItem();
    m_coords.clear();
    m_view->resetDefaultConfig(true);
  }
}


void te::layout::CreateLineItemTool::redraw()
{
  QPixmap* draftPixmap = m_view->getDraftPixmap();
  if (draftPixmap)
  {
    draftPixmap->fill(Qt::transparent);
    if (m_render)
    {
      DrawGeometries type = m_render->getTypeGeometry();
      if (m_coords.size() == 1)
      {
        m_render->setTypeGeometry(te::layout::DRAW_POINT);
      }
      m_render->drawGeometries(draftPixmap, m_coords);

      m_render->setTypeGeometry(type);
    }
  }
}

void te::layout::CreateLineItemTool::setCoords(const std::vector<te::gm::Point>& coords)
{
  m_coords.clear();
  m_coords = coords;
}

std::vector<te::gm::Point> te::layout::CreateLineItemTool::getCoords()
{
  return m_coords;
}

