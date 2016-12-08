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
  \file PropertiesCentralController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PropertiesCentralController.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"

#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../outside/PropertiesOutside.h"
#include "../outside/ObjectInspectorOutside.h"
#include "../core/ItemUtils.h"
#include "../core/View.h"
#include "../core/propertybrowser/MenuBuilder.h"

// Qt
#include <QGraphicsItem>

te::layout::PropertiesCentralController::PropertiesCentralController(View* view, QObject *parent) :
  QObject(parent),
  m_view(view),
  m_propertiesOutside(0),
  m_objectInspectorOutside(0),
  m_menuBuilder(0)
{
  if (m_view)
  {
    connect(m_view, SIGNAL(loadProperties()), this, SLOT(onSelectionChanged()), Qt::QueuedConnection);
    connect(m_view, SIGNAL(reloadProperties()), this, SLOT(onPropertiesChanged()), Qt::QueuedConnection);
    connect(m_view, SIGNAL(showContextMenu(QPointF, QPointF)), this, SLOT(onShowContextMenu(QPointF, QPointF)));
    connect(m_view->scene(), SIGNAL(addItemFinalized(QGraphicsItem*)), this, SLOT(onAddItemFinalized(QGraphicsItem*)));
    connect(m_view->scene(), SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()), Qt::QueuedConnection);
    connect(m_view, SIGNAL(showDialogWindow(EnumType*, QList<QGraphicsItem*>)), this, SLOT(onShowDialogWindow(EnumType*, QList<QGraphicsItem*>)));
  }
}

te::layout::PropertiesCentralController::~PropertiesCentralController()
{
 
}

void te::layout::PropertiesCentralController::setPropertiesOutside(PropertiesOutside* outside)
{
  m_propertiesOutside = outside;

  if (m_propertiesOutside)
  {
    connect(m_view->scene(), SIGNAL(deleteFinalized(std::vector<std::string>)),
      m_propertiesOutside, SLOT(onClear(std::vector<std::string>)));
  }
}

void te::layout::PropertiesCentralController::setObjectInspector(ObjectInspectorOutside* outside)
{
  m_objectInspectorOutside = outside;

  if (m_objectInspectorOutside)
  {
    connect(m_view->scene(), SIGNAL(deleteFinalized(std::vector<std::string>)), this, SLOT(onDeleteFinalized(std::vector<std::string>)));
    connect(m_view->scene(), SIGNAL(undoStackHasChanged()), this, SLOT(onUndoStackHasChanged()));
    connect(m_objectInspectorOutside, SIGNAL(selectionChanged(QList<QGraphicsItem*>)), this, SLOT(onSelectionChanged(QList<QGraphicsItem*>)));
  }
}

void te::layout::PropertiesCentralController::setMenuBuilder(MenuBuilder* menu)
{
  m_menuBuilder = menu;
  if (!m_menuBuilder)
  {
    return;
  }

  connect(m_menuBuilder, SIGNAL(changeDlgProperty(Property)), this, SLOT(onPropertiesChanged()));
  if (m_menuBuilder->parent() != this)
  {
    m_menuBuilder->setParent(this);
  }
}

void te::layout::PropertiesCentralController::onSelectionChanged()
{
  QList<QGraphicsItem*> graphicsItems = m_view->scene()->selectedItems();
  QList<QGraphicsItem*> allItems = m_view->scene()->items();

  Scene* myScene = dynamic_cast<Scene*>(m_view->scene());
  if (!myScene)
    return;
  
  QGraphicsItem* subSelectedItem = myScene->getSubSelectedItem();
  if (subSelectedItem != 0)
  {
    graphicsItems.clear();
    graphicsItems.append(subSelectedItem);
  }
  
  if (m_menuBuilder)
  {
    m_menuBuilder->closeAllWindows();
  }

  /* Scene isn't in edition mode and
  mouse release yet happened, will happen the reload of the properties.
  Otherwise, the reload will happen in double click event or in mouse release */
  if (!myScene->isEditionMode())
  {
    //Refresh Property window   
    if (m_propertiesOutside)
      m_propertiesOutside->itemsSelected(graphicsItems, allItems);

    if (m_objectInspectorOutside)
      m_objectInspectorOutside->selectItems(graphicsItems);
  }
}

