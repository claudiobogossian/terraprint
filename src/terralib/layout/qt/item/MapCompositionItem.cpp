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
#include "../../core/property/SharedProperties.h"
#include "../../core/property/GridSettingsConfigProperties.h"
#include "../core/pattern/factory/item/ItemFactory.h"
#include "MapItem.h"

// Qt
#include <QGraphicsSceneMouseEvent>

te::layout::MapCompositionItem::MapCompositionItem(AbstractItemController* controller)
  : ItemGroup(controller)
  , m_mapItem(0)
  , m_planarGridItem(0)
  , m_geodesicGridItem(0)
{
  m_stacksBehindParent = true;
  m_isSubSelectionAllowed = false;
  this->setHandlesChildEvents(false);
}

te::layout::MapCompositionItem::~MapCompositionItem()
{

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
  const Property& pName = this->getController()->getProperty("name");
  const std::string& parentName = pName.getValue().toString();

  std::string mapName = parentName + "_Map";
  {
    ItemFactoryParamsCreate params(mapName);// = createParams(itemType);
    std::string type = "MAP_ITEM";

    AbstractItemView* abstractItem = te::layout::ItemFactory::make(type, params);
    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(abstractItem);

    if (item != 0)
    {
      this->addToGroup(item);
    }

    m_mapItem = abstractItem;
  }
  {
    ItemFactoryParamsCreate params(parentName + "_PlanarGrid");// = createParams(itemType);
    std::string type = "GRID_PLANAR_ITEM";

    AbstractItemView* abstractItem = te::layout::ItemFactory::make(type, params);
    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(abstractItem);

    if (item != 0)
    {
      this->addToGroup(item);
    }

    m_planarGridItem = abstractItem;
  }
  {
    ItemFactoryParamsCreate params(parentName + "_GeodesicGrid");// = createParams(itemType);
    std::string type = "GRID_GEODESIC_ITEM";

    AbstractItemView* abstractItem = te::layout::ItemFactory::make(type, params);
    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(abstractItem);

    if (item != 0)
    {
      this->addToGroup(item);
    }

    m_geodesicGridItem = abstractItem;
  }

  SharedProperties sharedProps;
  GridSettingsConfigProperties settingsConfig;
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  te::layout::Property pAssociate;
  pAssociate.setName(sharedProps.getItemObserver());
  pAssociate.setValue(mapName, dataType->getDataTypeItemObserver());
  
  //sets the properties for the planar grid
  m_planarGridItem->getController()->setProperty(pAssociate);

  //sets the properties for the geodesic grid
  te::layout::Property pVisible;
  pVisible.setName(settingsConfig.getVisible());
  pVisible.setValue(false, dataType->getDataTypeBool());

  Properties geodesicProperties;    
  geodesicProperties.addProperty(pAssociate);
  geodesicProperties.addProperty(pVisible);


  m_geodesicGridItem->getController()->setProperties(geodesicProperties);
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
        if (!pOldMapItem.isNull())
        {
          if (pOldMapItem.getValue().toString().compare("") != 0)
          {
            // Dissociate
            Property prop(pOldMapItem);
            std::string itemName = "";
            prop.setValue(itemName, dataType->getDataTypeItemObserver());
            childAbstractView->getController()->setProperty(prop);

            // Active resizable
            Property property(0);
            property.setName("resizable");
            property.setValue(true, dataType->getDataTypeBool());
            childAbstractView->getController()->setProperty(property);
          }
        }
      }
    }
  }
  else if (change == QGraphicsItem::ItemSceneHasChanged)
  {
    initItems();
  }
  return ItemGroup::itemChange(change, value);
}

