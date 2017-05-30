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
  \file AddCommand.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AddCommand.h"
#include "../../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../../core/pattern/mvc/AbstractItemModel.h"
#include "../../../../core/enum/EnumType.h"
#include "../../../../core/enum/Enums.h"
#include "../../ItemUtils.h"
#include "../../Scene.h"
#include "../../View.h"
#include "../../../item/LineItem.h"
#include "../../../item/PolygonItem.h"
#include "../../tools/CreateLineItemTool.h"

// Qt
#include <QGraphicsScene>
#include <QGraphicsItem>

te::layout::AddCommand::AddCommand( QGraphicsItem* item, QUndoCommand *parent ) :
  QUndoCommand(parent),
  m_scene(0),
  m_item(item)
{
  if(!m_item)
    return;

  m_scene = item->scene();

  m_initialPosition = m_item->scenePos();
  setText(QObject::tr("Add %1").arg(createCommandString(m_item, m_initialPosition)));
}

te::layout::AddCommand::~AddCommand()
{
  
}

void te::layout::AddCommand::undo()
{
  Scene* scene = dynamic_cast<Scene*>(m_scene);

  if (!m_item)
    return;

  if(m_item->scene() != scene)
    return;

  AbstractItemView* abstractItem = dynamic_cast<AbstractItemView*>(m_item);
  if (!abstractItem)
    return;
  
  m_scene->removeItem(m_item);    
  scene->addItemStackWithoutScene(m_item);
  reactivateTool(abstractItem); // reactivate tool
  m_scene->clearSelection();
  m_scene->update();
}

void te::layout::AddCommand::redo()
{
  Scene* scene = dynamic_cast<Scene*>(m_scene);

  if(!scene || !m_item)
    return;

  AbstractItemView* obs = dynamic_cast<AbstractItemView*>(m_item);

  if (!obs)
    return;

  deactivateTool(); // deactivate tool

  /* Checks if the item is already 
     added to the scene */
  if(m_item->scene() == m_scene)
    return;
    
  obs->setUndoEnabled(false); // set properties will not generate an UndoCommand on the Stack
  scene->insertItem(m_item);
  scene->removeItemStackWithoutScene(m_item);
  m_item->setPos(m_initialPosition);
  obs->setUndoEnabled(true); // set properties will generate an UndoCommand on the Stack

  m_scene->clearSelection();
  m_scene->update();
}

QString te::layout::AddCommand::createCommandString( QGraphicsItem* item, const QPointF &pos )
{
  if(!m_item)
    return QObject::tr("%1");

  AbstractItemView* obs = dynamic_cast<AbstractItemView*>(item);

  if(!obs)
    return QObject::tr("%1");
  
  std::string name = obs->getProperties().getTypeObj()->getName();
  QString qName = ItemUtils::convert2QString(name);

  return QObject::tr("%1 at (%2, %3)")
    .arg(qName)
    .arg(pos.x()).arg(pos.y());
}

bool te::layout::AddCommand::reactivateTool(AbstractItemView* item)
{
  bool result = false;
  Scene* scene = dynamic_cast<Scene*>(m_scene);
  if (!scene)
    return result;

  // Type of items like line/polygon need a current tool to create
  if (scene->getView())
  {
    View* view = scene->getView();
    CreateLineItemTool* lineTool = activateTool(item);
    if (lineTool)
    {
      lineTool->setCoords(m_coords);
      /* When the tools need to draw in the pixmap, this function must be called,
      so the View itself will call the update() method and redraw() method of the current tool. */
      view->makeDraftPixmapDirty(true);
      result = true;
    }
  }
  return result;
}

void te::layout::AddCommand::deactivateTool()
{
  Scene* scene = dynamic_cast<Scene*>(m_scene);

  /* Checks if the item is already
  added to the scene */
  if (m_item->scene() == m_scene)
  {
    m_coords = getCoordsTool();
    return;
  }

  View* view = scene->getView();
  if (view)
  {
    CreateLineItemTool* lineTool = getLineActiveTool();
    if (lineTool)
    {
      // clear draft pixmap and delete current tool
      view->resetDefaultConfig(true);
      view->clearDraftPixmap();
    }
  }
}

std::vector<te::gm::Point> te::layout::AddCommand::getCoordsTool()
{
  std::vector<te::gm::Point> coords;

  Scene* scene = dynamic_cast<Scene*>(m_scene);
  if (!scene)
    return coords;

  // Type of items like line/polygon need a current tool to create
  CreateLineItemTool* lineTool = getLineActiveTool();
  if (lineTool)
  {
    coords = lineTool->getCoords();
  }
  return coords;
}

te::layout::CreateLineItemTool* te::layout::AddCommand::getLineActiveTool()
{
  CreateLineItemTool* tool = 0;
  Scene* scene = dynamic_cast<Scene*>(m_scene);
  if (!scene)
    return false;

  // Type of items like line/polygon need a current tool to create
  if (scene->getView())
  {
    View* view = scene->getView();
    AbstractLayoutTool* absTool = view->getCurrentTool();
    if (absTool)
    {
      CreateLineItemTool* lineTool = dynamic_cast<CreateLineItemTool*>(absTool);
      if (lineTool)
      {
        tool = lineTool;
      }
    }
  }
  return tool;
}

te::layout::CreateLineItemTool* te::layout::AddCommand::activateTool(AbstractItemView* item)
{
  CreateLineItemTool* tool = 0;
  Scene* scene = dynamic_cast<Scene*>(m_scene);
  if (!scene)
    return tool;

  EnumObjectType* objectType = Enums::getInstance().getEnumObjectType();

  // Type of items like line/polygon need a current tool to create
  if (scene->getView())
  {
    View* view = scene->getView();
    const Properties& props = item->getProperties();

    if (props.getTypeObj() == objectType->getLineItem())
    {
      view->createLineItem();
    }
    else if (props.getTypeObj() == objectType->getPolygonItem())
    {
      view->createPolygonItem();
    }
    tool = getLineActiveTool();
  }
  return tool;
}

