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
  \file ObjectInspectorOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ObjectInspectorOutside.h"
#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../core/ItemUtils.h"
#include "../item/ItemGroup.h"
#include "../core/Scene.h"

//Qt
#include <QGraphicsWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QTreeWidget>
#include <QGraphicsItem>
#include <QContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QPoint>

te::layout::ObjectInspectorOutside::ObjectInspectorOutside(Scene* scene, AbstractOutsideController* controller, QWidget* parent)
  : QWidget(parent)
  , AbstractOutsideView(controller)
  , m_treeWidget(0)
  , m_isChangingSelection(false)
  , m_scene(scene)
  , m_menu(0)
{
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
  te::gm::Envelope box = abstractModel->getBox();
  setBaseSize(box.getWidth(), box.getHeight());
  setVisible(false);
  setWindowTitle(tr("Object Inspector"));
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  QGridLayout* layout = new QGridLayout(this);
  m_treeWidget = new QTreeWidget(this);
  layout->addWidget(m_treeWidget);
  layout->setContentsMargins(0, 0, 0, 0);
  
  QStringList headerLabels;
  headerLabels.append(tr("Name"));
  headerLabels.append(tr("Type"));

  m_treeWidget->setColumnCount(2);
  m_treeWidget->setHeaderLabels(headerLabels);
  m_treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(m_treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));
}

te::layout::ObjectInspectorOutside::~ObjectInspectorOutside()
{

}

void te::layout::ObjectInspectorOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::ObjectInspectorOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::ObjectInspectorOutside::itemsInspector(QList<QGraphicsItem*> graphicsItems)
{
  if (m_graphicsItems == graphicsItems)
  {
    refreshInspector();
    return;
  }

  m_treeWidget->clear();

  m_graphicsItems = graphicsItems;

  if(m_graphicsItems.empty())
    return;

  te::layout::EnumObjectType* groupType = Enums::getInstance().getEnumObjectType();
  
  foreach(QGraphicsItem* parentItem, graphicsItems) 
  {
    if (!isParentItem(parentItem))
    {
      continue;
    }

    AbstractItemView* absParentItem = dynamic_cast<AbstractItemView*>(parentItem);
    QStringList parentList = propertiesToStringList(parentItem);        
    QTreeWidgetItem* parentTreeItem = new QTreeWidgetItem(parentList);

    QString qIconName = m_iconManager.getIconNameAsQString(absParentItem->getProperties().getTypeObj()->getName());
    if (qIconName.isEmpty() == false)
    {
      parentTreeItem->setIcon(1, QIcon::fromTheme(qIconName));
    }

    ItemGroup* group = dynamic_cast<ItemGroup*>(parentItem);
    //we just look into the children if the parent is a GroupItem
    if (group)
    {
      foreach(QGraphicsItem *childItem, parentItem->childItems())
      {
        if (!isValidItem(childItem))
        {
          continue;
        }

        AbstractItemView* absChildItem = dynamic_cast<AbstractItemView*>(childItem);
        QStringList childList = propertiesToStringList(childItem);
        QTreeWidgetItem* childTreeItem = new QTreeWidgetItem(childList);
        
        QString qIconName = m_iconManager.getIconNameAsQString(absChildItem->getProperties().getTypeObj()->getName());
        if (qIconName.isEmpty() == false)
        {
          childTreeItem->setIcon(1, QIcon::fromTheme(qIconName));
        }        
        parentTreeItem->addChild(childTreeItem);
      }
    }
    m_treeWidget->addTopLevelItem(parentTreeItem);
  }

  m_treeWidget->sortItems(0, Qt::AscendingOrder);
  m_treeWidget->resizeColumnToContents(0);
  m_treeWidget->expandAll();
}

void te::layout::ObjectInspectorOutside::onRemoveProperties( std::vector<std::string> names )
{
  for(size_t i = 0; i < names.size(); ++i)
  {
    std::string name = names[i];
    QString qName = ItemUtils::convert2QString(name);

    QList<QTreeWidgetItem*> listItems = m_treeWidget->findItems(qName, Qt::MatchExactly, 0);
    for(int  j = 0; j < listItems.size(); ++j)
    {
      m_treeWidget->removeItemWidget(listItems[j], 0);
    }
  }
}

