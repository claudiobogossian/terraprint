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
#include "PropertyTree.h"
#include "../../core/ItemUtils.h"
#include "PropertyDelegate.h"
#include "PropertyTreeItem.h"
#include "../propertytree/BuildTreeItem.h"

// STL
#include "stdlib.h"

// Qt
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <QString>

te::layout::PropertyTree::PropertyTree(View* view, PropertyDelegate* delegate, QWidget* parent) :
  QTreeWidget(parent),
  m_view(view),
  m_columns(2),
  m_nameColumn(0),
  m_valueColumn(1),
  m_loading(false)
{
  configTree(m_columns);

  PropertyDelegate* propDelegate = delegate;
  if (!propDelegate)
  {
    propDelegate = new PropertyDelegate(this);
  }

  setItemDelegateForColumn(1, propDelegate); // Add new delegate to second column

  // check column and let just the value column editable
  connect(this, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(onCheckEdit(QTreeWidgetItem*, int)));
}

te::layout::PropertyTree::~PropertyTree()
{

}

void te::layout::PropertyTree::setProperties(std::vector<te::layout::Property> vprops)
{
  m_vprops = vprops;
}

void te::layout::PropertyTree::configTree(int numberColumns)
{
  setColumnCount(numberColumns);

  QStringList horHeaders;
  horHeaders.append("Property");
  horHeaders.append("Value");
  setHeaderLabels(horHeaders);

  setAutoFillBackground(true);
  setAlternatingRowColors(true);

  // To edit only the column values, you must disable the triggers and make via signal / slot. 
  setEditTriggers(QAbstractItemView::NoEditTriggers); 
}

void te::layout::PropertyTree::load(std::vector<te::layout::Property> props)
{
  m_loading = true;
  m_vprops = props;
  clearAll();
  std::size_t rows = props.size();

  configTree(m_columns);
    
  // Tree widget items are used to hold rows of information for tree widgets    
  
  std::vector<Property>::iterator it;
  for (it = m_vprops.begin(); it != m_vprops.end(); ++it)
  {
    Property prop = (*it);
    QTreeWidgetItem* newItem = createNewRow(prop, invisibleRootItem()); // root QTreeWidgetItem
    setItemExpanded(newItem, true); // Warning: The QTreeWidgetItem must be added to the QTreeWidget before calling this function
  }
  m_loading = false;
}

QTreeWidgetItem* te::layout::PropertyTree::createNewRow(te::layout::Property prop, QTreeWidgetItem* parent)
{
  QString propertyName = ItemUtils::convert2QString(prop.getName());

  BuildTreeItem build;

  PropertyTreeItem* newItem = build.buildTreeItem(prop, parent); //  Detail to generate a hierarchical tree, the parent must be passed in the constructor QTreeWidgetItem

  newItem->setText(m_nameColumn, propertyName); // First Column (Property Name)
  newItem->setData(m_nameColumn, Qt::UserRole, QVariant(propertyName)); // First Column(Property Name)

  int propertyType = qRegisterMetaType<te::layout::Property>("te::layout::Property");

  // te::layout::Property to QVariant (Wrapper)
  QVariant variant = QVariant::fromValue<te::layout::Property>(prop);
  // Second Column (Value)
  newItem->setData(m_valueColumn, propertyType, variant); // Property Value
  
  // find child
  if (!prop.getSubProperty().empty())
  {
    std::vector<Property>::iterator it;
    for (it = m_vprops.begin(); it != m_vprops.end(); ++it)
    {
      Property child = (*it);
      QTreeWidgetItem* newChildItem = createNewRow(child, newItem); // add child
    }
  }

  return newItem;
}

void te::layout::PropertyTree::clearAll()
{
  clear();
}

void te::layout::PropertyTree::onCheckEdit(QTreeWidgetItem * item, int column)
{
  if (column == m_valueColumn)
  {
    editItem(item, column);
  }
}

void te::layout::PropertyTree::propertyTreeItemChanged(QTreeWidgetItem * item, int column)
{
  if (column == 1 && m_loading == false)
  {
    QTreeWidgetItem* itemForUpdate = findTopParent(item, item->parent());
    if (itemForUpdate)
    {
      PropertyTreeItem* treeItem = dynamic_cast<PropertyTreeItem*>(itemForUpdate);
      if (treeItem)
      {
        Property propForUpdate = treeItem->getProperty();
        emit propertiesChanged(propForUpdate);
      }
    }
  }
}

QTreeWidgetItem* te::layout::PropertyTree::findTopParent(QTreeWidgetItem* currentItem, QTreeWidgetItem* parentItem)
{
  if (parentItem)
  {
    if (parentItem != invisibleRootItem())
    {
      currentItem = parentItem;
      if (parentItem->parent())
      {
        findTopParent(currentItem, parentItem->parent());
      }
      else
      {
        return currentItem;
      }
    }
    else
    {
      return currentItem;
    }
  }
  return currentItem;
}
