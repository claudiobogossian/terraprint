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
  \file ItemGroup.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemGroup.h"
#include "ItemGroupController.h"

#include "../core/Scene.h"

// Qt
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

te::layout::ItemGroup::ItemGroup(AbstractItemController* controller)
  : AbstractItem(controller)
  , m_stacksBehindParent(false)
  , m_isSubSelectionAllowed(true)
{
  this->setHandlesChildEvents(true);
}

te::layout::ItemGroup::~ItemGroup()
{

}

QRectF te::layout::ItemGroup::boundingRect() const
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

void te::layout::ItemGroup::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  //we dont want the group to draw its selection, so we dont call the paint function
}

QVariant te::layout::ItemGroup::itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value )
{
  if(change == QGraphicsItem::ItemChildAddedChange)
  {
    ItemGroupController* controller = dynamic_cast<ItemGroupController*>(getController());
    if(controller != 0)
    {
      controller->itemAdded();
    }

    QGraphicsItem* child = qvariant_cast<QGraphicsItem *>(value);
    if (child != 0)
    {
      child->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    }
  }
  else if (change == QGraphicsItem::ItemChildRemovedChange)
  {
    QGraphicsItem* child = qvariant_cast<QGraphicsItem *>(value);
    if (child != 0)
    {
      child->setFlag(QGraphicsItem::ItemStacksBehindParent, false);

      AbstractItemView* absItem = dynamic_cast < AbstractItemView*>(child);
      if (absItem != 0)
      {
        absItem->setSubSelection(false);
      }
    }
  }
  else if (change == QGraphicsItem::ItemSelectedHasChanged)
  {
    if (isSelected() == false)
    {
      //we remove the subSelection of the children
      QList<QGraphicsItem*> children = childItems();
      QList<QGraphicsItem*>::iterator it = children.begin();
      while (it != children.end())
      {
        AbstractItemView* item = dynamic_cast<AbstractItemView*>(*it);
        if (item != 0 && item->isSubSelected() == true && item->getCurrentAction() != te::layout::RESIZE_ACTION)
        {
          bool result = true;
          if (item->isEditionMode())
          {
            result = false;
          }
          item->setSubSelection(false);
          setHandlesChildEvents(result);
          (*it)->setFlag(QGraphicsItem::ItemStacksBehindParent, result);
        }
        ++it;
      }
    }
  }

  return AbstractItem::itemChange(change, value);
}

void te::layout::ItemGroup::addToGroup(QGraphicsItem* item)
{
  QPointF itemCurrentPos = item->pos();

  item->setParentItem(this);

  QPointF itemNewPos = this->mapFromScene(itemCurrentPos);
  item->setPos(itemNewPos);

  ItemUtils::normalizeChildrenPosition(this);
}

void te::layout::ItemGroup::removeFromGroup(QGraphicsItem* item)
{
  QPointF itemNewPos = item->mapToScene(QPointF(0,0));
  item->setParentItem(this->parentItem());

  if (item->parentItem() != 0)
  {
    itemNewPos = item->parentItem()->mapFromScene(itemNewPos);
  }

  item->setPos(itemNewPos);


  ItemUtils::normalizeChildrenPosition(this);
}

void te::layout::ItemGroup::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  bool wasSelected = isSelected();
  AbstractItem::mousePressEvent(event);
  bool continuedSelected = isSelected();

  if (m_currentAction == te::layout::RESIZE_ACTION)
  {
    return;
  }

  bool is_childrenResizeMode = hasChildrenInResizeMode();

  if (isEditionMode() == true)
  {
    return;
  }

  if (event->button() & Qt::LeftButton)
  {
    if (is_childrenResizeMode)
    {
      m_currentAction = te::layout::NO_ACTION;
      return;
    }

    QList<QGraphicsItem*> children = childItems();
    QList<QGraphicsItem*>::iterator it = children.begin();
    while (it != children.end())
    {
      AbstractItemView* item = dynamic_cast<AbstractItemView*>(*it);
      if (item != 0 && item->isSubSelected() == true)
      {
        item->setSubSelection(false);
        setHandlesChildEvents(true);
        (*it)->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
      }

      ++it;
    }

    if (wasSelected == true && continuedSelected == true)
    {
      //we try to select the children
      QList<QGraphicsItem*> children = childItems();
      QList<QGraphicsItem*>::iterator it = children.begin();
      while (it != children.end())
      {
        QPointF childrenPos = (*it)->mapFromParent(event->pos());
        if ((*it)->contains(childrenPos))
        {
          AbstractItemView* item = dynamic_cast<AbstractItemView*>(*it);
          if (item != 0)
          {
            if(m_isSubSelectionAllowed == true)
            {
              item->setSubSelection(true);
            }

            setHandlesChildEvents(false);
            (*it)->setFlag(QGraphicsItem::ItemStacksBehindParent, m_stacksBehindParent);
            break;
          }
        }
        ++it;
      }
    }
  }
}

bool te::layout::ItemGroup::hasChildrenInResizeMode()
{
  bool result = false;

  QList<QGraphicsItem*> children = childItems();
  for (QList<QGraphicsItem*>::iterator it = children.begin(); it != children.end(); ++it)
  {
    AbstractItemView* item = dynamic_cast<AbstractItemView*>(*it);
    if (item != 0 && item->isSubSelected() == true)
    {
      if (item->getCurrentAction() == te::layout::RESIZE_ACTION)
      {
        result = true;
        break;
      }
    }
  }
  return result;
}

