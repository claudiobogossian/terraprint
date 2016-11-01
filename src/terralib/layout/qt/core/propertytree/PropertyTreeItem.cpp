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
  \file PropertyTree.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PropertyTreeItem.h"

// Qt
#include <QString>
#include <QVariant>

te::layout::PropertyTreeItem::PropertyTreeItem(Property & prop, QTreeWidgetItem* parent) :
  QTreeWidgetItem(parent),
  m_property(prop)
{
  m_dataRole = prop.getType()->getId();
}

te::layout::PropertyTreeItem::~PropertyTreeItem()
{

}

void te::layout::PropertyTreeItem::setData(int column, int role, const QVariant & value)
{
  QTreeWidgetItem::setData(column, role, value);

  if (column != 0)
  {
    //m_datatype = role
    QVariant name = data(0, Qt::UserRole); // get name of the property

    if (parent())
    {
      PropertyTreeItem* parentTreeItem = dynamic_cast<PropertyTreeItem*>(parent());
      if (parentTreeItem)
      {
        parentTreeItem->refresh(column, role, name.toString(), value, this); // refresh parent
      }
    }
  }
}

te::layout::Property te::layout::PropertyTreeItem::getProperty()
{
  return m_property;
}

void te::layout::PropertyTreeItem::setPropertyValue(QVariant variant)
{
  // do nothing
}

void te::layout::PropertyTreeItem::refresh(int column, int role, QString name, QVariant value, QTreeWidgetItem* childItem)
{
  // do nothing
}

void te::layout::PropertyTreeItem::refreshChild(int column, int role, QString name, QVariant value)
{
  int count = childCount();
  for (int i = 0; i < count; ++i)
  {
    QTreeWidgetItem* itemChild = child(i);
    if (itemChild->data(0, Qt::UserRole) == name)
    {
      setChildData(itemChild, column, role, value);
      break;
    }
  }
}

void te::layout::PropertyTreeItem::setChildData(QTreeWidgetItem* childItem, int column, int role, QVariant value)
{
  childItem->setData(column, role, value); // Property Value
  PropertyTreeItem* childTreeItem = dynamic_cast<PropertyTreeItem*>(childItem);
  if (childTreeItem)
  {
    Property prop = childTreeItem->getProperty(); //How update child property(toSomething()) ?
  }
}

int te::layout::PropertyTreeItem::getDataRole()
{
  return m_dataRole;
}

