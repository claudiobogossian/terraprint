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

// Qt
#include <QtTest/QtTest>
#include <QDate>
#include <QDateEdit>
#include <QSize>
#include <QPoint>
#include <QString>

Q_DECLARE_METATYPE(QDate)

void te::layout::ViewTest::testChanges()
{
  // 11 March 1967
  QDate date(1967, 3, 11);
  QDateEdit dateEdit(date);
  
  // up-arrow should increase day by one
  QTest::keyClick(&dateEdit, Qt::Key_Up);
  QCOMPARE(dateEdit.date(), date.addDays(1));
  
  // we click twice on the "reduce" arrow at the bottom RH corner
  // first we need the widget size to know where to click
  QSize editWidgetSize = dateEdit.size();
  QPoint clickPoint(editWidgetSize.rwidth() - 2, editWidgetSize.rheight() - 2);
  // issue two clicks
  QTest::mouseClick(&dateEdit, Qt::LeftButton, 0, clickPoint);
  QTest::mouseClick(&dateEdit, Qt::LeftButton, 0, clickPoint);
  // and we should have decreased day by two (one less than original)
  QCOMPARE(dateEdit.date(), date.addDays(-1));
  
  QTest::keyClicks(&dateEdit, "25122005");
  QCOMPARE(dateEdit.date(), QDate(2005, 12, 25));
  
  QTest::keyClick(&dateEdit, Qt::Key_Tab, Qt::ShiftModifier);
  QTest::keyClicks(&dateEdit, "08");
  QCOMPARE(dateEdit.date(), QDate(2005, 8, 25));
}

void te::layout::ViewTest::testValidator_data()
{
  qRegisterMetaType<QDate>("QDate");
  
  QTest::addColumn<QDate>("initialDate");
  QTest::addColumn<QString>("keyclicks");
  QTest::addColumn<QDate>("finalDate");
  
  QTest::newRow("1968/4/12") << QDate(1967, 3, 11)
  << QString("12041968")
  << QDate(1968, 4, 12);
  
  QTest::newRow("1967/3/14") << QDate(1967, 3, 11)
  << QString("140abcdef[")
  << QDate(1967, 3, 14);
  // more rows can go in here
}

void te::layout::ViewTest::testValidator()
{
  QFETCH(QDate, initialDate);
  QFETCH(QString, keyclicks);
  QFETCH(QDate, finalDate);
  
  QDateEdit dateEdit(initialDate);
  // this next line is just to start editing
  QTest::keyClick(&dateEdit, Qt::Key_Enter);
  QTest::keyClicks(&dateEdit, keyclicks);
  QCOMPARE(dateEdit.date(), finalDate);
}

