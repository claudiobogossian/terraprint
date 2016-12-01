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

#include "PropertyEditorExample.h"
#include "ui_PropertyEditorExampleDialog.h"

// Layout Module
#include <terralib/layout/qt/core/propertyeditor/tree/PropertyTree.h>
#include <terralib/layout/qt/core/BuildGraphicsItem.h>
#include <terralib/layout/qt/item/RectangleItem.h>
#include <terralib/layout/qt/item/MapItem.h>
#include <terralib/layout/qt/item/MapCompositionItem.h>
#include <terralib/layout/core/pattern/mvc/AbstractItemView.h>
#include <terralib/layout/core/pattern/mvc/AbstractItemController.h>
#include <terralib/layout/core/enum/Enums.h>
#include <terralib/layout/core/property/Property.h>
#include <terralib/qt/widgets/Utils.h>
#include "ProxyLayers.h"

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
#include <QVBoxLayout>
#include <QComboBox>
#include <QStringList>
#include <QFileDialog>
#include <QFileInfo>

te::layout::example::propertyeditor::PropertyEditorExample::PropertyEditorExample(QWidget* parent) :
  QWidget(parent),
  m_ui(new Ui::PropertyEditorExampleDialog),
  m_rectItem(0),
  m_mapItem(0),
  m_mapCompositionItem(0),
  m_tree(0),
  m_combobox(0)
{
  m_ui->setupUi(this);

  m_proxy = new ProxyLayers;

  setWindowTitle(tr("Property Editor"));
  resize(480, 320);
}

te::layout::example::propertyeditor::PropertyEditorExample::~PropertyEditorExample()
{
  if (m_proxy)
  {
    delete m_proxy;
  }
}

void te::layout::example::propertyeditor::PropertyEditorExample::run()
{
  createGraphicsViewInfrastructure();
  createRectangleItem();
  createMapItem();
  createMapCompositionItem();
  createPropertyTree();
}

void te::layout::example::propertyeditor::PropertyEditorExample::createGraphicsViewInfrastructure()
{
  m_view.reset(new te::layout::View); // create View
  
  te::layout::Scene* myScene = new te::layout::Scene(m_view.get()); // create Scene
  myScene->setProxyProject(m_proxy);
  m_view->setScene(myScene);
  m_view->config(); // init layout infrastructure
}

void te::layout::example::propertyeditor::PropertyEditorExample::createRectangleItem()
{
  EnumType* itemType = Enums::getInstance().getEnumObjectType()->getRectangleItem();
  te::gm::Coord2D coord(5, 5);

  QGraphicsItem* item = createItem(itemType, coord);
  m_rectItem = dynamic_cast<RectangleItem*>(item);
}

void te::layout::example::propertyeditor::PropertyEditorExample::createMapItem()
{
  EnumType* itemType = Enums::getInstance().getEnumObjectType()->getMapItem();
  te::gm::Coord2D coord(30, 30);

  QGraphicsItem* item = createItem(itemType, coord);
  m_mapItem = dynamic_cast<MapItem*>(item);
}

void te::layout::example::propertyeditor::PropertyEditorExample::createMapCompositionItem()
{
  EnumType* itemType = Enums::getInstance().getEnumObjectType()->getMapCompositionItem();
  te::gm::Coord2D coord(60, 60);

  QGraphicsItem* item = createItem(itemType, coord);
  m_mapCompositionItem = dynamic_cast<MapCompositionItem*>(item);
}

QGraphicsItem* te::layout::example::propertyeditor::PropertyEditorExample::createItem(EnumType* itemType, te::gm::Coord2D& coord, double width, double height)
{
  QGraphicsItem* item = 0;

  if (!itemType)
  {
    return item;
  }

  BuildGraphicsItem buildItem(m_view->getScene());
  item = buildItem.createItem(itemType, coord, width, height);  
  return item;
}

void te::layout::example::propertyeditor::PropertyEditorExample::createPropertyTree()
{
  if (!m_rectItem)
  {
    return;
  }

  m_tree = new te::layout::PropertyTree(m_view.get(), 0, this); // create property tree

  connect(m_tree, SIGNAL(propertiesChanged(const te::layout::Property&)), this, SLOT(onPropertiesChanged(const te::layout::Property&)));

  createLayout();
}

