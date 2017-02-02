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
  \file UngroupCommand.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "UngroupCommand.h"
#include "../../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../item/ItemGroup.h"
#include "../../Scene.h"

// Qt
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QString>

te::layout::UngroupCommand::UngroupCommand(QGraphicsScene* scene, QGraphicsItem* item, QUndoCommand *parent) :
  QUndoCommand(parent),
  m_scene(scene),
  m_group(item)
{
  init();
}

te::layout::UngroupCommand::~UngroupCommand()
{
  
}

void te::layout::UngroupCommand::init()
{
  m_scene->clearSelection();

  std::string name = "";

  ItemGroup* group = dynamic_cast<ItemGroup*>(m_group);
  if (group)
  {
    std::vector<std::string> childrenNames;
    // get names of all children
    foreach(QGraphicsItem* item, group->childItems())
    {
      AbstractItemView* abView = dynamic_cast<AbstractItemView*>(item);
      if (abView)
      {
        const Property& prop_name = abView->getProperty("name");
        std::string name = te::layout::Property::GetValueAs<std::string>(prop_name);

        childrenNames.push_back(name);
      }
    }
    m_childrenNames = childrenNames; // group with children names
    name = Property::GetValueAs<std::string>(group->getProperty("name"));
  }

  setText(QObject::tr("Ungroup %1")
    .arg(createCommandString(name)));
}

void te::layout::UngroupCommand::undo()
{
  Scene* scene = dynamic_cast<Scene*>(m_scene);

  if (!scene || !m_group)
    return;

  if (m_group->scene() == m_scene)
  {
    return;
  }

  m_scene->clearSelection();

  QList<QGraphicsItem *> items = childrenItems();
  scene->createItemGroup(items, m_group);
  m_group->setSelected(true);

  m_scene->update();
}

void te::layout::UngroupCommand::redo()
{
  Scene* scene = dynamic_cast<Scene*>(m_scene);

  if (!scene || !m_group)
    return;

  if (m_group->scene() != m_scene)
  {
    return;
  }

  ItemGroup* group = dynamic_cast<ItemGroup*>(m_group);
  if (group)
  {
    scene->removeItemGroup(group);
    m_scene->clearSelection();
    m_scene->update();
  }
}

QString te::layout::UngroupCommand::createCommandString(const std::string& name)
{
  QString qName = ItemUtils::convert2QString(name);
  return QObject::tr("%1").arg(qName);
}

QList<QGraphicsItem *> te::layout::UngroupCommand::childrenItems()
{
  QList<QGraphicsItem *> items;

  Scene* scene = dynamic_cast<Scene*>(m_scene);

  if (!scene || !m_group)
    return items;

  std::vector<std::string>::iterator it = m_childrenNames.begin();

  for (; it != m_childrenNames.end(); ++it)
  {
    std::string name = (*it);
    AbstractItemView* iView = scene->getItem(name);
    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(iView);
    if (item)
    {
      items.append(item);
    }
  }
  return items;
}
