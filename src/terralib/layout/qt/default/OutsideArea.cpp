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
  \file OutsideArea.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "OutsideArea.h"
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
#include "MenuPrincipal.h"

// STL
#include <string>

// Qt
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QStatusBar>

te::layout::OutsideArea::OutsideArea(AbstractProxyProject* proxyProject, te::layout::View* view, QStatusBar* status) :
  m_dockProperties(0),
  m_dockInspector(0),
  m_dockEditTemplate(0),
  m_view(view),
  m_toolbar(0),
  m_statusBar(status),
  m_menuPrincipal(0),
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
  init(proxyProject);
}

te::layout::OutsideArea::~OutsideArea()
{
  if(m_dockProperties)
  {

    m_dockProperties->close();
    m_dockProperties->setParent(0);
    delete m_dockProperties;
    m_dockProperties = 0;
  }

  if(m_dockInspector)
  {

    m_dockInspector->close();
    m_dockInspector->setParent(0);
    delete m_dockInspector;
    m_dockInspector = 0;
  }

  if(m_toolbar)
  {

    m_toolbar->close();
    m_toolbar->setParent(0);
    delete m_toolbar;
    m_toolbar = 0;
  }

  if(m_dockEditTemplate)
  {

    m_dockEditTemplate->close();
    m_dockEditTemplate->setParent(0);
    delete m_dockEditTemplate;
    m_dockEditTemplate = 0;
  }
}

void te::layout::OutsideArea::init(AbstractProxyProject* proxyProject)
{
  if(m_view)
  {
    connect(m_view, SIGNAL(loadProperties()), this, SLOT(onSelectionChanged()));
    connect(m_view, SIGNAL(reloadProperties()), this, SLOT(onPropertiesChanged()));
    connect(m_view->scene(), SIGNAL(addItemFinalized(QGraphicsItem*)), this, SLOT(onAddItemFinalized(QGraphicsItem*)));
    connect(m_view, SIGNAL(closeView()), this, SLOT(onCloseView()));
    connect(m_view, SIGNAL(showView()), this, SLOT(onShowView()));
    connect(m_view, SIGNAL(changeContext()), this, SLOT(onRefreshStatusBar()));
    connect(m_view->scene(), SIGNAL(editionFinalized()), this, SLOT(onEditionFinalized()));
    connect(m_view->scene(), SIGNAL(editionInitialized()), this, SLOT(onEditionInitialized()));

    addAllItemToolbars();
  }
  
  createPropertiesDock(proxyProject);

  createInspectorDock();
  
  createToolbar();

  createEditTemplateDock();

  if(m_toolbar)
  {
    connect(m_toolbar, SIGNAL(changeMode(te::layout::EnumType*)), m_view, SLOT(onToolbarChangeMode(te::layout::EnumType*)));
    connect(m_toolbar, SIGNAL(zoomChangedInComboBox(int)), m_view, SLOT(setZoom(int)));

    te::layout::ToolbarController* controller = dynamic_cast<te::layout::ToolbarController*>(m_toolbar->getController());
    connect(m_view, SIGNAL(zoomChanged(int)), controller, SLOT(onZoomChanged(int)));
  }

  if(m_dockInspector)
  {
    connect(m_view->scene(), SIGNAL(deleteFinalized(std::vector<std::string>)), this, SLOT(onDeleteFinalized(std::vector<std::string>)));

    connect(m_dockInspector->getObjectInspectorOutside(), SIGNAL(selectionChanged(QList<QGraphicsItem*>)), this, SLOT(onSelectionChanged(QList<QGraphicsItem*>)));
  }

  if(m_dockProperties)
  {
    connect(m_view->scene(), SIGNAL(deleteFinalized(std::vector<std::string>)), 
      m_dockProperties->getPropertiesOutside(), SLOT(onClear(std::vector<std::string>)));
  }
}

void te::layout::OutsideArea::createPropertiesDock(AbstractProxyProject* proxyProject)
{
  m_dockProperties = new PropertiesDock(proxyProject, m_view->getScene());  
  m_dockProperties->setFeatures(QDockWidget::DockWidgetMovable |  
    QDockWidget::DockWidgetFloatable);
}

