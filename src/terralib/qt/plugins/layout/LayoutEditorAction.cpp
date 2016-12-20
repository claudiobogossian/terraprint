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
  \file terralib/qt/plugins/layout/LayoutEditorAction.cpp

  \brief This file defines the LayoutEditor class
*/

// Terralib
#include "LayoutEditorAction.h"
#include <terralib/qt/af/ApplicationController.h>
#include "../../../layout/qt/default/MainLayout.h"
#include "terralib/common/TerraLib.h"
#include "../../../layout/core/Config.h"
#include "../../../layout/qt/default/DisplayDock.h"
#include "../../../layout/qt/default/PropertiesDock.h"
#include "../../../layout/qt/default/EditTemplateDock.h"
#include "../../../layout/qt/default/ObjectInspectorDock.h"
#include "../../../layout/qt/outside/ToolbarOutside.h"
#include "terralib/common/progress/ProgressManager.h"
#include "../../../layout/core/enum/Enums.h"

#include "ProxyProject.h"

// Qt
#include <QObject>
#include <QMenu>
#include <QMenuBar>
#include <QGroupBox>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGraphicsScene>

// STL
#include <memory>

te::qt::plugins::layout::LayoutEditorAction::LayoutEditorAction(QMenu* menu)
  : te::qt::plugins::layout::AbstractAction(menu),
   m_dockLayoutDisplay(0),
   m_mainLayout(0),
   m_groupBox(0),
   m_statusBar(0),
   m_verticalLayout(0)
{
  createAction(tr("Layout Editor..."));
}

te::qt::plugins::layout::LayoutEditorAction::~LayoutEditorAction()
{
  TerraLib::getInstance().remove(TE_LAYOUT_MODULE_NAME);

  m_menu->clear();
}

void te::qt::plugins::layout::LayoutEditorAction::onActionActivated(bool checked)
{
  ProxyProject* proxyProject = new ProxyProject;

  QMainWindow* mw = dynamic_cast<QMainWindow*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

  QSize size = mw->centralWidget()->size();
  QRect screen = mw->centralWidget()->geometry();

  m_mainLayout = new te::layout::MainLayout(proxyProject);
  m_mainLayout->init(size, screen);

  m_dockLayoutDisplay = new te::layout::DisplayDock;

  m_statusBar = m_mainLayout->getStatusBar();

  te::layout::View* view = m_mainLayout->getView();

  m_verticalLayout = new QVBoxLayout;
  m_verticalLayout->addWidget(view);
  m_verticalLayout->addWidget(m_statusBar);

  m_groupBox = new QGroupBox(m_dockLayoutDisplay);
  m_groupBox->setLayout(m_verticalLayout);

  m_dockLayoutDisplay->setWidget(m_groupBox);
  m_dockLayoutDisplay->setPreviousCentralWidget(mw->centralWidget());
  m_dockLayoutDisplay->setParent(mw);
  
  mw->setCentralWidget(m_dockLayoutDisplay);
  m_dockLayoutDisplay->setVisible(true);

  m_mainLayout->postInit();
  mw->connect(m_mainLayout, SIGNAL(exit()), this, SLOT(onExit()));

  m_menu->clear();
  createMenu();
  m_mainLayout->getToolbar()->getActionExitButton()->setVisible(false);

  if(m_mainLayout->getProperties())
  {
    m_mainLayout->getProperties()->setParent(mw);
    mw->addDockWidget(Qt::LeftDockWidgetArea, m_mainLayout->getProperties());
    m_mainLayout->getProperties()->setVisible(true);
  }
  if(m_mainLayout->getObjectInspector())
  {
    m_mainLayout->getObjectInspector()->setParent(mw);
    mw->addDockWidget(Qt::LeftDockWidgetArea, m_mainLayout->getObjectInspector());
    m_mainLayout->getObjectInspector()->setVisible(true);
  }
  if(m_mainLayout->getToolbar())
  {
    m_mainLayout->getToolbar()->setParent(mw);
    mw->addToolBar(m_mainLayout->getToolbar());
    m_mainLayout->getToolbar()->setVisible(true);
  }
  if(m_mainLayout->getEditTemplate())
  {
    m_mainLayout->getEditTemplate()->setParent(mw);
    mw->addDockWidget(Qt::LeftDockWidgetArea, m_mainLayout->getEditTemplate());
    bool visible = m_mainLayout->getEditTemplate()->isVisible();
    m_mainLayout->getEditTemplate()->setVisible(visible);
  }

  //disabling taskManager
  te::common::ProgressManager::getInstance().setSuspendViewers(true);
}