void te::layout::ObjectInspectorOutside::selectItems( QList<QGraphicsItem*> graphicsItems )
{
  m_isChangingSelection = true;

  m_treeWidget->clearSelection();
  foreach(QGraphicsItem *item, graphicsItems) 
  {
    AbstractItemView* parentItem = dynamic_cast<AbstractItemView*>(item);
    if(parentItem == 0)
    {
      continue;
    }

    const Property& pName = parentItem->getProperty("name");
    std::string name = te::layout::Property::GetValueAs<std::string>(pName);

    QString qName = ItemUtils::convert2QString(name);
    
    QList<QTreeWidgetItem*> treeItems = m_treeWidget->findItems(qName, Qt::MatchExactly | Qt::MatchRecursive, 0);
    foreach(QTreeWidgetItem* treeItem, treeItems) 
    {
      treeItem->setSelected(true);
    }
  }

  m_isChangingSelection = false;
}

void te::layout::ObjectInspectorOutside::setIconManager(const te::layout::ItemIconManager& iconManager)
{
  m_iconManager = iconManager;
}

void te::layout::ObjectInspectorOutside::itemSelectionChanged()
{
  if(m_isChangingSelection == true)
  {
    return;
  }

  QList<QTreeWidgetItem*> selectedTreeItems = m_treeWidget->selectedItems();

  QList<QGraphicsItem*> selectedGraphicsItem;

  foreach(QTreeWidgetItem* treeItem, selectedTreeItems) 
  {
    QString qTreeItemName = treeItem->text(0);

    std::string treeItemName = ItemUtils::convert2StdString(qTreeItemName);

    foreach(QGraphicsItem* graphicsItem, m_graphicsItems) 
    {
      AbstractItemView* absItemView = dynamic_cast<AbstractItemView*>(graphicsItem);
      if(absItemView == 0)
      {
        continue;
      }

      const Property& pName = absItemView->getProperty("name");
      std::string name = te::layout::Property::GetValueAs<std::string>(pName);

      if(name == treeItemName)
      {
        selectedGraphicsItem.append(graphicsItem);
        break;
      }
    }
  }

  emit selectionChanged(selectedGraphicsItem);
}

void te::layout::ObjectInspectorOutside::contextMenuEvent(QContextMenuEvent * event)
{
  if (event->reason() != QContextMenuEvent::Mouse)
  {
    QWidget::contextMenuEvent(event);
    return;
  }  
  
  std::string treeItemName = selectItemName();
  if (treeItemName.compare("") == 0)
    return;

  createMenu(treeItemName);

  if (!m_menu)
    return;

  QPoint posMenu = event->pos();
  QPoint pt = mapToGlobal(posMenu);

  m_menu->exec(pt);
}

void te::layout::ObjectInspectorOutside::onMenuTriggered(QAction*)
{
  if (!m_scene)
    return;

  removeSelectedItem();
}

QAction* te::layout::ObjectInspectorOutside::createAction(const QString& text, const QString& objName, const QString& icon, const QString& tooltip)
{
  QAction *actionMenu = new QAction(text, this);
  actionMenu->setObjectName(objName);

  actionMenu->setIcon(QIcon::fromTheme(icon));
  actionMenu->setToolTip(tooltip);

  return actionMenu;
}

void te::layout::ObjectInspectorOutside::createMenu(std::string itemName)
{
  if (!m_menu)
  {
    m_menu = new QMenu(this);
    connect(m_menu, SIGNAL(triggered(QAction*)), this, SLOT(onMenuTriggered(QAction*)));
  }
  else
  {
    m_menu->clear();
  }

  std::string text = TR_LAYOUT("Delete ") + itemName;
  QString qText = ItemUtils::convert2QString(text);
  QString qItemName = ItemUtils::convert2QString(itemName);
  
  QAction* action = createAction(qText, qItemName, "");
  m_menu->addAction(action);
}

void te::layout::ObjectInspectorOutside::keyPressEvent(QKeyEvent * event)
{
  if (event->key() != Qt::Key_Delete)
  {
    QWidget::keyPressEvent(event);
    return;
  }

  removeSelectedItem();
}

bool te::layout::ObjectInspectorOutside::removeSelectedItem()
{
  bool result = false;

  std::string treeItemName = selectItemName();

  if (treeItemName.compare("") != 0)
  {
    m_scene->removeItemByName(treeItemName);
    result = true;
  }

  return result;
}

