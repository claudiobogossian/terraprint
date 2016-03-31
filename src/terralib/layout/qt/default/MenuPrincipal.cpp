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
  \file MenuPrincipal.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MenuPrincipal.h"
#include "PropertiesDock.h"
#include "ObjectInspectorDock.h"
#include "EditTemplateDock.h"
#include "../../../layout/qt/outside/PropertiesOutside.h"
#include "../../../layout/qt/outside/ObjectInspectorOutside.h"
#include "../../../layout/core/enum/Enums.h"
#include "../../../layout/qt/core/BuildGraphicsItem.h"
#include "../../../layout/qt/core/BuildGraphicsOutside.h"
#include "terralib/layout/qt/outside/ToolbarController.h"
#include "../../core/pattern/proxy/AbstractProxyProject.h"
#include "../inside/MapToolbarInside.h"
#include "../inside/MapToolbarController.h"
#include "../inside/ToolbarItemInside.h"

// STL
#include <string>

// Qt
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QStatusBar>

te::layout::MenuPrincipal::MenuPrincipal(te::layout::View* view, QMenu* parentMenu) :
  m_dockProperties(0),
  m_dockInspector(0),
  m_dockEditTemplate(0),
  m_view(view),
  m_layoutMenu(parentMenu),
  m_optionNew("mnu_main_new"),
  m_optionUpdate("mnu_main_update"),
  m_optionImportXml("mnu_main_import_xml"),
  m_optionExportXml("mnu_main_export_xml"),
  m_optionPageConfig("mnu_main_page_config"),
  m_optionPrint("mnu_main_print"),
  m_optionExit("mnu_main_exit"),
  m_optionDockInspector("mnu_dock_inspector"),
  m_optionDockProperties("mnu_dock_properties"),
  m_optionDockToolbar("mnu_dock_toolbar"),
  m_optionDockEditTemplate("mnu_dock_edit_template")
{
  init();
}

te::layout::MenuPrincipal::~MenuPrincipal()
{
  
}

void te::layout::MenuPrincipal::init()
{
  createMainMenu();
}

void te::layout::MenuPrincipal::createMainMenu()
{
  if(!m_layoutMenu)
  {
    m_layoutMenu = new QMenu(tr("Print Model"));
  }

  connect(m_layoutMenu, SIGNAL(triggered(QAction*)), this, SLOT(onMainMenuTriggered(QAction*)));

  QAction* actionNew = createAction(tr("New"), m_optionNew, "layout-new");
  m_layoutMenu->addAction(actionNew);

  QAction* actionSave = createAction(tr("Update Map"), m_optionUpdate, "layout-save");
  m_layoutMenu->addAction(actionSave);

  m_layoutMenu->addSeparator();

  QMenu* mnuImport = m_layoutMenu->addMenu(tr("Import Map"));
  QMenu* mnuExport = m_layoutMenu->addMenu(tr("Export Map"));

  QAction* actionImportJSON = createAction(tr("Import Xml Map"), m_optionImportXml, "layout-import");
  mnuImport->addAction(actionImportJSON);

  QAction* actionExportJSON = createAction(tr("Export XML Map"), m_optionExportXml, "layout-export");
  mnuExport->addAction(actionExportJSON);

  m_layoutMenu->addSeparator();

  QAction* actionDockInspector = createAction(tr("Dock Inspector"), m_optionDockInspector, "");
  actionDockInspector->setCheckable(true);
  actionDockInspector->setChecked(true);
  m_layoutMenu->addAction(actionDockInspector);

  QAction* actionDockProperties = createAction(tr("Dock Properties"), m_optionDockProperties, "");
  actionDockProperties->setCheckable(true);
  actionDockProperties->setChecked(true);
  m_layoutMenu->addAction(actionDockProperties);

  m_layoutMenu->addSeparator();

  QAction* actionPageConfig = createAction(tr("Page Config..."), m_optionPageConfig, "layout-page-setup");
  m_layoutMenu->addAction(actionPageConfig);

  QAction* actionPrint = createAction(tr("Print..."), m_optionPrint, "layout-printer");
  m_layoutMenu->addAction(actionPrint);

  m_layoutMenu->addSeparator();
}

