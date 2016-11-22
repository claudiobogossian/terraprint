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
#include "PropertyTree.h"

// Qt
#include <QMetaType>
#include <QString>
#include <QVariant>
#include <QTreeWidget>

te::layout::PropertyTreeItem::PropertyTreeItem(Property & prop, QTreeWidgetItem* parent) :
  QTreeWidgetItem(parent),
  m_property(prop)
{
  setFlags(flags() | Qt::ItemIsEditable);
}

te::layout::PropertyTreeItem::~PropertyTreeItem()
{

}

void te::layout::PropertyTreeItem::setData(int column, int role, const QVariant & value)
{
  QTreeWidgetItem::setData(column, role, value);

  if (column != 0)
  {
    QVariant name = data(0, Qt::UserRole); // get name of the property

    // update property
    m_property = qvariant_cast<te::layout::Property>(value);

    if (parent())
    {
      PropertyTreeItem* parentTreeItem = dynamic_cast<PropertyTreeItem*>(parent());
      if (parentTreeItem)
      {
        //QTreeWidgetItem::setData(column, role, value);
        parentTreeItem->refresh(column, role, name.toString(), value, this); // refresh parent
      }
    }
    else
    {
      /* For each setData there is an internal call to the model->emitDataChanged, so with each change,
      the QTreeWidget's itemChanged method is called. In case of this customization,
      when the item has a parent, then only the parent could notify */
      // this item data has changed, so notify QTreeWidget
      QTreeWidget* tree = treeWidget();
      if (tree)
      {
        PropertyTree* propTree = dynamic_cast<PropertyTree*>(tree);
        if (propTree)
        {
          propTree->propertyTreeItemChanged(this, column);
        }
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

te::layout::EnumType* te::layout::PropertyTreeItem::getDataType()
{
  return m_property.getType();
}

int te::layout::PropertyTreeItem::getDataRole()
{
  int propertyRole = qRegisterMetaType<te::layout::Property>("te::layout::Property");
  return propertyRole;
}

