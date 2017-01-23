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
  \file MapCompositionItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapCompositionItem.h"

#include "MapCompositionController.h"
#include "MapItem.h"
#include "../core/pattern/factory/item/ItemFactory.h"
#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/property/SharedProperties.h"
#include "../../core/property/GridSettingsConfigProperties.h"
#include "../../item/MapCompositionModel.h"

// Qt
#include <QGraphicsSceneMouseEvent>

te::layout::MapCompositionItem::MapCompositionItem(const std::string& name)
  : ItemGroup()
  , m_mapItem(0)
  , m_planarGridItem(0)
  , m_geodesicGridItem(0)
{
  m_stacksBehindParent = true;
  m_isSubSelectionAllowed = false;
  this->setHandlesChildEvents(false);

  std::string mapName = name + "_Map";
  {
    Properties props;
    Property propName(0);
    propName.setName("name");
    propName.setValue(mapName, Enums::getInstance().getEnumDataType()->getDataTypeString());
    props.addProperty(propName);
    ItemFactoryParamsCreate params(props);

    AbstractItemView* abstractItem = te::layout::ItemFactory::make(Enums::getInstance().getEnumObjectType()->getMapItem()->getName(), params);
    abstractItem->getController()->attach(this->getController());

    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(abstractItem);

    if (item != 0)
    {
      item->setFlag(QGraphicsItem::ItemIsSelectable, false);
      this->addToGroup(item);
    }

    m_mapItem = abstractItem;
  }
  std::string gridPlanarName = name + "_PlanarGrid";
  {
    Properties props;
    Property propName(0);
    propName.setName("name");
    propName.setValue(gridPlanarName, Enums::getInstance().getEnumDataType()->getDataTypeString());
    props.addProperty(propName);
    ItemFactoryParamsCreate params(props);

    AbstractItemView* abstractItem = te::layout::ItemFactory::make(Enums::getInstance().getEnumObjectType()->getGridPlanarItem()->getName(), params);
    abstractItem->getController()->attach(this->getController());

    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(abstractItem);

    if (item != 0)
    {
      item->setFlag(QGraphicsItem::ItemIsSelectable, false);
      this->addToGroup(item);
    }

    m_planarGridItem = abstractItem;
  }
  std::string gridGeoGridName = name + "_GeodesicGrid";
  {
    Properties props;
    Property propName(0);
    propName.setName("name");
    propName.setValue(gridGeoGridName, Enums::getInstance().getEnumDataType()->getDataTypeString());
    props.addProperty(propName);
    ItemFactoryParamsCreate params(props);

    AbstractItemView* abstractItem = te::layout::ItemFactory::make(Enums::getInstance().getEnumObjectType()->getGridGeodesicItem()->getName(), params);
    abstractItem->getController()->attach(this->getController());

    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(abstractItem);

    if (item != 0)
    {
      item->setFlag(QGraphicsItem::ItemIsSelectable, false);
      this->addToGroup(item);
    }

    m_geodesicGridItem = abstractItem;
  }

  //Property pName;
  //pName.setName("name");
  //pName.setValue<std::string>(name, Enums::getInstance().getEnumDataType()->getDataTypeString());

  //getController()->setProperty(pName);
}

te::layout::MapCompositionItem::~MapCompositionItem()
{
  finalizeItems();
}

QRectF te::layout::MapCompositionItem::boundingRect() const
{
  bool resizable = te::layout::Property::GetValueAs<bool>(this->getProperty("resizable"));
  if (m_currentAction == te::layout::RESIZE_ACTION && resizable)
  {
    return AbstractItem::boundingRect();
  }

  QRectF rect = this->childrenBoundingRect();
  if(rect.isValid() == true)
  {
    return rect;
  }

  return AbstractItem::boundingRect();
}

te::layout::AbstractItemModel* te::layout::MapCompositionItem::createModel() const
{
  return new MapCompositionModel();
}

te::layout::AbstractItemController* te::layout::MapCompositionItem::createController() const
{
  AbstractItemModel* model = createModel();
  return new MapCompositionController(model, (AbstractItemView*)this);
}

te::layout::AbstractItemView* te::layout::MapCompositionItem::getMapItem()
{
  return m_mapItem;
}

te::layout::AbstractItemView* te::layout::MapCompositionItem::getPlanarGridItem()
{
  return m_planarGridItem;
}