void te::qt::plugins::layout::LayoutEditorAction::onExit()
{
  finalizeLayout();
}

void te::qt::plugins::layout::LayoutEditorAction::createMenu()
{
  QMenu* layoutMenu = m_mainLayout->getMenu(m_menu);

  QAction* actionExit = new QAction(tr("Exit"), layoutMenu);
  actionExit->setObjectName("mnu_main_exit");
  actionExit->setIcon(QIcon::fromTheme("layout-close"));
  actionExit->setToolTip("");
  layoutMenu->addAction(actionExit);
}

void te::qt::plugins::layout::LayoutEditorAction::finalizeLayout(bool shutdown)
{
  QMainWindow* mw = dynamic_cast<QMainWindow*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

  bool saveTempFile = false;

  if (m_dockLayoutDisplay && !shutdown)
  {
    QGraphicsScene* sc = m_mainLayout->getView()->scene();
    if (sc)
    {
      if (sc->items().count() > 1)
      {
        int answer = QMessageBox::question(mw, tr("Close"), tr("You have been editing a map. If you leave before saving, your changes will be lost. Do you want to save before you leave?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (answer == QMessageBox::Yes)
        {
          saveTempFile = true;
        }
      }
    }

    // export template
    if (saveTempFile)
    {
      bool cancel = false;
      te::layout::EnumTemplateType* enumTemplate = te::layout::Enums::getInstance().getEnumTemplateType();
      m_mainLayout->getView()->exportTemplate(enumTemplate->getXmlType(), cancel); // export to xml
    }
  }

  m_menu->clear();

  closeLayout(shutdown);

  if (m_mainLayout)
  {
    delete m_mainLayout;
    m_mainLayout = 0;
  }

  createAction(tr("Layout Editor..."));

  //enabling taskManager
  te::common::ProgressManager::getInstance().setSuspendViewers(false);
}

void te::qt::plugins::layout::LayoutEditorAction::closeLayout(bool shutdown)
{
  QMainWindow* mw = dynamic_cast<QMainWindow*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

  if (m_mainLayout)
  {
    if (m_mainLayout->getProperties())
    {
      mw->removeDockWidget(m_mainLayout->getProperties());
      m_mainLayout->getProperties()->close();
    }

    if (m_mainLayout->getObjectInspector())
    {
      mw->removeDockWidget(m_mainLayout->getObjectInspector());
      m_mainLayout->getObjectInspector()->close();
    }

    if (m_mainLayout->getToolbar())
    {
      mw->removeToolBar(m_mainLayout->getToolbar());
      m_mainLayout->getToolbar()->close();
    }

    if (m_mainLayout->getEditTemplate())
    {
      mw->removeDockWidget(m_mainLayout->getEditTemplate());
      m_mainLayout->getEditTemplate()->close();

    }

    if (m_dockLayoutDisplay)
    {
      if (shutdown) // shutdown plugin
      {
        m_dockLayoutDisplay->setPreviousCentralWidget(0);
      }
      mw->removeDockWidget(m_dockLayoutDisplay);
      m_dockLayoutDisplay->close();
      delete m_dockLayoutDisplay;
      m_dockLayoutDisplay = 0;
    }
  }
}

