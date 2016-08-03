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

#ifdef STAND_ALONE
#define BOOST_TEST_MODULE TempDataStorageEditorTest
#endif

// Layout Module
#include "terralib/layout/qt/core/tempDataStorage/TempDataStorageEditor.h"
#include "terralib/layout/qt/core/tempDataStorage/TempFileInfo.h"
#include "terralib/layout/qt/core/pattern/command/AddCommand.h"
#include "terralib/layout/core/enum/Enums.h"
#include "terralib/layout/qt/core/Scene.h"
#include "terralib/layout/qt/core/View.h"
#include "terralib/layout/qt/core/ItemUtils.h"

// STL
#include <string>

// Qt
#include <QDir>
#include <QUndoStack>
#include <QUndoCommand>
#include <QGraphicsItem>

// Boost
#include <boost/test/unit_test.hpp>

void getTestPath(std::string& path)
{
  // User folder path
  // Whether a directory separator is added to the end or not, depends on the operating system.
  QString newPath = QDir(QDir::tempPath()).filePath("TerraPrint");

  QDir dir(newPath);
  if (!dir.exists())
  {
    bool result = dir.mkpath(newPath); // create a new path or directory
    if (result)
    {
      path = "";
    }
  }

  QString fullNewPath = newPath + "/~layoutTemp.xml";
  path = fullNewPath.toLatin1().constData();
}

BOOST_AUTO_TEST_SUITE(test_suite_temp_data_storage)

BOOST_AUTO_TEST_CASE(test_create_view)
{
  te::layout::View view;
  BOOST_TEST_MESSAGE("View object successfully created!");
}

BOOST_AUTO_TEST_CASE(test_create_scene)
{
  te::layout::View view;

  BOOST_TEST_MESSAGE("View object successfully created!");

  te::layout::Scene* myScene = new te::layout::Scene(&view);
  view.setScene(myScene);

  BOOST_REQUIRE_MESSAGE(view.getScene() != 0, "Scene null pointer."); //throws on error
  BOOST_TEST_MESSAGE("Scene object successfully created!");

  view.config();

  BOOST_TEST_MESSAGE("Configuration View successfully done!");
}

BOOST_AUTO_TEST_CASE(test_path)
{
  // User folder path
  // Whether a directory separator is added to the end or not, depends on the operating system.
  QString newPath = QDir(QDir::tempPath()).filePath("TerraPrint");

  QDir dir(newPath);
  BOOST_CHECK_MESSAGE(dir.exists() == true, "dir.exists(): " << dir.exists());

  if (!dir.exists())
  {
    bool result = dir.mkpath(newPath); // create a new path or directory
    if (result)
    {
      BOOST_FAIL("FAIL: create a new directory - TerraPrint"); //throws on error
    }
  }
}

BOOST_AUTO_TEST_CASE(test_create_editor)
{
  // Create View and Scene
  te::layout::View view;

  BOOST_TEST_MESSAGE("View object successfully created!");

  te::layout::Scene* myScene = new te::layout::Scene(&view);
  view.setScene(myScene);

  BOOST_REQUIRE(view.getScene() != 0); //throws on error

  BOOST_TEST_MESSAGE("Scene object successfully created!");

  view.config();

  BOOST_TEST_MESSAGE("Configuration View successfully done!");

  std::string path;
  getTestPath(path); // get path
  if (path.compare("") == 0)
  {
    BOOST_FAIL("Path not found."); //throws on error
  }

  BOOST_TEST_MESSAGE("Path found!");

  te::layout::EnumTempDataStorageType* type = te::layout::Enums::getInstance().getEnumTempDataStorageType();

  if (!type)
  {
    BOOST_FAIL("FAIL: EnumTempDataStorageType null pointer."); //throws on error
  }

  te::layout::TempFileInfo* info = new te::layout::TempFileInfo(view.getScene(), path);

  BOOST_REQUIRE(info != 0); //throws on error

  te::layout::TempDataStorageEditor tempData(view.getScene()->getUndoStack(), type->getTempFileType(), info, false);

  BOOST_TEST_MESSAGE("TempDataStorageEditor object successfully created!");

  tempData.stop();

  BOOST_TEST_MESSAGE("TempDataStorageEditor: Threads successfully stopped!");
}