void te::layout::OutsideArea::createInspectorDock()
{
  m_dockInspector = new ObjectInspectorDock(m_view->getScene());
  m_dockInspector->setFeatures(QDockWidget::DockWidgetMovable |  
    QDockWidget::DockWidgetFloatable);
}

void te::layout::OutsideArea::createToolbar()
{
  te::layout::BuildGraphicsOutside buildOutside;

  te::layout::EnumObjectType* objectType = te::layout::Enums::getInstance().getEnumObjectType();
  if(!objectType)
  {
    return;
  }

  QWidget* widget = buildOutside.createOutside(objectType->getToolbar(), m_view->getScene(), (QWidget*) m_view->parent());
  if(!widget)
  {
    return;
  }

  m_toolbar = dynamic_cast<te::layout::ToolbarOutside*>(widget);
}

void te::layout::OutsideArea::createEditTemplateDock()
{
  m_dockEditTemplate = new EditTemplateDock(m_view->getScene());
  m_dockEditTemplate->setFeatures(QDockWidget::NoDockWidgetFeatures);
  m_dockEditTemplate->setVisible(false);
}

te::layout::PropertiesDock* te::layout::OutsideArea::getPropertiesDock()
{
  return m_dockProperties;
}

te::layout::ObjectInspectorDock* te::layout::OutsideArea::getObjectInspectorDock()
{
  return m_dockInspector;
}

te::layout::ToolbarOutside* te::layout::OutsideArea::getToolbar()
{
  return m_toolbar;
}

te::layout::EditTemplateDock* te::layout::OutsideArea::getEditTemplate()
{
  return m_dockEditTemplate;
}

void te::layout::OutsideArea::openAllDocks()
{
  if(m_dockProperties)
  {
    m_dockProperties->setVisible(true);
  }
  if(m_dockInspector)
  {
    m_dockInspector->setVisible(true);
  }
  if(m_toolbar)
  {
    m_toolbar->setVisible(true);
  }
  if(m_dockEditTemplate)
  {
    bool visible = m_dockEditTemplate->isVisible();
    m_dockEditTemplate->setVisible(visible);
  }
}

void te::layout::OutsideArea::closeAllDocks()
{
   if(m_dockProperties)
   {
       m_dockProperties->close();
   }
   if(m_dockInspector)
   {
       m_dockInspector->close();
   }
   if(m_toolbar)
   {
       m_toolbar->close();
   }
   if(m_dockEditTemplate)
   {
       m_dockEditTemplate->close();
   }
}

void te::layout::OutsideArea::onSelectionChanged()
{  
  QList<QGraphicsItem*> graphicsItems = m_view->scene()->selectedItems();
  QList<QGraphicsItem*> allItems = m_view->scene()->items();

  Scene* myScene = dynamic_cast<Scene*>(m_view->scene());
  if (myScene != 0)
  {
    QGraphicsItem* subSelectedItem = myScene->getSubSelectedItem();
    if (subSelectedItem != 0)
    {
      graphicsItems.clear();
      graphicsItems.append(subSelectedItem);
    }
  }

  //Refresh Property window   
  if(m_dockProperties)
    m_dockProperties->getPropertiesOutside()->itemsSelected(graphicsItems, allItems);

  if(m_dockInspector)
    m_dockInspector->getObjectInspectorOutside()->selectItems(graphicsItems);
}

void te::layout::OutsideArea::onPropertiesChanged()
{
  QList<QGraphicsItem*> graphicsItems = m_view->scene()->selectedItems();
  QList<QGraphicsItem*> allItems = m_view->scene()->items();

  Scene* myScene = dynamic_cast<Scene*>(m_view->scene());
  if (myScene != 0)
  {
    QGraphicsItem* subSelectedItem = myScene->getSubSelectedItem();
    if (subSelectedItem != 0)
    {
      graphicsItems.clear();
      graphicsItems.append(subSelectedItem);
    }
  }

  //Refresh Property window   
  if (m_dockProperties)
    m_dockProperties->getPropertiesOutside()->propertiesChanged(graphicsItems, allItems);

  if (m_dockInspector)
    m_dockInspector->getObjectInspectorOutside()->selectItems(graphicsItems);
}

void te::layout::OutsideArea::onAddItemFinalized(QGraphicsItem* item)
{
  QList<QGraphicsItem*> allItems = m_view->scene()->items();
  //Refresh Inspector Object window
  if(m_dockInspector)
    m_dockInspector->getObjectInspectorOutside()->itemsInspector(allItems);
}

