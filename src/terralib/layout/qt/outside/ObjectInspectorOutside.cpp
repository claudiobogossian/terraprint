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
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../item/MovingItemGroup.h"
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
  
  //QVBoxLayout* layout = new QVBoxLayout(this);
  //layout->setMargin(0);
  //layout->addWidget(m_treeWidget);
  //
  //QGroupBox* groupBox = new QGroupBox;
  //groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //groupBox->setLayout(layout);

  //QVBoxLayout* layoutAll = new QVBoxLayout(this);
  //layoutAll->setMargin(0);

  //layoutAll->addWidget(groupBox);

  //setLayout(layoutAll);

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
  m_treeWidget->clear();

  m_graphicsItems = graphicsItems;

  if(m_graphicsItems.empty())
    return;

  te::layout::EnumObjectType* groupType = Enums::getInstance().getEnumObjectType();
  
  foreach(QGraphicsItem* parentItem, graphicsItems) 
  {
    if(parentItem->parentItem() != 0)
    {
      continue;
    }

    if (!isValidItem(parentItem))
    {
      continue;
    }

    AbstractItemView* absParentItem = dynamic_cast<AbstractItemView*>(parentItem);

    const Property& pParentName = absParentItem->getController()->getProperty("name");
    const std::string& parentName = pParentName.getValue().toString();
    std::string parentTypeName = absParentItem->getController()->getProperties().getTypeObj()->getName();

    QString qParentName = ItemUtils::convert2QString(parentName);
    QString qParentTypeName = ItemUtils::convert2QString(parentTypeName);

    QStringList parentList;
    parentList.append(qParentName);
    parentList.append(qParentTypeName);

    QTreeWidgetItem* parentTreeItem = new QTreeWidgetItem(parentList);
    QString qIconName = m_iconManager.getIconNameAsQString(absParentItem->getController()->getProperties().getTypeObj()->getName());
    if (qIconName.isEmpty() == false)
    {
      parentTreeItem->setIcon(1, QIcon::fromTheme(qIconName));
    }

    //we just look into the children if the parent is a GroupItem
    if (parentTypeName == groupType->getItemGroup()->getName())
    {
      foreach(QGraphicsItem *childItem, parentItem->childItems())
      {
        if (!isValidItem(childItem))
        {
          continue;
        }

        AbstractItemView* absChildItem = dynamic_cast<AbstractItemView*>(childItem);

        const Property& pChildName = absChildItem->getController()->getProperty("name");
        const std::string& childName = pChildName.getValue().toString();
        std::string childTypeName = absChildItem->getController()->getProperties().getTypeObj()->getName();

        QString qChildName = ItemUtils::convert2QString(childName);
        QString qChildTypeName = ItemUtils::convert2QString(childTypeName);

        QStringList childList;
        childList.append(qChildName);
        childList.append(qChildTypeName);

        QTreeWidgetItem* childTreeItem = new QTreeWidgetItem(childList);
        
        QString qIconName = m_iconManager.getIconNameAsQString(absChildItem->getController()->getProperties().getTypeObj()->getName());
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
  m_treeWidget->expandAll();
  m_treeWidget->adjustSize();
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

    const Property& pName = parentItem->getController()->getProperty("name");
    std::string name = pName.getValue().toString();

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

      const Property& pName = absItemView->getController()->getProperty("name");
      std::string name = pName.getValue().toString();

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

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

  MovingItemGroup* moving = dynamic_cast<MovingItemGroup*>(item);
  if (moving)
  {
    return result;
  }

  AbstractItemView* absParentItem = dynamic_cast<AbstractItemView*>(item);
  if (absParentItem == 0)
  {
    return result;
  }
  if (absParentItem->getController() == 0)
  {
    return result;
  }

  if (absParentItem->getController()->getProperties().getTypeObj() == enumObj->getPaperItem())
  {
    return result;
  }

  return true;
}

