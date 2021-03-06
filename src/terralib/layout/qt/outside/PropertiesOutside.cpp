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
  \file PropertiesOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PropertiesOutside.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "terralib/geometry/Envelope.h"
#include "../core/propertybrowser/PropertyBrowser.h"
#include "../item/MapItem.h"
#include "../../core/property/SharedProperties.h"
#include "../../item/MapModel.h"
#include "../core/ItemUtils.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/enum/Enums.h"
#include "../core/Scene.h"
#include "../core/propertybrowser/PropertiesUtils.h"
#include "../../core/pattern/proxy/AbstractProxyProject.h"
#include "../core/propertybrowser/DialogPropertiesBrowser.h"

// Qt
#include <QGroupBox>
#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>

#include <QtPropertyBrowser/QtTreePropertyBrowser>

te::layout::PropertiesOutside::PropertiesOutside(Scene* scene, AbstractProxyProject* proxyProject, AbstractOutsideController* controller, QWidget* parent, PropertyBrowser* propertyBrowser) :
  QWidget(parent),
  AbstractOutsideView(controller),
  m_updatingValues(false),
  m_sharedProps(0),
  m_propUtils(0),
  m_scene(scene)
{
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
  te::gm::Envelope box = abstractModel->getBox();
  setBaseSize(box.getWidth(), box.getHeight());
  setVisible(false);
  setWindowTitle(tr("Properties"));
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  m_propUtils = new PropertiesUtils;

  if(!propertyBrowser)
    m_layoutPropertyBrowser = new PropertyBrowser(scene, proxyProject, parent);
  else
    m_layoutPropertyBrowser = propertyBrowser;
  
  connect(m_layoutPropertyBrowser,SIGNAL(changePropertyValue(const Property&)),
    this,SLOT(onChangePropertyValue(const Property&))); 

  createLayout();

  m_sharedProps = new SharedProperties;
}

te::layout::PropertiesOutside::~PropertiesOutside()
{
  if(m_propUtils)
  {
    delete m_propUtils;
    m_propUtils = 0;
  }

  if(m_layoutPropertyBrowser)
  {
    delete m_layoutPropertyBrowser;
    m_layoutPropertyBrowser = 0;
  }

  if(m_sharedProps)
  {
    delete m_sharedProps;
    m_sharedProps = 0;
  }
}

void te::layout::PropertiesOutside::createLayout()
{
  //Layout

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setMargin(0);

  QHBoxLayout* filterLayout = new QHBoxLayout;

  m_configurePropertyEditor = new QToolButton(this);
  m_configurePropertyEditor->setText(tr("Config"));
  m_configurePropertyEditor->setBaseSize(QSize(16,16));
  m_configurePropertyEditor->setVisible(false);

  m_propertyFilterEdit = new QLineEdit(this);
  m_propertyFilterEdit->setToolTip(tr("String or regular expression to filter property list with"));

  connect(m_propertyFilterEdit,SIGNAL(textChanged(QString)),
    m_layoutPropertyBrowser,SLOT(onChangeFilter(QString))); 

  filterLayout->addWidget(m_propertyFilterEdit);
  filterLayout->addWidget(m_configurePropertyEditor);

  layout->addLayout(filterLayout);
  m_nameLabel = new QLabel(tr("Component::"), this);
  layout->addWidget(m_nameLabel);
  layout->addWidget((QWidget *) m_layoutPropertyBrowser->getPropertyEditor());

  QGroupBox* groupBox = new QGroupBox(this);
  groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  groupBox->setLayout(layout);

  QVBoxLayout* layoutAll = new QVBoxLayout(this);
  layoutAll->setMargin(0);

  layoutAll->addWidget(groupBox);

  setLayout(layoutAll);
}