void te::layout::OutsideArea::onSelectionChanged(QList<QGraphicsItem*> selectedItems)
{
  m_view->scene()->clearSelection();
  foreach(QGraphicsItem* item, selectedItems) 
  {
    item->setSelected(true);
    if (item->parentItem() != 0)
    {
      AbstractItemView* absItem = dynamic_cast<AbstractItemView*>(item);
      if (absItem != 0)
      {
        absItem->setSubSelection(true);
      }
    }
  }

  QList<QGraphicsItem*> allItems = m_view->scene()->items();
  if(m_dockProperties)
    m_dockProperties->getPropertiesOutside()->itemsSelected(selectedItems, allItems);
}

void te::layout::OutsideArea::onShowView()
{
  openAllDocks();
  if (m_menuPrincipal)
  {
    m_menuPrincipal->openMainMenu();
  }
}

void te::layout::OutsideArea::onCloseView()
{
  closeAllDocks();
  if (m_menuPrincipal)
  {
    m_menuPrincipal->closeMainMenu();
  }
  m_view->closeOutsideWindows();
}

void te::layout::OutsideArea::onRefreshStatusBar()
{
  if(!m_statusBar)
  {
    return;
  }

  te::layout::EnumType* mode = m_view->getCurrentMode();

  QString msg("");

  if(mode == te::layout::Enums::getInstance().getEnumModeType()->getModeNone())
  {
    msg = tr("TerraPrint - TerraLib 5");
    m_statusBar->showMessage(msg);
    return;
  }
  
  msg = tr("TerraPrint - TerraLib 5 | Context: ");

  std::string s_mode = mode->getLabel();
  if(s_mode.compare("") == 0)
  {
    s_mode = mode->getName();
  }

  QString qMode = ItemUtils::convert2QString(s_mode);

  msg += qMode;
  m_statusBar->showMessage(msg);
}

void te::layout::OutsideArea::onAddChildFinalized( QGraphicsItem* parent, QGraphicsItem* child )
{
  QList<QGraphicsItem*> allItems = m_view->scene()->items();
  //Refresh Inspector Object window
  if(m_dockInspector)
    m_dockInspector->getObjectInspectorOutside()->itemsInspector(allItems);
}

void te::layout::OutsideArea::onDeleteFinalized(std::vector<std::string>)
{
  QList<QGraphicsItem*> allItems = m_view->scene()->items();
  //Refresh Inspector Object window
  if(m_dockInspector)
    m_dockInspector->getObjectInspectorOutside()->itemsInspector(allItems);
}

te::layout::MenuPrincipal* te::layout::OutsideArea::getMenuPrincipal(QMenu* parentMenu)
{
  if (!m_menuPrincipal && parentMenu)
  {
    m_menuPrincipal = new MenuPrincipal(m_view, parentMenu);
    m_menuPrincipal->setDockProperties(m_dockProperties);
    m_menuPrincipal->setDockInspector(m_dockInspector);
    m_menuPrincipal->setDockEditTemplate(m_dockEditTemplate);
    //exit
    connect(m_menuPrincipal, SIGNAL(exit()), this, SLOT(onExit()));
  }
  return m_menuPrincipal;
}

void te::layout::OutsideArea::onEditionFinalized()
{
  if(m_dockProperties)
  {
    m_dockProperties->getPropertiesOutside()->setEnabled(true);
  }
  if(m_dockInspector)
  {
    m_dockInspector->getObjectInspectorOutside()->setEnabled(true);
  }
}

void te::layout::OutsideArea::onEditionInitialized()
{
  if(m_dockProperties)
  {
    m_dockProperties->getPropertiesOutside()->setDisabled(true);
  }
  if(m_dockInspector)
  {
    m_dockInspector->getObjectInspectorOutside()->setDisabled(true);
  }
}

void te::layout::OutsideArea::addAllItemToolbars()
{
  EnumObjectType* object = Enums::getInstance().getEnumObjectType();

  MapToolbarController* controller = new MapToolbarController;
  MapToolbarInside* inside = new MapToolbarInside(controller);

  m_view->addToolbarItemInside(object->getMapItem(), inside);
}

void te::layout::OutsideArea::onExit()
{
  emit exit();
}

