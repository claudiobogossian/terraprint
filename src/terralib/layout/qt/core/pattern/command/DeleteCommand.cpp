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
  \file DeleteCommand.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "DeleteCommand.h"
#include "../../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../item/ItemGroup.h"
#include "../../Scene.h"

// Qt
#include <QGraphicsScene>
#include <QGraphicsItem>

te::layout::DeleteCommand::DeleteCommand( QGraphicsScene* scene, const QList<QGraphicsItem*>& listItems, QUndoCommand *parent ) :
  QUndoCommand(parent),
  m_scene(scene),
  m_items(listItems)
{
  init();
}

te::layout::DeleteCommand::~DeleteCommand()
{
  
}

void te::layout::DeleteCommand::init()
{
  m_scene->clearSelection();

  foreach(QGraphicsItem *item, m_items)
  {
    ItemGroup* group = dynamic_cast<ItemGroup*>(item);
    if (group)
    {
      std::vector<std::string> childrenNames;
      // get names of all children
      foreach(QGraphicsItem* item, item->childItems())
      {
        AbstractItemView* abView = dynamic_cast<AbstractItemView*>(item);
        if (abView)
        {
          const Property& prop_name = abView->getProperty("name");
          std::string name = te::layout::Property::GetValueAs<std::string>(prop_name);

          childrenNames.push_back(name);
        }
      }
      m_groups[item] = childrenNames; // group with children names
    }
  }

  int size = m_items.size();

  setText(QObject::tr("Delete %1")
    .arg(createCommandString(size)));
}

void te::layout::DeleteCommand::undo()
{
  Scene* scene = dynamic_cast<Scene*>(m_scene);

  if(!scene)
    return;

  m_scene->clearSelection();

  foreach( QGraphicsItem *item, m_items ) 
  {
    if(item->scene() != m_scene)
    {
      ItemGroup* group = dynamic_cast<ItemGroup*>(item);
      if (group)
      {
        QList<QGraphicsItem *> items = childrenItems(group);
        scene->createItemGroup(items, group);
      }
      else
      {
        scene->insertItem(item);
        scene->removeItemStackWithoutScene(item);
      }
      item->setSelected(true);
    }
  }
  m_scene->update();
}

void te::layout::DeleteCommand::redo()
{
  Scene* scene = dynamic_cast<Scene*>(m_scene);

  if(!scene)
    return;

  foreach( QGraphicsItem *item, m_items ) 
  {
    if(item->scene() == m_scene)
    {
      ItemGroup* group = dynamic_cast<ItemGroup*>(item);
      if (group)
      {
        scene->removeItemGroup(group);
      }
      else
      {
        m_scene->removeItem(item);
        scene->addItemStackWithoutScene(item);
      }
    }
  }

  m_scene->clearSelection();
  m_scene->update();
}

QString te::layout::DeleteCommand::createCommandString( int totalItems )
{
  return QObject::tr("%1")
    .arg(totalItems);
}

QList<QGraphicsItem *> te::layout::DeleteCommand::childrenItems(QGraphicsItem* group)
{
  QList<QGraphicsItem *> items;

  Scene* scene = dynamic_cast<Scene*>(m_scene);
  if (!scene)
    return items;

  std::vector<std::string> childrenNames;
  std::map< QGraphicsItem*, std::vector<std::string> >::iterator it;
  it = m_groups.find(group);

  if (it != m_groups.end())
  {
    childrenNames = it->second;
  }

  std::vector<std::string>::iterator itChild = childrenNames.begin();
  for (; itChild != childrenNames.end(); ++itChild)
  {
    std::string name = (*itChild);
    AbstractItemView* iView = scene->getItem(name);
    if (iView)
    {
      QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(iView);
      if (item)
      {
        items.append(item);
      }
    }
  }
  return items;
}