void te::layout::PropertiesOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::PropertiesOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::PropertiesOutside::itemsSelected(QList<QGraphicsItem*> graphicsItems, QList<QGraphicsItem*> allItems)
{
  if(graphicsItems.empty())
  {
    clearAll();
    return;
  }  

  m_updatingValues = false;
  bool window = false;

  Properties props = m_propUtils->intersection(graphicsItems, window);
  m_layoutPropertyBrowser->setHasWindows(window);

  if(props.getProperties().empty())
    return;

  clearAll();

  m_graphicsItems = graphicsItems;

  m_allItems = allItems;

  std::string name = props.getObjectName();
  QString qName = ItemUtils::convert2QString(name);

  m_nameLabel->setText(tr("Component::") + qName);
  
  const std::vector<Property>& vecProperties = props.getProperties();

  Properties newProperties(props);
  newProperties.clear();

  foreach(Property prop, vecProperties)
  {    
    m_propUtils->checkDynamicProperty(prop, allItems);
    newProperties.addProperty(prop);
  }

  m_layoutPropertyBrowser->addProperties(newProperties);

  update();
}

void te::layout::PropertiesOutside::propertiesChanged(QList<QGraphicsItem*> graphicsItems, QList<QGraphicsItem*> allItems)
{
  if (graphicsItems.empty())
  {
    clearAll();
    return;
  }

  m_updatingValues = false;
  bool window = false;

  Properties props = m_propUtils->intersection(graphicsItems, window);
  m_layoutPropertyBrowser->setHasWindows(window);

  if (props.getProperties().empty())
    return;

  updateTree(graphicsItems, props);
}

void te::layout::PropertiesOutside::onChangePropertyValue( const Property& property )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(property.getType() == dataType->getDataTypeNone())
    return;

  QList<QGraphicsItem*> currentSelectionList = m_graphicsItems;
  
  sendPropertyToItems(property, currentSelectionList);

  m_scene->update();
}

bool te::layout::PropertiesOutside::sendPropertyToItems(const Property& property, const QList<QGraphicsItem*>& items)
{
  bool result = true; 

  Properties properties;
  properties.addProperty(property);

  m_scene->addChangePropertiesCommandToStack(items, properties);
  
  return result;
}

void te::layout::PropertiesOutside::closeEvent( QCloseEvent * event )
{
  //Closing the PropertiesOutside, all open windows from a property will be closed.
  m_layoutPropertyBrowser->closeAllWindows();
}

void te::layout::PropertiesOutside::changeZValueOrder(QList<QGraphicsItem*> listItemsToConnect)
{
  if (listItemsToConnect.empty())
  {
    return;
  }

  QGraphicsItem* first = listItemsToConnect.first();
  int minimumZValue = first->zValue();
  foreach(QGraphicsItem* item, listItemsToConnect)
  {
    if (item)
    {
      if (minimumZValue > item->zValue())
      {
        first->setZValue(item->zValue());
        item->setZValue(minimumZValue);
        break;
      }
    }
  }
}

void te::layout::PropertiesOutside::refreshOutside()
{
  bool window = false;

  Properties props = m_propUtils->intersection(m_graphicsItems, window);
  m_layoutPropertyBrowser->setHasWindows(window);

  if(props.getProperties().empty())
    return;

  const std::vector<Property>& vecProperties = props.getProperties();

  Properties newProperties(props);
  newProperties.clear();

  foreach(Property prop, vecProperties)
  {
    if (!prop.isVisible())
      continue;

    m_propUtils->checkDynamicProperty(prop, m_allItems);
    newProperties.addProperty(prop);
  }

  updatePropertyBrowser(newProperties);
}

void te::layout::PropertiesOutside::onClear( std::vector<std::string> names )
{
  clearAll();
}

void te::layout::PropertiesOutside::updatePropertyBrowser( Properties props )
{
  m_layoutPropertyBrowser->updateProperties(props);
}

void te::layout::PropertiesOutside::clearAll()
{
  m_updatingValues = false;
  m_nameLabel->setText(tr("Component::"));
  m_layoutPropertyBrowser->clearAll();
  m_graphicsItems.clear();
  m_allItems.clear();
}

bool te::layout::PropertiesOutside::updateTree( QList<QGraphicsItem*> graphicsItems, Properties props )
{
  bool result = false;
  
  if(m_graphicsItems == graphicsItems)
  {
    if(m_layoutPropertyBrowser->equalsProperties(props))
    {
      updatePropertyBrowser(props);
      return true;
    }
  }
  return false;
}

void te::layout::PropertiesOutside::onMenuPropertyClicked(const Property& prop)
{
  m_layoutPropertyBrowser->getDialogPropertiesBrowser()->directlyShowWindow(prop);
}

