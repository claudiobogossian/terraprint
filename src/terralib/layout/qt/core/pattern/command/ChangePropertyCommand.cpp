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
  \file ChangePropertyCommand.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ChangePropertyCommand.h"
#include "../../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../../core/pattern/mvc/AbstractItemModel.h"
#include "../../../outside/PropertiesOutside.h"
#include "../../ItemUtils.h"

// Qt
#include <QGraphicsItem>
#include <QPointF>

te::layout::ChangePropertyCommand::ChangePropertyCommand( QGraphicsItem* item, Properties oldProperties, 
  Properties newProperties, PropertiesOutside* outside, QUndoCommand *parent ) :
  QUndoCommand(parent),
  m_item(item),
  m_oldProperties(oldProperties),
  m_newProperties(newProperties),
  m_outside(outside)
{

}

te::layout::ChangePropertyCommand::ChangePropertyCommand( std::vector<QGraphicsItem*> items, std::vector<Properties> allOld, 
  std::vector<Properties> allNew, PropertiesOutside* outside, QUndoCommand *parent ) :
  QUndoCommand(parent),
  m_item(0),
  m_outside(outside),
  m_items(items),
  m_allOldProperties(allOld),
  m_allNewProperties(allNew)
{

}

te::layout::ChangePropertyCommand::~ChangePropertyCommand()
{
  m_allOldProperties.clear();
  m_allNewProperties.clear();
}

void te::layout::ChangePropertyCommand::undo()
{
  if(m_item)
  {
    if(!checkItem(m_item, m_oldProperties))
    {
      return;
    }
  }

  if(!checkVectors())
  {
    return;
  }
  
  for(unsigned int i = 0 ; i < m_items.size() ; ++i)
  {
    QGraphicsItem* item = m_items[i];
    Properties props = m_allOldProperties[i];
    if (item)
    {
      checkItem(item, props);
    }
  }
    
  if(m_outside)
  {
    m_outside->refreshOutside();
  }

  setText(QObject::tr("Change Properties %1")
    .arg(createCommandString(m_item)));
}

void te::layout::ChangePropertyCommand::redo()
{
  if(m_item)
  {
    if(!checkItem(m_item, m_newProperties))
    {
      return;
    }
  }
  
  if(!checkVectors())
  {
    return;
  }

  for(unsigned int i = 0 ; i < m_items.size() ; ++i)
  {
    QGraphicsItem* item = m_items[i];
    Properties props = m_allNewProperties[i];
    if (item)
    {
      checkItem(item, props);
    }
  }
  
  if(m_outside)
  {
    m_outside->refreshOutside();
  }

  setText(QObject::tr("Change Properties %1")
    .arg(createCommandString(m_item)));
}

QString te::layout::ChangePropertyCommand::createCommandString( QGraphicsItem* item )
{
  if(!m_item)
  {
    if(!m_items.empty())
    {
       return QObject::tr("%1 %2").arg(m_items.size()).arg("items");
    }
    return QObject::tr("%1");
  }
    
  AbstractItemView* absView = dynamic_cast<AbstractItemView*>(item);

  if(!absView)
    return QObject::tr("%1");

  QPointF pos = m_item->scenePos();

  std::string name = absView->getProperties().getTypeObj()->getName();
  QString qName = ItemUtils::convert2QString(name);

  return QObject::tr("%1 at (%2, %3)")
    .arg(qName)
    .arg(pos.x()).arg(pos.y());
}

bool te::layout::ChangePropertyCommand::checkItem( QGraphicsItem* item, Properties props )
{
  if(!item)
    return false;

  AbstractItemView* obs = dynamic_cast<AbstractItemView*>(item);

  if(!obs)
    return false;

  const Properties& propsModel = obs->getProperties();
  if (props.equals(propsModel) == true)
    return false;

  obs->setUndoEnabled(false); // set properties will not generate an UndoCommand on the Stack
  obs->setProperties(props);
  obs->setUndoEnabled(true); // set properties will generate an UndoCommand on the Stack
  obs->refresh();

  return true;
}

bool te::layout::ChangePropertyCommand::checkVectors()
{
  if(m_allNewProperties.size() != m_items.size() ||
    m_allOldProperties.size() != m_items.size())
  {
    return false;
  }

  return true;
}
