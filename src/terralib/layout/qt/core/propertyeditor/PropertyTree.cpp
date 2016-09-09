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

// STL
#include "stdlib.h"

// Qt
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <QString>

te::layout::PropertyTree::PropertyTree(View* view, QWidget* parent) :
  QTreeWidget(parent),
  m_view(view),
  m_columns(2)
{
  configTree(m_columns);

  PropertyDelegate* propDelegate = new PropertyDelegate(this);
  setItemDelegateForColumn(1, propDelegate); // Add new delegate to second column
}

te::layout::PropertyTree::~PropertyTree()
{

}

void te::layout::PropertyTree::setProperties(std::vector<Property> vprops)
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
  setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
}

void te::layout::PropertyTree::load(std::vector<Property> props)
{
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

  update();
}

QTreeWidgetItem* te::layout::PropertyTree::createNewRow(Property prop, QTreeWidgetItem* parent)
{
  QString propertyName = ItemUtils::convert2QString(prop.getName());

  QTreeWidgetItem* newItem = new QTreeWidgetItem(parent); //  Detail to generate a hierarchical tree, the parent must be passed in the constructor QTreeWidgetItem
  newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
  newItem->setText(0, propertyName); // First Column (Property Name)
  newItem->setData(0, Qt::UserRole, QVariant(propertyName)); // First Column(Property Name)

  QVariant variant = prop.getValue().toQVariant();

  // Second Column (Value)
  newItem->setData(1, prop.getValue().getType()->getId(), variant); // Property Value

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

