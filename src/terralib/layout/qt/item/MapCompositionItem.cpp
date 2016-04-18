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
#include "MapItem.h"
#include "../../core/property/SharedProperties.h"
#include "../core/pattern/factory/item/ItemFactory.h"



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

  {
    ItemFactoryParamsCreate params("MapCompositionItem_Map");// = createParams(itemType);
    std::string name = "MAP_ITEM";

    AbstractItemView* abstractItem = te::layout::ItemFactory::make(name, params);
    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(abstractItem);

    if (item != 0)
    {
      this->addToGroup(item);
    }

    m_mapItem = abstractItem;
  }
  {
    ItemFactoryParamsCreate params("MapCompositionItem_PlanarGrid");// = createParams(itemType);
    std::string name = "GRID_PLANAR_ITEM";

    AbstractItemView* abstractItem = te::layout::ItemFactory::make(name, params);
    QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(abstractItem);

    if (item != 0)
    {
      this->addToGroup(item);
    }

    m_planarGridItem = abstractItem;
  }
    {
      ItemFactoryParamsCreate params("MapCompositionItem_GeodesicGrid");// = createParams(itemType);
      std::string name = "GRID_GEODESIC_ITEM";

      AbstractItemView* abstractItem = te::layout::ItemFactory::make(name, params);
      QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(abstractItem);

      if (item != 0)
      {
        this->addToGroup(item);
      }

      m_geodesicGridItem = abstractItem;
    }
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

void te::layout::MapCompositionItem::setEditionMode(bool editionMode)
{
  m_mapItem->setEditionMode(editionMode);
}

bool te::layout::MapCompositionItem::isEditionMode()
{
  return m_mapItem->isEditionMode();
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
    SharedProperties sharedProps;
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    std::string mapName = "MapCompositionItem_Map";

    te::layout::Property pAssociate;
    pAssociate.setName(sharedProps.getItemObserver());
    pAssociate.setValue(mapName, dataType->getDataTypeItemObserver());
    m_planarGridItem->getController()->setProperty(pAssociate);
    m_geodesicGridItem->getController()->setProperty(pAssociate);
  }
  return ItemGroup::itemChange(change, value);
}

void te::layout::MapCompositionItem::updateChildSize(AbstractItemView* item)
{
  if (!m_spaceBetweenParentChild.contains(item) || !m_mapItem)
  {
    return;
  }

  MapItem* map = dynamic_cast<MapItem*>(item);
  if (!map)
    return;

  if (map != m_mapItem)
    return;

  QSize childSpace = m_spaceBetweenParentChild[item];

  double currentWidth = m_controller->getProperty("width").getValue().toDouble();
  double currentHeight = m_controller->getProperty("height").getValue().toDouble();

  double width = currentWidth - childSpace.width();
  double height = currentHeight - childSpace.height();

  if (width < m_marginResizePrecision || height < m_marginResizePrecision)
  {
    return;
  }

  SharedProperties sharedProps;
  // search for observable item
  const Property& pConnectItemPosition = item->getController()->getProperty(sharedProps.getItemObserver());
  if (pConnectItemPosition.isNull())
  {
    //update properties
    item->getController()->resized(width, height);
    item->prepareGeometryChange(); //update childrenBoundingRect
  }
}

void te::layout::MapCompositionItem::beginResize()
{
  m_spaceBetweenParentChild.clear();
  QList<QGraphicsItem*> children = childItems();
  for (QList<QGraphicsItem*>::iterator it = children.begin(); it != children.end(); ++it)
  {
    AbstractItemView* item = dynamic_cast<AbstractItemView*>(*it);
    if (item)
    {
      QRectF boundRect = (*it)->boundingRect();
      double width = childrenBoundingRect().width() - boundRect.width();
      double height = childrenBoundingRect().height() - boundRect.height();
      m_spaceBetweenParentChild[item] = QSize(width, height);
    }
  }
}

void te::layout::MapCompositionItem::resized()
{
  
}