te::layout::AbstractItemView* te::layout::MapCompositionItem::getGeodesicGridItem()
{
  return m_geodesicGridItem;
}

void te::layout::MapCompositionItem::initItems()
{
  /*
  const std::string& mapName = Property::GetValueAs<std::string>(m_mapItem->getController()->getProperty("name"));

  SharedProperties sharedProps;
  GridSettingsConfigProperties settingsConfig;
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  te::layout::Property pAssociate;
  pAssociate.setName(sharedProps.getItemObserver());
  pAssociate.setValue(mapName, dataType->getDataTypeItemObserver());
  
  //sets the properties for the planar grid
  Properties planarProperties;
  planarProperties.addProperty(pAssociate);
  m_planarGridItem->getController()->setProperties(planarProperties);

  //sets the properties for the geodesic grid
  te::layout::Property pVisible;
  pVisible.setName(settingsConfig.getVisible());
  pVisible.setValue(false, dataType->getDataTypeBool());

  Properties geodesicProperties;
  geodesicProperties.addProperty(pAssociate);
  geodesicProperties.addProperty(pVisible);

  m_geodesicGridItem->getController()->setProperties(geodesicProperties);

  //this->prepareGeometryChange();
  //ItemUtils::normalizeItem(this);
  */
}

void te::layout::MapCompositionItem::finalizeItems()
{
  if (m_mapItem)
  {
    delete m_mapItem;
    m_mapItem = 0;
  }
  if (m_planarGridItem)
  {
    delete m_planarGridItem;
    m_planarGridItem = 0;
  }
  if (m_geodesicGridItem)
  {
    delete m_geodesicGridItem;
    m_geodesicGridItem = 0;
  }
}

void te::layout::MapCompositionItem::setEditionMode(bool editionMode)
{
  m_mapItem->setEditionMode(editionMode);
}

bool te::layout::MapCompositionItem::isEditionMode()
{
  return m_mapItem->isEditionMode();
}

bool te::layout::MapCompositionItem::changeCurrentTool(EnumType* tool)
{
  MapItem* mapItem = dynamic_cast<MapItem*>(m_mapItem);
  if(mapItem != 0)
  {
    return mapItem->changeCurrentTool(tool);
  }

  return false;
}

QVariant te::layout::MapCompositionItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant & value)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  QVariant variant = ItemGroup::itemChange(change, value);

  if (change == QGraphicsItem::ItemChildAddedChange)
  {
    QGraphicsItem* child = qvariant_cast<QGraphicsItem *>(value);
    if (child != 0)
    {
      AbstractItemView* childAbstractView = dynamic_cast<AbstractItemView*>(child);
      if (childAbstractView != 0)
      {
        Property property(0);
        property.setName("resizable");
        property.setValue(false, dataType->getDataTypeBool());
        childAbstractView->getController()->setProperty(property);
      }
    }
  }
  else if (change == QGraphicsItem::ItemChildRemovedChange)
  {
    QGraphicsItem* child = qvariant_cast<QGraphicsItem *>(value);
    if (child != 0)
    {
      // Observer pattern relationship. Associate: != 0 / Dissociate: == 0.
      AbstractItemView* childAbstractView = dynamic_cast<AbstractItemView*>(child);
      if (childAbstractView != 0)
      {
        SharedProperties sharedProps;
        const Property& pOldMapItem = childAbstractView->getController()->getProperty(sharedProps.getItemObserver());
        if (te::layout::Property::GetValueAs<std::string>(pOldMapItem).compare("") != 0)
        {
          // Dissociate
          Property prop(pOldMapItem);
          std::string itemName = "";
          prop.setValue(itemName, dataType->getDataTypeItemObserver());
          childAbstractView->getController()->setProperty(prop);

          // Active resizable
          Property property(0);
          property.setName("resizable");
          property.setValue<bool>(true, dataType->getDataTypeBool());
          childAbstractView->getController()->setProperty(property);
        }
      }
    }
  }
  else if (change == QGraphicsItem::ItemSceneHasChanged)
  {
    QGraphicsScene* scene = qvariant_cast<QGraphicsScene *>(value);
    if (scene == 0)
    {
      //finalizeItems();
    }
    else
    {
      initItems();
    }
    
  }
  return variant;
}


void te::layout::MapCompositionItem::drawItem(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
  AbstractItem::drawFrame(painter);
}