void te::layout::PropertiesCentralController::onPropertiesChanged()
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
  if (m_propertiesOutside)
    m_propertiesOutside->propertiesChanged(graphicsItems, allItems);

  if (m_objectInspectorOutside)
    m_objectInspectorOutside->selectItems(graphicsItems);
}

void te::layout::PropertiesCentralController::onAddItemFinalized(QGraphicsItem* item)
{
  QList<QGraphicsItem*> allItems = m_view->scene()->items();
  //Refresh Inspector Object window
  if (m_objectInspectorOutside)
    m_objectInspectorOutside->itemsInspector(allItems);
}

void te::layout::PropertiesCentralController::onSelectionChanged(QList<QGraphicsItem*> selectedItems)
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
  if (m_propertiesOutside)
  {
    m_propertiesOutside->itemsSelected(selectedItems, allItems);
  }

  if (m_menuBuilder)
  {
    m_menuBuilder->closeAllWindows();
  }
}

void te::layout::PropertiesCentralController::onAddChildFinalized(QGraphicsItem* parent, QGraphicsItem* child)
{
  QList<QGraphicsItem*> allItems = m_view->scene()->items();
  //Refresh Inspector Object window
  if (m_objectInspectorOutside)
    m_objectInspectorOutside->itemsInspector(allItems);
}

void te::layout::PropertiesCentralController::onDeleteFinalized(std::vector<std::string>)
{
  QList<QGraphicsItem*> allItems = m_view->scene()->items();
  //Refresh Inspector Object window
  if (m_objectInspectorOutside)
    m_objectInspectorOutside->itemsInspector(allItems);
}

void te::layout::PropertiesCentralController::onUndoStackHasChanged()
{
  QList<QGraphicsItem*> allItems = m_view->scene()->items();
  //Refresh Inspector Object window
  if (m_objectInspectorOutside)
    m_objectInspectorOutside->itemsInspector(allItems);
}

void te::layout::PropertiesCentralController::onShowContextMenu(QPointF scenePoint, QPointF globalPoint)
{
  if (!m_menuBuilder)
  {
    return;
  }

  te::layout::ItemUtils iUtils = ((Scene*)m_view->scene())->getItemUtils();

  QGraphicsItem* hasItem = iUtils.intersectionSelectionItem(scenePoint.x(), scenePoint.y());
  QList<QGraphicsItem*> graphicsItems = getSelectedGraphicsItems();
  if (!hasItem)
  {
    hasItem = iUtils.intersectionOnlyPaperItem(scenePoint.x(), scenePoint.y());
    graphicsItems.clear();
    graphicsItems.append(hasItem);
    if (!hasItem)
    {
      return;
    }
  }

  m_menuBuilder->createMenu(graphicsItems);
  m_menuBuilder->menuExec(globalPoint.x(), globalPoint.y());
}

QList<QGraphicsItem*> te::layout::PropertiesCentralController::getSelectedGraphicsItems()
{
  QList<QGraphicsItem*> graphicsItems = m_view->scene()->selectedItems();

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
  return graphicsItems;
}

void te::layout::PropertiesCentralController::closeAllWindows()
{
  if (m_menuBuilder)
  {
    m_menuBuilder->closeAllWindows();
  }
}

void te::layout::PropertiesCentralController::onShowDialogWindow(EnumType* type, QList<QGraphicsItem*> itemList)
{
  EnumDataType* enumData = Enums::getInstance().getEnumDataType();
  if (type == enumData->getDataTypeImage())
  {
    if (m_menuBuilder)
    {
      m_menuBuilder->setSelectedGraphicsItems(itemList);
      m_menuBuilder->setCurrentProperty(std::string("file_name"));
      m_menuBuilder->onShowImageDlg();
    }
  }
}
