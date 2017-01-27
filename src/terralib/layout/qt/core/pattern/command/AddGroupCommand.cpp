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
#include "AddGroupCommand.h"
#include "../../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../../core/pattern/mvc/AbstractItemModel.h"
#include "../../../../core/enum/EnumType.h"
#include "../../../../core/enum/Enums.h"
#include "../../ItemUtils.h"
#include "../../Scene.h"
#include "../../../item/ItemGroup.h"

// Qt
#include <QGraphicsItem>

te::layout::AddGroupCommand::AddGroupCommand(QGraphicsItem* item, QUndoCommand *parent) :
  QUndoCommand(parent),
  m_group(item)
{
  init(item);
}

te::layout::AddGroupCommand::~AddGroupCommand()
{
  
}

void te::layout::AddGroupCommand::init(QGraphicsItem* item)
{
  QGraphicsScene* scene = item->scene();
  if (!scene)
  {
    return;
  }

  m_scene = dynamic_cast<Scene*>(scene);

  ItemGroup* group = dynamic_cast<ItemGroup*>(item);
  m_groupProperties = group->getProperties();

  // get names of all children
  foreach(QGraphicsItem* item, item->childItems())
  {
    AbstractItemView* abView = dynamic_cast<AbstractItemView*>(item);

    const Property& prop_name = abView->getProperty("name");
    std::string name = te::layout::Property::GetValueAs<std::string>(prop_name);

    m_childrenNames.push_back(name);
  }

  setText(QObject::tr("Add %1").arg(createCommandString()));
}

void te::layout::AddGroupCommand::undo()
{
  /* Checks if the item is already
  added to the scene */
  if (m_group->scene() != m_scene)
    return;
  
  ItemGroup* group = dynamic_cast<ItemGroup*>(m_group);
  m_scene->removeItemGroup(group);
  m_scene->update();
}

void te::layout::AddGroupCommand::redo()
{
  /* Checks if the item is already
  added to the scene */
  if (m_group->scene() == m_scene)
    return;

  QList<QGraphicsItem *> items = childrenItems();
  ItemGroup* group = dynamic_cast<ItemGroup*>(m_group);

  m_scene->createItemGroup(items, group);
  
  m_scene->clearSelection();
  m_group->setSelected(true);
  m_scene->update();
}

QString te::layout::AddGroupCommand::createCommandString()
{
  const Property& prop_x = m_groupProperties.getProperty("x");
  double x = te::layout::Property::GetValueAs<double>(prop_x);

  const Property& prop_y = m_groupProperties.getProperty("y");
  double y = te::layout::Property::GetValueAs<double>(prop_y);

  const Property& prop_name = m_groupProperties.getProperty("name");
  std::string name = te::layout::Property::GetValueAs<std::string>(prop_name);

  QPointF pos(x, y);
  QString qName = ItemUtils::convert2QString(name);

  return QObject::tr("%1 at (%2, %3)")
    .arg(qName)
    .arg(pos.x()).arg(pos.y());
}

QList<QGraphicsItem *> te::layout::AddGroupCommand::childrenItems()
{
  QList<QGraphicsItem *> items;
  std::vector<std::string>::iterator it = m_childrenNames.begin();

  for (; it != m_childrenNames.end(); ++it)
  {
    std::string name = (*it);
    AbstractItemView* iView = m_scene->getItem(name);
    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(iView);
    if (item)
    {
      items.append(item); 
    }
  }
  return items;
}
