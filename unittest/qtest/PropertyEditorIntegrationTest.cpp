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

#include "PropertyEditorIntegrationTest.h"

// Layout Module
#include <terralib/layout/qt/core/propertyeditor/tree/PropertyTree.h>
#include <terralib/layout/qt/core/propertyeditor/tree/PropertyDelegate.h>
#include <terralib/layout/qt/core/propertyeditor/tree/ContextPropertyEditor.h>
#include <terralib/layout/qt/outside/ToolbarOutside.h>
#include <terralib/layout/qt/core/Scene.h>
#include <terralib/layout/core/pattern/mvc/AbstractItemView.h>
#include <terralib/layout/core/pattern/mvc/AbstractItemController.h>

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

void te::layout::qtest::PropertyEditorIntegrationTest::on_test_create_view()
{
  m_view.reset(new te::layout::View);
  QVERIFY2(m_view.get() != 0, "Could not create a View object.");
}

void te::layout::qtest::PropertyEditorIntegrationTest::on_test_create_scene()
{
  te::layout::Scene* myScene = new te::layout::Scene(m_view.get());
  m_view->setScene(myScene);

  QVERIFY2(m_view->getScene() != 0, "Could not create a Scene object.");
  m_view->config();
}

void te::layout::qtest::PropertyEditorIntegrationTest::on_test_create_outside_area()
{
  m_outsideArea.reset(new OutsideArea(0, m_view.get()));
  QVERIFY2(m_outsideArea.get() != 0, "Could not create a OutsideArea object.");
}

void te::layout::qtest::PropertyEditorIntegrationTest::on_test_create_rectangle_item()
{
  QVERIFY2(m_outsideArea->getToolbar() != 0, "The ToolBar object is null.");
  QString rectName = m_outsideArea->getToolbar()->getActionRectangle();

  QToolButton* toolButton = m_outsideArea->getToolbar()->getGeometryToolButton();
  QVERIFY2(toolButton != 0, "The geometry toolbutton object is null.");

  QMenu* mnu = toolButton->menu();
  QVERIFY2(mnu != 0, "The menu of geometry toolbutton object is null.");

  QList<QAction*> actions = mnu->actions();
  QAction* rectActionMnu = 0;
  for (int i = 0; i < actions.size(); ++i)
  {
    QAction* action = actions.value(i);
    if (action->objectName().compare(rectName) == 0)
    {
      rectActionMnu = action;
      break;
    }
  }

  QVERIFY2(rectActionMnu != 0, "The rectangle action of the menu of geometry toolbutton object is null.");

  // Test: click on geometry menu
  QTest::mouseClick(mnu, Qt::LeftButton, 0, mnu->geometry().center(), 300);
  // Test: click on rectangle item action (select rectangle item for creation)
  QTest::mouseClick(mnu, Qt::LeftButton, 0, mnu->actionGeometry(rectActionMnu).center(), 300);

  // Test: create rectangle item via mouse click
  QPoint clickPoint(75, 105);
  QTest::mouseClick(m_view->viewport(), Qt::LeftButton, 0, clickPoint);
}

void te::layout::qtest::PropertyEditorIntegrationTest::on_test_click_rectangle_item()
{
  // Test: select rectangle item
  QPoint clickPoint(75, 105);
  QTest::mouseClick(m_view->viewport(), Qt::LeftButton, 0, clickPoint);
}

void te::layout::qtest::PropertyEditorIntegrationTest::on_test_create_property_tree()
{
  ContextPropertyEditor* context = new ContextPropertyEditor(0, 0);
  PropertyDelegate* propDelegate = new PropertyDelegate(context);
  te::layout::PropertyTree* tree = new te::layout::PropertyTree(m_view.get(), propDelegate, m_view->viewport());
  QVERIFY2(m_view.get() != 0, "Could not create a PropertyTree object.");

  QList<QGraphicsItem*> graphicsItems = m_view->scene()->selectedItems();

  std::vector<Property> props;
  foreach(QGraphicsItem* item, graphicsItems)
  {
    AbstractItemView* view = dynamic_cast<AbstractItemView*>(item);
    Properties itemProperties = view->getProperties();
    props = itemProperties.getProperties();
  }

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