void te::layout::example::propertyeditor::PropertyEditorExample::createLayout()
{
  //Layout

  if (!m_tree)
    return;

  QVBoxLayout* layout = m_ui->propEditorLayout;
  layout->setMargin(0);
  
  m_combobox = new QComboBox(this);
  m_combobox->setBaseSize(QSize(16, 16));
  m_combobox->setVisible(true);

  loadComboboxNames();

  layout->addWidget(m_combobox);
  layout->addWidget(m_tree);
  
  connect(m_combobox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onCurrentIndexChanged(const QString &)));
  
  m_combobox->setCurrentIndex(-1);
  m_combobox->setCurrentIndex(0);
}

void te::layout::example::propertyeditor::PropertyEditorExample::loadComboboxNames()
{
  QStringList list;

  // add rect item name
  const te::layout::Property rectProp = m_rectItem->getController()->getProperty("name");
  std::string nameRect = te::layout::Property::GetValueAs<std::string>(rectProp);
  list.append(nameRect.c_str());

  // add map item name
  const te::layout::Property mapProp = m_mapItem->getController()->getProperty("name");
  std::string nameMap = te::layout::Property::GetValueAs<std::string>(mapProp);
  list.append(nameMap.c_str());

  // add map composition item name
  const te::layout::Property mapCompProp = m_mapCompositionItem->getController()->getProperty("name");
  std::string nameMapComp = te::layout::Property::GetValueAs<std::string>(mapCompProp);
  list.append(nameMapComp.c_str());

  m_combobox->addItems(list);
}

void te::layout::example::propertyeditor::PropertyEditorExample::onCurrentIndexChanged(const QString & text)
{
  if (m_rectItem && m_mapItem && m_mapCompositionItem)
  {
    const Property& prop_nameRect = m_rectItem->getController()->getProperty("name");
    std::string nameRect = te::layout::Property::GetValueAs<std::string>(prop_nameRect);
    QString rectText = ItemUtils::convert2QString(nameRect);
    
    if (text.compare(rectText) == 0)
    {
      m_view->getScene()->selectItem(m_rectItem);
      loadProperties(m_rectItem);
      return;
    }

    const Property& prop_nameMap = m_mapItem->getController()->getProperty("name");
    std::string nameMap = te::layout::Property::GetValueAs<std::string>(prop_nameMap);
    QString mapText = ItemUtils::convert2QString(nameMap);
    
    if (text.compare(mapText) == 0)
    {
      m_view->getScene()->selectItem(m_mapItem);
      loadProperties(m_mapItem);
      return;
    }

    const Property& prop_nameMapComp = m_mapCompositionItem->getController()->getProperty("name");
    std::string nameMapComp = te::layout::Property::GetValueAs<std::string>(prop_nameMapComp);
    QString mapCompText = ItemUtils::convert2QString(nameMapComp);

    if (text.compare(mapCompText) == 0)
    {
      m_view->getScene()->selectItem(m_mapCompositionItem);
      loadProperties(m_mapCompositionItem);
    }
  }  
}

void te::layout::example::propertyeditor::PropertyEditorExample::loadProperties(QGraphicsItem* item)
{
  std::vector<Property> props;

  AbstractItemView* view = dynamic_cast<AbstractItemView*>(item);
  
  Properties itemProperties = view->getController()->getProperties();
  props = itemProperties.getProperties();

  m_tree->load(props); // load properties from rectangle item
}

void te::layout::example::propertyeditor::PropertyEditorExample::on_tbtnLoadLayers_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(0, "Open Shape File", te::qt::widgets::GetFilePathFromSettings("shp"), "Shape Files (*.shp)");
  
  if (m_proxy)
  {
    if (m_proxy->loadShapesToLayers(fileName))
    {
      m_ui->layerList->clear();

      QFileInfo fi(fileName);
      QString name = fi.fileName();

      QStringList list;
      list.append(name);

      m_ui->layerList->addItems(list);
    }
  }
}

void te::layout::example::propertyeditor::PropertyEditorExample::onPropertiesChanged(const te::layout::Property& prop)
{
  if (!m_view.get())
    return;

  Scene* scene = m_view->getScene();

  QList<QGraphicsItem*> items = scene->selectedItems();

  foreach(QGraphicsItem* item, items)
  {
    if (item)
    {
      AbstractItemView* lItem = dynamic_cast<AbstractItemView*>(item);
      if (lItem)
      {
        if (!lItem->getController())
        {
          continue;
        }        
        lItem->getController()->setProperty(prop);
      }
    }
  }
}