void te::layout::MenuPrincipal::onMainMenuTriggered(QAction* action)
{
  if (!m_dockProperties || !m_dockInspector || !m_dockEditTemplate)
    return;

  if(action->objectName().compare(m_optionNew) == 0)
  {
    m_view->newTemplate();
  }
  else if(action->objectName().compare(m_optionUpdate) == 0)
  {
    //changeAction will be TypeSaveCurrentTemplate
  }
  else if(action->objectName().compare(m_optionImportXml) == 0)
  {
    te::layout::EnumTemplateType* enumTemplate = te::layout::Enums::getInstance().getEnumTemplateType();
    m_view->importTemplate(enumTemplate->getXmlType());
  }
  else if(action->objectName().compare(m_optionExportXml) == 0)
  {
    te::layout::EnumTemplateType* enumTemplate = te::layout::Enums::getInstance().getEnumTemplateType();
    m_view->exportProperties(enumTemplate->getXmlType());
  }
  else if(action->objectName().compare(m_optionPageConfig) == 0)
  {    
    m_view->showPageSetup();
  }
  else if(action->objectName().compare(m_optionPrint) == 0)
  {
    m_view->print();
  }
  else if(action->objectName().compare(m_optionExit) == 0)
  {
    m_view->close();
    emit exit();
  }
  else if(action->objectName().compare(m_optionDockInspector) == 0)
  {
    if(m_dockInspector->isVisible())
    {
      m_dockInspector->setVisible(false);
    }
    else
    {
      m_dockInspector->setVisible(true);
    }
  }
  else if(action->objectName().compare(m_optionDockProperties) == 0)
  {
    if(m_dockProperties->isVisible())
    {
      m_dockProperties->setVisible(false);
    }
    else
    {
      m_dockProperties->setVisible(true);
    }
  }
  else if(action->objectName().compare(m_optionDockEditTemplate) == 0)
  {
    if(m_dockEditTemplate->isVisible())
    {
      m_dockEditTemplate->setVisible(false);
    }
    else
    {
      m_dockEditTemplate->setVisible(true);
    }
  }
}

QAction* te::layout::MenuPrincipal::createAction(const QString& text, const QString& objName, const QString& icon, const QString& tooltip)
{
  QAction *actionMenu = new QAction(text, m_layoutMenu);
  actionMenu->setObjectName(objName);

  actionMenu->setIcon(QIcon::fromTheme(icon));
  actionMenu->setToolTip(tooltip);

  return actionMenu;
}

void te::layout::MenuPrincipal::openMainMenu()
{
  if(!m_layoutMenu)
    return;
  
  QList<QAction*> acts = m_layoutMenu->actions();
 
  foreach(QAction* act, acts)
  {
    act->setVisible(true);
  }

  if(!acts.empty())
  {
    //m_parentMenu: The first QAction of the QMenu is to open the layout
    QAction* firstOption = acts.first();
    firstOption->setVisible(false);
  }
}

void te::layout::MenuPrincipal::closeMainMenu()
{
  if(!m_layoutMenu)
   return;

  QList<QAction*> acts = m_layoutMenu->actions();
  foreach(QAction* act, acts)
  {
    act->setVisible(false);
  }

  if(!acts.empty())
  {
    //m_parentMenu: The first QAction of the QMenu is to open the layout
    QAction* firstOption = acts.first();
    firstOption->setVisible(true);
  }

  m_view->closeOutsideWindows();
}

QMenu* te::layout::MenuPrincipal::getMenu()
{
  return m_layoutMenu;
}

void te::layout::MenuPrincipal::setDockProperties(PropertiesDock* dock)
{
  m_dockProperties = dock;
}

void te::layout::MenuPrincipal::setDockInspector(ObjectInspectorDock* dock)
{
  m_dockInspector = dock;
}

void te::layout::MenuPrincipal::setDockEditTemplate(EditTemplateDock* dock)
{
  m_dockEditTemplate = dock;
}

