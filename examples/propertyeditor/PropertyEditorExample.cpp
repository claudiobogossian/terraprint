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

#include "PropertyEditorExample.h"

// Layout Module
#include "terralib/layout/qt/core/propertytree/PropertyTree.h"
#include "terralib/layout/qt/core/BuildGraphicsItem.h"
#include "terralib/layout/qt/item/RectangleItem.h"
#include "terralib/layout/core/pattern/mvc/AbstractItemView.h"
#include "terralib/layout/core/pattern/mvc/AbstractItemController.h"
#include "terralib/layout/core/enum/Enums.h"
#include "terralib/layout/core/property/Property.h"

// STL
#include <vector>

// Qt
#include <QtTest/QtTest>
#include <QGraphicsItem>
#include <QSize>
#include <QPoint>
#include <QString>
#include <QToolButton>
#include <QAction>
#include <QList>
#include <QMenu>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QComboBox>
#include <QStringList>

te::layout::PropertyEditorExample::PropertyEditorExample(QWidget* parent) :
  QWidget(parent),
  m_rectItem(0),
  m_tree(0),
  m_combobox(0)
{
  setWindowTitle(tr("Property Editor"));
  resize(480, 320);
}

te::layout::PropertyEditorExample::~PropertyEditorExample()
{
  if (m_rectItem)
  {
    delete m_rectItem;
  }
}

void te::layout::PropertyEditorExample::run()
{
  createRectangleItem();
  createPropertyTree();
}

void te::layout::PropertyEditorExample::createRectangleItem()
{
  EnumType* itemType = Enums::getInstance().getEnumObjectType()->getRectangleItem();
  if (!itemType)
  {
    return;
  }

  te::gm::Coord2D coord(5, 5);
  BuildGraphicsItem buildItem(0);
  QGraphicsItem* item = buildItem.createItem(itemType, coord, 20, 20);

  if (!item)
  {
    return;
  }

  m_rectItem = dynamic_cast<RectangleItem*>(item);
}

void te::layout::PropertyEditorExample::createPropertyTree()
{
  if (!m_rectItem)
  {
    return;
  }

  m_tree = new te::layout::PropertyTree(0, 0, this); // create property tree
  createLayout();
  
  std::vector<Property> props;
  AbstractItemView* view = dynamic_cast<AbstractItemView*>(m_rectItem);
  Properties itemProperties = view->getController()->getProperties();
  props = itemProperties.getProperties();

  m_tree->load(props); // load properties from rectangle item
}

void te::layout::PropertyEditorExample::createLayout()
{
  //Layout

  if (!m_tree)
    return;

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setMargin(0);
  
  m_combobox = new QComboBox(this);
  m_combobox->setBaseSize(QSize(16, 16));
  m_combobox->setVisible(true);

  loadComboboxNames();

  layout->addWidget(m_combobox);
  layout->addWidget(m_tree);
  
  setLayout(layout);
}

void te::layout::PropertyEditorExample::loadComboboxNames()
{
  QStringList list;

  // add rect item name
  const te::layout::Property rectProp = m_rectItem->getController()->getProperty("name");

  std::string name = te::layout::Property::GetValueAs<std::string>(rectProp);

  list.append(name.c_str());

  m_combobox->addItems(list);
}