std::string te::layout::ObjectInspectorOutside::selectItemName()
{
  std::string treeItemName = "";

  QList<QTreeWidgetItem*> selectedTreeItems = m_treeWidget->selectedItems();
  if (selectedTreeItems.empty())
    return treeItemName;

  QTreeWidgetItem* treeItem = selectedTreeItems.first();

  QString qTreeItemName = treeItem->text(0);
  treeItemName = ItemUtils::convert2StdString(qTreeItemName);

  return treeItemName;
}

bool te::layout::ObjectInspectorOutside::isValidItem(QGraphicsItem* item)
{
  bool result = false;

  AbstractItemView* absParentItem = dynamic_cast<AbstractItemView*>(item);
  if (absParentItem == 0)
  {
    return result;
  }

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  if (absParentItem->getProperties().getTypeObj() == enumObj->getPaperItem())
  {
    return result;
  }

  return true;
}

void te::layout::ObjectInspectorOutside::clearItemList()
{
  m_graphicsItems.clear();
}

void te::layout::ObjectInspectorOutside::refreshInspector()
{
  m_isChangingSelection = true;
  te::layout::EnumObjectType* groupType = Enums::getInstance().getEnumObjectType();

  bool addChild = false;

  foreach(QGraphicsItem* parentItem, m_graphicsItems)
  {
    if (!isParentItem(parentItem))
    {
      continue;
    }

    ItemGroup* group = dynamic_cast<ItemGroup*>(parentItem);
    if (group)
    {
      QTreeWidgetItem* parentTreeItem = itemTreeFromTreeWidget(group);

      //we just look into the children if the parent is a GroupItem
      if (parentTreeItem)
      {
        foreach(QGraphicsItem *childItem, parentItem->childItems())
        {
          if (!isValidItem(childItem))
          {
            continue;
          }

          QTreeWidgetItem* childTreeItem = itemTreeFromTreeWidget(childItem);
          if (childTreeItem)
          {
            /* To add a child to an item, both can not be inside QTreeWidget */
            if (childTreeItem->parent() != parentTreeItem)
            {
              int index = m_treeWidget->indexOfTopLevelItem(parentTreeItem);
              if (m_treeWidget->takeTopLevelItem(index))
              {
                index = m_treeWidget->indexOfTopLevelItem(childTreeItem);
                if (m_treeWidget->takeTopLevelItem(index))
                {
                  parentTreeItem->addChild(childTreeItem);
                  m_treeWidget->addTopLevelItem(parentTreeItem);
                  addChild = true;
                }
              }
            }
          }
        }
      }
    }
  }

  if (addChild)
  {
    m_treeWidget->sortItems(0, Qt::AscendingOrder);
    m_treeWidget->resizeColumnToContents(0);
    m_treeWidget->expandAll();
  }
  m_isChangingSelection = false;
}

QStringList te::layout::ObjectInspectorOutside::propertiesToStringList(QGraphicsItem* item)
{
  AbstractItemView* absItem = dynamic_cast<AbstractItemView*>(item);

  const Property& pParentName = absItem->getProperty("name");
  const std::string& parentName = te::layout::Property::GetValueAs<std::string>(pParentName);
  std::string parentTypeName = absItem->getProperties().getTypeObj()->getName();

  QString qParentName = ItemUtils::convert2QString(parentName);
  QString qParentTypeName = ItemUtils::convert2QString(parentTypeName);

  QStringList list;
  list.append(qParentName);
  list.append(qParentTypeName);

  return list;
}

QTreeWidgetItem* te::layout::ObjectInspectorOutside::itemTreeFromTreeWidget(QGraphicsItem* item)
{
  QTreeWidgetItem* treeItem = 0;

  QStringList parentList = propertiesToStringList(item);

  QString qParentName = parentList.first();
  QString qParentTypeName = parentList.last();
  std::string parentTypeName = ItemUtils::convert2StdString(qParentTypeName);

  QList<QTreeWidgetItem*> clist = m_treeWidget->findItems(qParentName, Qt::MatchContains | Qt::MatchRecursive, 0);
  if (clist.isEmpty())
  {
    return treeItem;
  }
  treeItem = clist.first();

  return treeItem;
}

bool te::layout::ObjectInspectorOutside::isParentItem(QGraphicsItem* item)
{
  if (item->parentItem() == 0 && isValidItem(item))
  {
    return true;
  }
  return false;
}
