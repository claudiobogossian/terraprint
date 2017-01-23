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

// TerraLib
#include "ItemGroupController.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "ItemGroup.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"


te::layout::ItemGroupController::ItemGroupController(AbstractItemModel* model, AbstractItemView* view)
  : AbstractItemController(model, view)
  , m_propagateResize(true)
  , m_ignoreChildrenUpdates(false)
{
}

te::layout::ItemGroupController::~ItemGroupController()
{
}


void te::layout::ItemGroupController::addItem(QGraphicsItem* newChild)
{
  QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(m_view);
  if(item == 0)
  {
    return;
  }

  AbstractItemView* childView = dynamic_cast<AbstractItemView*>(newChild);

  QPointF currentChildPos = newChild->pos();
  QPointF newChildPos = item->mapFromScene(currentChildPos);

  newChild->setParentItem(item);

  m_propagateResize = false;
  newChild->setPos(newChildPos);
 
  ItemUtils::normalizeItem(item);

  childView->getController()->attach(m_view->getController());

  m_propagateResize = true;
}

void te::layout::ItemGroupController::setProperties(const Properties& properties)
{
  if (m_propagateResize == false)
  {
    return AbstractItemController::setProperties(properties);
  }

  Properties propertiesCopy(properties);

  scaleChildrenItems(propertiesCopy);

  return AbstractItemController::setProperties(propertiesCopy);
}

void te::layout::ItemGroupController::update(const Subject* subject)
{
  return;

  if (m_ignoreChildrenUpdates == true)
  {
    return;
  }

  if (subject != m_model)
  {
    //we must check if the size of the children has been changed in order to update the size of the mapComposition
    QGraphicsItem* qItem = dynamic_cast<QGraphicsItem*>(m_view);
    QRectF rectF = qItem->childrenBoundingRect();
    if (rectF.isValid() == false || rectF.isEmpty() == true)
    {
      return;
    }

    double width = Property::GetValueAs<double>(getProperty("width"));
    double height = Property::GetValueAs<double>(getProperty("height"));

    if (width == rectF.width() && height == rectF.height())
    {
      return;
    }

    //for the children, we normalize its position in order to ajust them to the reference position coord (0,0)
    m_ignoreChildrenUpdates = true;
    m_propagateResize = false;

    ItemUtils::normalizeItem(qItem);

    m_ignoreChildrenUpdates = false;
    m_propagateResize = true;
  }
}

void te::layout::ItemGroupController::scaleChildrenItems(Properties& properties)
{
  //if the size o the group is being changed, we must scale of the children items
  bool containsX = properties.contains("x");
  bool containsY = properties.contains("y");
  bool containsWidth = properties.contains("width");
  bool containsHeight = properties.contains("height");

  //if the size has not been changed, we do nothing
  if (containsWidth == false && containsHeight == false)
  {
    return;
  }

  const Property& pCurrentX = getProperty("x");
  const Property& pCurrentY = getProperty("y");
  const Property& pCurrentWidth = getProperty("width");
  const Property& pCurrentHeight = getProperty("height");

  const Property& pNewX = getProperty("x", properties);
  const Property& pNewY = getProperty("y", properties);
  const Property& pNewWidth = getProperty("width", properties);
  const Property& pNewHeight = getProperty("height", properties);

  double currentX = te::layout::Property::GetValueAs<double>(pCurrentX);
  double currentY = te::layout::Property::GetValueAs<double>(pCurrentY);
  double currentWidth = te::layout::Property::GetValueAs<double>(pCurrentWidth);
  double currentHeight = te::layout::Property::GetValueAs<double>(pCurrentHeight);

  double newX = te::layout::Property::GetValueAs<double>(pNewX);
  double newY = te::layout::Property::GetValueAs<double>(pNewY);
  double newWidth = te::layout::Property::GetValueAs<double>(pNewWidth);
  double newHeight = te::layout::Property::GetValueAs<double>(pNewHeight);

  //if the width and height had not been initialized yet, we do nothing
  if (currentWidth == 0 && currentHeight == 0)
  {
    return;
  }

  //if nothing have been changed, we exit the function
  if (currentWidth == newWidth && currentHeight == newHeight)
  {
    return;
  }

  QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(m_view);
  if (item == 0)
  {
    return;
  }

  //if the position and the size have been changed, we must RESIZE the entire item
  //1 - if we have children, we resize them first keeping its proportionality to this
  QList<QGraphicsItem*> qChildItemsList = item->childItems();
  if (qChildItemsList.isEmpty() == false)
  {
    //calculates the porcentage of the new size in comparisson with the old size
    double itemWidthFactor = newWidth / currentWidth;
    double itemHeightFactor = newHeight / currentHeight;

    QRectF newRect = resizeChildren(item, itemWidthFactor, itemHeightFactor);
    newRect.moveTo(newX, newY);

    prepareBoundingRectForUpdate(newRect, properties);
  }
}

QRectF te::layout::ItemGroupController::resizeChildren(QGraphicsItem* qItem, double widthFactor, double heightFactor)
{
  //we must first resize all the children
  QList<QGraphicsItem*> qChildrenList = qItem->childItems();
  if (qChildrenList.isEmpty() == false)
  {
    QRectF newItemRect;
    QList<QRectF> qChildrenRectList;
    for (int i = 0; i < qChildrenList.size(); ++i)
    {
      QGraphicsItem* qChild = qChildrenList.at(i);
      QRectF newChildRect = resizeChildren(qChild, widthFactor, heightFactor);
      newItemRect = newItemRect.united(newChildRect);

      qChildrenRectList.append(newChildRect);
    }

    ItemUtils::normalizeChildrenPosition(newItemRect, qChildrenRectList);

    for (int i = 0; i < qChildrenList.size(); ++i)
    {
      QGraphicsItem* qChild = qChildrenList.at(i);
      QRectF childRect = qChildrenRectList.at(i);

      AbstractItemView* childView = dynamic_cast<AbstractItemView*>(qChild);

      Properties childProperties;
      prepareBoundingRectForUpdate(childRect, childProperties);
      childView->setProperties(childProperties);
    }

    return newItemRect;
  }

  QGraphicsItem* qParentItem = qItem->parentItem();

  //if we have a parent, our CS must me revative to it.
  //if we dont have a parent, we consider the scene
  QPointF oldParentPos(0, 0);
  if (qParentItem != 0)
  {
    oldParentPos = qParentItem->mapToScene(QPoint(0, 0));
  }

  QRectF oldItemRect = qItem->boundingRect();
  QPointF oldItemPos = qItem->mapToScene(QPoint(0, 0));

  double oldDistanceFromOriginX = oldItemPos.x() - oldParentPos.x();
  double oldDistanceFromOriginY = oldItemPos.y() - oldParentPos.y();

  double newDistanceFromOriginX = oldParentPos.x() + (oldDistanceFromOriginX * widthFactor);
  double newDistanceFromOriginY = oldParentPos.y() + (oldDistanceFromOriginY * heightFactor);

  double newItemWidth = oldItemRect.width() * widthFactor;
  double newItemHeight = oldItemRect.height() * heightFactor;

  QPointF newItemPos(newDistanceFromOriginX, newDistanceFromOriginY);
  newItemPos = qItem->mapFromScene(newItemPos);
  newItemPos = qItem->mapToParent(newItemPos);

  QRectF newItemRect(newItemPos.x(), newItemPos.y(), newItemWidth, newItemHeight);
  return newItemRect;
}

