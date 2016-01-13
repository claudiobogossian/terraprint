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

// Qt
#include <QGraphicsSceneMouseEvent>

te::layout::MapCompositionItem::MapCompositionItem(AbstractItemController* controller, bool invertedMatrix)
  : ItemGroup(controller, invertedMatrix)
{
  m_stacksBehindParent = true;
}

te::layout::MapCompositionItem::~MapCompositionItem()
{

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
  return ItemGroup::itemChange(change, value);
}

void te::layout::MapCompositionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  bool resize = false;
  if (m_currentAction == te::layout::RESIZE_ACTION)
  {
    resize = true;
  }

  ItemGroup::mouseReleaseEvent(event);

  if (resize)
  {
    SharedProperties sharedProps;

    AbstractItemView* observableItem = 0;
    QRectF childrenBoundRect = childrenBoundingRect();
    if (m_rect != childrenBoundRect)
    {
      QList<QGraphicsItem*> children = childItems();
      for (QList<QGraphicsItem*>::iterator it = children.begin(); it != children.end(); ++it)
      {
        AbstractItemView* item = dynamic_cast<AbstractItemView*>(*it);
        if (item)
        {
          //(*it)->setPos(0,0);
          const Property& pConnectItemPosition = item->getController()->getProperty(sharedProps.getItemObserver());
          if (pConnectItemPosition.isNull())
          {
            observableItem = item;
          }
        }
      }
    }

    if (observableItem)
    {
      updateObservableSize(observableItem);
    }
  }
}

void te::layout::MapCompositionItem::updateObservableSize(AbstractItemView* observableItem)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  Properties props;

  double width = m_rect.width();
  double height = m_rect.height();

  if (width > 0)
  {
    Property prop_width(0);
    prop_width.setName("width");
    prop_width.setLabel("Width");
    prop_width.setValue(width, dataType->getDataTypeDouble());
    props.addProperty(prop_width);
  }

  if (height > 0)
  {
    Property prop_height(0);
    prop_height.setName("height");
    prop_height.setLabel("Height");
    prop_height.setValue(height, dataType->getDataTypeDouble());
    props.addProperty(prop_height);
  }

  //update properties
  observableItem->getController()->setProperties(props);
}

