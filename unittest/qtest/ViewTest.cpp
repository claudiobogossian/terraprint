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

#include "ViewTest.h"

// Layout Module
#include "terralib/layout/qt/core/Scene.h"
#include "terralib/layout/qt/item/RectangleItem.h"
#include "terralib/layout/core/pattern/proxy/AbstractProxyProject.h"
#include "terralib/layout/qt/outside/ToolbarOutside.h"

// Qt
#include <QtTest/QtTest>
#include <QSize>
#include <QPoint>
#include <QString>
#include <QToolButton>
#include <QAction>
#include <QList>
#include <QMenu>
#include <QStatusBar>

void te::layout::qtest::ViewTest::on_test_create_view()
{
  m_view.reset(new te::layout::View);
  QVERIFY2(m_view.get() != 0, "Could not create a View object.");
}

void te::layout::qtest::ViewTest::on_test_create_scene()
{
  te::layout::Scene* myScene = new te::layout::Scene(m_view.get());
  m_view->setScene(myScene);

  QVERIFY2(m_view->getScene() != 0, "Could not create a Scene object.");
  m_view->config();
}

void te::layout::qtest::ViewTest::on_test_click_view()
{
  QPoint clickPoint1(100, 80);
  QTest::mouseClick(m_view->viewport(), Qt::LeftButton, 0, clickPoint1);

  QPoint clickPoint2(75, 105);
  QTest::mouseClick(m_view->viewport(), Qt::LeftButton, 0, clickPoint2);
}

void te::layout::qtest::ViewTest::on_test_create_outside_area()
{
  m_outsideArea.reset(new OutsideArea(0, m_view.get()));
  QVERIFY2(m_outsideArea.get() != 0, "Could not create a OutsideArea object.");
}

void te::layout::qtest::ViewTest::on_test_create_rectangle_item()
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

void te::layout::qtest::ViewTest::on_test_click_rectangle_item()
{
  // Test: select rectangle item
  QPoint clickPoint(75, 105);
  QTest::mouseClick(m_view->viewport(), Qt::LeftButton, 0, clickPoint);
}

void te::layout::qtest::ViewTest::on_test_create_map_composition_item()
{
  QString rectName = m_outsideArea->getToolbar()->getActionMapDefault();

  QToolButton* toolButton = m_outsideArea->getToolbar()->getMapToolButton();
  QVERIFY2(toolButton != 0, "The map tool toolbutton object is null.");

  QMenu* mnu = toolButton->menu();
  QVERIFY2(mnu != 0, "The menu of map tool toolbutton object is null.");

  QList<QAction*> actions = mnu->actions();
  QAction* mapActionMenu = 0;
  for (int i = 0; i < actions.size(); ++i)
  {
    QAction* action = actions.value(i);
    if (action->objectName().compare(rectName) == 0)
    {
      mapActionMenu = action;
      break;
    }
  }

  QVERIFY2(mapActionMenu != 0, "The map composition action of the menu of map tool toolbutton object is null.");

  // Test: click on map menu
  QTest::mouseClick(mnu, Qt::LeftButton, 0, mnu->geometry().center(), 300);
  // Test: click on map composition item action (select map composition item for creation)
  QTest::mouseClick(mnu, Qt::LeftButton, 0, mnu->actionGeometry(mapActionMenu).center(), 300);

  /* Map Composition: MapComposition (group) + MapItem + GridGeodesic + GridPlanar  */

  // Test: create map composition item via mouse click
  QPoint clickPoint(100, 80);
  QTest::mouseClick(m_view->viewport(), Qt::LeftButton, 0, clickPoint);
}

void te::layout::qtest::ViewTest::on_test_click_map_composition_item()
{
  // Test: select map composition item
  QPoint clickPoint(100, 80);
  QTest::mouseClick(m_view->viewport(), Qt::LeftButton, 0, clickPoint);
}

void te::layout::qtest::ViewTest::on_test_verify_items()
{
  int total = m_view->getScene()->items().size();
  QVERIFY2(total == 6, "Wrong number of created objects, inconsistent scene.");
}