BOOST_AUTO_TEST_CASE(test_synchronous)
{
  // Create View and Scene
  te::layout::View view;

  BOOST_TEST_MESSAGE("View object successfully created!");

  te::layout::Scene* myScene = new te::layout::Scene(&view);
  view.setScene(myScene);

  BOOST_REQUIRE(view.getScene() != 0); //throws on error

  BOOST_TEST_MESSAGE("Scene object successfully created!");

  view.config();

  BOOST_TEST_MESSAGE("Configuration View successfully done!");

  std::string path;
  getTestPath(path); // get path
  if (path.compare("") == 0)
  {
    BOOST_FAIL("Path not found."); //throws on error
  }

  BOOST_TEST_MESSAGE("Path found!");

  te::layout::EnumTempDataStorageType* type = te::layout::Enums::getInstance().getEnumTempDataStorageType();

  if (!type)
  {
    BOOST_FAIL("FAIL: EnumTempDataStorageType null pointer."); //throws on error
  }

  te::layout::TempFileInfo* info = new te::layout::TempFileInfo(view.getScene(), path);

  BOOST_REQUIRE(info != 0); //throws on error

  // synchronous
  te::layout::TempDataStorageEditor tempData(view.getScene()->getUndoStack(), type->getTempFileType(), info, false);

  BOOST_TEST_MESSAGE("TempDataStorageEditor (synchronous) object successfully created!");

  QGraphicsItem* item = 0;

  /* Commands added to the undo/redo to trigger the execution of the threads. 
  In this case one at a time (synchronous). */

  QUndoCommand* command1 = new te::layout::AddCommand(item);
  view.getScene()->addUndoStack(command1);

  BOOST_TEST_MESSAGE("AddCommand created!");

  QUndoCommand* command2 = new te::layout::AddCommand(item);
  view.getScene()->addUndoStack(command2);

  BOOST_TEST_MESSAGE("AddCommand created!");

  QUndoCommand* command3 = new te::layout::AddCommand(item);
  view.getScene()->addUndoStack(command3);

  BOOST_TEST_MESSAGE("AddCommand created!");

  tempData.stop();

  BOOST_TEST_MESSAGE("TempDataStorageEditor (synchronous): Threads successfully stopped!");
}

BOOST_AUTO_TEST_CASE(test_asynchronous)
{
  // Create View and Scene
  te::layout::View view;

  BOOST_TEST_MESSAGE("View object successfully created!");

  te::layout::Scene* myScene = new te::layout::Scene(&view);
  view.setScene(myScene);

  BOOST_REQUIRE(view.getScene() != 0); //throws on error

  BOOST_TEST_MESSAGE("Scene object successfully created!");

  view.config();

  BOOST_TEST_MESSAGE("Configuration View successfully done!");

  std::string path;
  getTestPath(path); // get path
  if (path.compare("") == 0)
  {
    BOOST_FAIL("Path not found."); //throws on error
  }

  BOOST_TEST_MESSAGE("Path found!");

  te::layout::EnumTempDataStorageType* type = te::layout::Enums::getInstance().getEnumTempDataStorageType();

  if (!type)
  {
    BOOST_FAIL("FAIL: EnumTempDataStorageType null pointer."); //throws on error
  }

  te::layout::TempFileInfo* info = new te::layout::TempFileInfo(view.getScene(), path);

  BOOST_REQUIRE(info != 0); //throws on error

  // asynchronous (default)
  te::layout::TempDataStorageEditor tempData(view.getScene()->getUndoStack(), type->getTempFileType(), info);

  BOOST_TEST_MESSAGE("TempDataStorageEditor (asynchronous) object successfully created!");

  QGraphicsItem* item = 0;

  /* Commands added to the undo/redo to trigger the execution of the threads.
  In this case concurrent threads (asynchronous). */

  QUndoCommand* command1 = new te::layout::AddCommand(item);
  view.getScene()->addUndoStack(command1);

  BOOST_TEST_MESSAGE("AddCommand created!");

  QUndoCommand* command2 = new te::layout::AddCommand(item);
  view.getScene()->addUndoStack(command2);

  BOOST_TEST_MESSAGE("AddCommand created!");

  QUndoCommand* command3 = new te::layout::AddCommand(item);
  view.getScene()->addUndoStack(command3);

  BOOST_TEST_MESSAGE("AddCommand created!");

  tempData.stop();

  BOOST_TEST_MESSAGE("TempDataStorageEditor (asynchronous): Threads successfully stopped!");
}

BOOST_AUTO_TEST_SUITE_END()
