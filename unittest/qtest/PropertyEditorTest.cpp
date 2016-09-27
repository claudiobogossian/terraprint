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

#include "PropertyEditorTest.h"

// Layout Module
#include "terralib/layout/qt/core/propertyeditor/PropertyTree.h"
#include "terralib/layout/qt/core/BuildGraphicsItem.h"
#include "terralib/layout/core/pattern/mvc/AbstractItemView.h"
#include "terralib/layout/core/pattern/mvc/AbstractItemController.h"
#include "terralib/layout/core/enum/Enums.h"

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

// Boost
#include <boost/test/unit_test.hpp>

void te::layout::PropertyEditorTest::on_test_create_rectangle_item()
{
  EnumType* itemType = Enums::getInstance().getEnumObjectType()->getRectangleItem();
  if (!itemType)
  {
    BOOST_FAIL("FAIL: pointer to enum retangle item is null."); //throws on error
  }

  te::gm::Coord2D coord(5, 5);
  BuildGraphicsItem buildItem(0);
  QGraphicsItem* item = buildItem.createItem(itemType, coord, 20, 20);

  if (!item)
  {
    BOOST_FAIL("FAIL: pointer to rectangle object is null."); //throws on error
  }

  RectangleItem* rect = dynamic_cast<RectangleItem*>(item);
  if (!rect)
  {
    BOOST_FAIL("FAIL: pointer to rectangle object is null."); //throws on error
  }

  m_rectItem.reset(rect);
}

void te::layout::PropertyEditorTest::on_test_create_property_tree()
{
  if (!m_rectItem.get())
  {
    BOOST_FAIL("FAIL: pointer to rectangle object is null."); //throws on error
  }

  te::layout::PropertyTree* tree = new te::layout::PropertyTree(0);
  
  std::vector<Property> props;
  AbstractItemView* view = dynamic_cast<AbstractItemView*>(m_rectItem.get());
  Properties itemProperties = view->getController()->getProperties();
  props = itemProperties.getProperties();

  tree->load(props);
    
  if (tree->topLevelItemCount() > 0)
  {
    QTreeWidgetItem* item0 = tree->topLevelItem(0);
    tree->editItem(item0, 1); // enter edit mode, automatically call the delegate
  }
  else
  {
    QVERIFY2(tree->topLevelItemCount() == 0, "List of items is empty.");
  }
}

