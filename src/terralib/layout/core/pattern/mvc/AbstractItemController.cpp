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
#include "AbstractItemController.h"

#include "../../enum/EnumDataType.h"
#include "../../enum/Enums.h"
#include "AbstractItemModel.h"
#include "AbstractItemView.h"
#include "../factory/ItemParamsCreate.h"
#include "../factory/AbstractItemFactory.h"
#include "../../property/SharedProperties.h"
#include "../../AbstractScene.h"
#include "../../../qt/core/ItemUtils.h"
#include "../../WarningManager.h"

// Qt
#include <QGraphicsItem>

te::layout::AbstractItemController::AbstractItemController(AbstractItemModel* model)
  : Observer()
  , m_model(model)
  , m_view(0)
  , m_marginResizePrecision(2.)
  , m_warningManager(new WarningManager())
  , m_resizableDefaultState(false)
{
  if(m_model != 0)
  {
    m_model->attach(this);
  }

  const Property& pResizable = m_model->getProperty("resizable");
  m_resizableDefaultState = te::layout::Property::GetValueAs<bool>(pResizable);
}

te::layout::AbstractItemController::~AbstractItemController()
{
  if(m_model != 0)
  {
    m_model->detach(this);
    delete m_model;
  }
}

te::layout::AbstractItemView* te::layout::AbstractItemController::getView() const
{
  return m_view;
}

const te::layout::Property& te::layout::AbstractItemController::getProperty(const std::string& propertyName) const
{
  return m_model->getProperty(propertyName);
}

const te::layout::Property& te::layout::AbstractItemController::getProperty(const std::string& propertyName, const te::layout::Properties& properties) const
{
  //we first try to find the given 'propertyName' in the given 'properties' container
  if (properties.contains(propertyName))
  {
    return properties.getProperty(propertyName);
  }

  //then we try to find in in the model
  return this->getProperty(propertyName);
}


void te::layout::AbstractItemController::setProperty(const te::layout::Property& property)
{  
  Properties props(m_model->getName(), m_model->getType());
  props.addProperty(property);

  setProperties(props);
}

const te::layout::Properties& te::layout::AbstractItemController::getProperties() const
{
  return m_model->getProperties();
}

void te::layout::AbstractItemController::setProperties(const te::layout::Properties& properties)
{
  Properties propertiesCopy = properties;

  syncItemPos(propertiesCopy);

  syncZValue(propertiesCopy);

  syncItemAssociation(propertiesCopy);

  bool hasGeometryChanged = false;
  if (propertiesCopy.contains("width"))
  {
    double newWidth = te::layout::Property::GetValueAs<double>(propertiesCopy.getProperty("width"));
    double currentWidth = te::layout::Property::GetValueAs<double>(getProperty("width"));
    if (newWidth != currentWidth)
    {
      hasGeometryChanged = true;
    }
  }
  if (propertiesCopy.contains("height"))
  {
    double newHeight = te::layout::Property::GetValueAs<double>(propertiesCopy.getProperty("height"));
    double currentHeight = te::layout::Property::GetValueAs<double>(getProperty("height"));
    if (newHeight != currentHeight)
    {
      hasGeometryChanged = true;
    }
  }
  if (propertiesCopy.contains("rotation"))
  {
      double newRotation = te::layout::Property::GetValueAs<double>(propertiesCopy.getProperty("rotation"));
      double currentRotation = te::layout::Property::GetValueAs<double>(getProperty("rotation"));

      bool enableResize = m_resizableDefaultState;
      if (newRotation != 0.0)
      {
        enableResize = false;
      }

      EnumDataType* dataType = Enums::getInstance().getEnumDataType();

      //if it has rotation, we be turn-off the resizable property
      Property property(0);
      property.setName("resizable");
      property.setLabel(TR_LAYOUT("Resizable"));
      property.setValue(enableResize, dataType->getDataTypeBool());
      propertiesCopy.addProperty(property);

      Property pWidth = m_model->getProperty("width");
      pWidth.setEditable(enableResize);
      m_model->completelyUpdateProperty(pWidth);

      Property pHeight = m_model->getProperty("height");
      pHeight.setEditable(enableResize);
      m_model->completelyUpdateProperty(pHeight);
  }
  if (hasGeometryChanged)
  {
    if (m_view != 0)
    {
      m_view->prepareGeometryChange();
    }
  }
  m_model->setProperties(propertiesCopy);

  validateItem();
}

void te::layout::AbstractItemController::attach(te::layout::AbstractItemController* controller)
{
  //we try to attach to the controller
  Observer* observer = dynamic_cast<Observer*>(controller);
  if (observer == 0)
  {
    return;
  }

  this->getModel()->attach(observer);
}

void te::layout::AbstractItemController::detach(AbstractItemController* controller)
{
  Observer* observer = dynamic_cast<Observer*>(controller);
  if (observer == 0)
  {
    return;
  }

  this->getModel()->detach(observer);
}

void te::layout::AbstractItemController::update(const te::layout::Subject* subject)
{
  if (!m_view)
  {
    return;
  }

  const Property& property = m_model->getProperty("rotation");
  if(te::layout::Property::GetValueAs<double>(property) != m_view->getItemRotation())
  {
    m_view->setItemRotation(te::layout::Property::GetValueAs<double>(property));
  }

  refresh();
}

bool te::layout::AbstractItemController::contains(const te::gm::Coord2D &coord) const
{
  return m_model->contains(coord);
}

void te::layout::AbstractItemController::setView(AbstractItemView* view)
{
  m_view = view;
  refresh(); // controller could be refresh your view
}

void te::layout::AbstractItemController::rotated(const double& degree)
{
  Properties properties;
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  {
    Property property(0);
    property.setName("rotation");
    property.setValue(degree, dataType->getDataTypeDouble());
    property.setParent(this->getModel()->getProperties().getTypeObj()->getName());
    properties.addProperty(property);
  }

  setProperties(properties);
}



void te::layout::AbstractItemController::itemPositionChanged(double x, double y)
{
  Properties properties;
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  {
    Property property(0);
    property.setName("x");
    property.setValue(x, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("y");
    property.setValue(y, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }
  m_model->setProperties(properties);
}

void te::layout::AbstractItemController::itemZValueChanged(int index)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  {
    Property property(0);
    property.setName("zValue");
    property.setValue(index, dataType->getDataTypeInt());
    m_model->setProperty(property);
  }
}

te::layout::AbstractItemModel* te::layout::AbstractItemController::getModel() const
{
  return m_model;
}

void te::layout::AbstractItemController::refresh()
{
  if (m_view == 0)
  {
    return;
  }

  // do nothing
  m_view->refresh();
}

bool te::layout::AbstractItemController::syncItemPos(Properties& properties)
{
  if (properties.getProperties().empty())
  {
    return false;
  }

  if (properties.contains("x") == false && properties.contains("y") == false)
  {
    return false;
  }

  Property prop_x = m_model->getProperty("x");
  if (properties.contains("x"))
  {
    prop_x = properties.getProperty("x");
  }

  Property prop_y = m_model->getProperty("y");
  if (properties.contains("y"))
  {
    prop_y = properties.getProperty("y");
  }

  double x = te::layout::Property::GetValueAs<double>(prop_x);
  double y = te::layout::Property::GetValueAs<double>(prop_y);
  QPointF newPos(x, y);

  QGraphicsItem* gItem = dynamic_cast<QGraphicsItem*>(m_view);
  if (gItem != 0)
  {
    if (gItem->pos() != newPos)
    {
      m_view->setItemPosition(x, y);
      return true;
    }
  }

  return false;
}

bool te::layout::AbstractItemController::syncZValue(Properties& properties)
{
  if (properties.getProperties().empty())
  {
    return false;
  }

  if (properties.contains("zValue") == false)
  {
    return false;
  }

  const Property& pZValue = properties.getProperty("zValue");
  double zValue = (double)te::layout::Property::GetValueAs<int>(pZValue);
  
  QGraphicsItem* gItem = dynamic_cast<QGraphicsItem*>(m_view);
  if (gItem != 0)
  {
    if (gItem->zValue() != zValue)
    {
      gItem->setZValue(zValue);
      return true;
    }
  }

  return false;
}

bool te::layout::AbstractItemController::syncItemAssociation(Properties& properties)
{
  SharedProperties sharedPropertiesName;
  //we first check if there the property that associated two items is being set  
  // Observer pattern relationship. Associate: != 0 / Dissociate: == 0.  
  if (properties.contains(sharedPropertiesName.getItemObserver()) == false)
  {
    return false;
  }

  const Property& pNewObserver = properties.getProperty(sharedPropertiesName.getItemObserver());
  const Property& pCurrentObserver = m_model->getProperty(sharedPropertiesName.getItemObserver());

  AbstractScene* scene = m_view->getScene();
  if (scene == 0)
  {
    return false;
  }

  std::string strNewObserver = te::layout::Property::GetValueAs<std::string>(pNewObserver);
  std::string strCurrentObserver = te::layout::Property::GetValueAs<std::string>(pCurrentObserver);
  if (strNewObserver == strCurrentObserver)
  {
    return false;
  }

  //if this property is being set, we first remove the current association (if it exists)

  // Observer pattern relationship. Associate: != 0 / Dissociate: == 0.
  AbstractItemView* currentObserver = scene->getItem(strCurrentObserver);
  if (currentObserver != 0)
  {
    currentObserver->getController()->detach(this);
  }

  //and then we make the new association (if there is a valid item)
  AbstractItemView* newObserver = scene->getItem(strNewObserver);
  if (newObserver != 0)
  {
    newObserver->getController()->attach(this);
  }

  return true;
}

QRectF te::layout::AbstractItemController::resize(te::layout::LayoutAlign grabbedPoint, QPointF initialCoord, QPointF finalCoord)
{
  if (initialCoord == finalCoord)
  {
    return QRectF();
  }

  AbstractItemView* currentView = getView();
  QGraphicsItem* currentItem = dynamic_cast<QGraphicsItem*>(currentView);

  QPointF oldItemPos = currentItem->mapToScene(QPoint(0, 0));
  QRectF oldItemRec = currentItem->boundingRect();
  QRectF newRect = calculateResize(grabbedPoint, initialCoord, finalCoord);

  double oldItemWidth = oldItemRec.width();
  double oldItemHeight = oldItemRec.height();

  //calculates the porcentage of the new size in comparisson with the old size
  double itemWidthFactor = newRect.width() / oldItemWidth;
  double itemHeightFactor = newRect.height() / oldItemHeight;

  //if the item has children, we must resize them first
  QList<QGraphicsItem*> qChildrenList = currentItem->childItems();
  QList<QGraphicsItem*>::iterator itChild = qChildrenList.begin();
  while(itChild != qChildrenList.end())
  {
    QGraphicsItem* qChild = *itChild;
    AbstractItemView* childView = dynamic_cast<AbstractItemView*>(qChild);
    childView->getController()->scaleItem(itemWidthFactor, itemHeightFactor);
 
    ++itChild;
  }

  //update the bounding box of the item
  updateBoundingRect(newRect);

  //we normalize the box and position so the item bounding rect can start at 0,0
  if (qChildrenList.isEmpty() == false)
  {
    ItemUtils::normalizeChildrenPosition(currentItem);

    newRect = currentItem->boundingRect();
    updateBoundingRect(newRect);
  }

  //we normalize the parent
  if (currentItem->parentItem() != 0)
  {
    QGraphicsItem* qParentItem = currentItem->parentItem();
    AbstractItemView* parentView = dynamic_cast<AbstractItemView*>(qParentItem);

    QRectF newParentRect = qParentItem->boundingRect();

    ItemUtils::normalizeChildrenPosition(qParentItem);
  }

  newRect = currentItem->boundingRect();

  return newRect;
}

void te::layout::AbstractItemController::scaleItem(double widthFactor, double heightFactor)
{
  QGraphicsItem* qItem = dynamic_cast<QGraphicsItem*>(this->m_view);

  //if the item has children, we must resize them first
  QList<QGraphicsItem*> qChildrenList = qItem->childItems();
  if (qChildrenList.isEmpty() == false)
  {
    QList<QGraphicsItem*>::iterator itChild = qChildrenList.begin();
    while (itChild != qChildrenList.end())
    {
      QGraphicsItem* qChild = *itChild;
      AbstractItemView* childView = dynamic_cast<AbstractItemView*>(qChild);
      childView->getController()->scaleItem(widthFactor, heightFactor);

      ++itChild;
    }

    if (qChildrenList.isEmpty() == false)
    {
      ItemUtils::normalizeChildrenPosition(qItem);

      QRectF newRect = qItem->boundingRect();
      updateBoundingRect(newRect);
    }

    return;
  }

  QGraphicsItem* qParentItem = qItem->parentItem();

  //if we have a parent, our CS must me revative to it.
  //if we dont have a parent, we consider the scene
  QPointF oldItemPos(0, 0);
  if (qParentItem != 0)
  {
    oldItemPos = qParentItem->mapToScene(QPoint(0, 0));
  }

  QRectF oldChildRect = qItem->boundingRect();
  QPointF oldChildPos = qItem->mapToScene(QPoint(0, 0));

  double oldDistanceFromOriginX = oldChildPos.x() - oldItemPos.x();
  double oldDistanceFromOriginY = oldChildPos.y() - oldItemPos.y();

  double newDistanceFromOriginX = oldItemPos.x() + (oldDistanceFromOriginX * widthFactor);
  double newDistanceFromOriginY = oldItemPos.y() + (oldDistanceFromOriginY * heightFactor);

  double newChildWidth = oldChildRect.width() * widthFactor;
  double newChildHeight = oldChildRect.height() * heightFactor;

  QPointF newChildPoint(newDistanceFromOriginX, newDistanceFromOriginY);
  newChildPoint = qItem->mapFromScene(newChildPoint);

  QRectF newChildRect(newChildPoint.x(), newChildPoint.y(), newChildWidth, newChildHeight);

  this->m_view->getController()->updateBoundingRect(newChildRect);
}

QRectF te::layout::AbstractItemController::calculateResize(te::layout::LayoutAlign grabbedPoint, QPointF initialCoord, QPointF finalCoord)
{
  AbstractItemView* currentView = getView();
  QGraphicsItem* currentItem = dynamic_cast<QGraphicsItem*>(currentView);

  QRectF oldRect = currentItem->boundingRect();
  QRectF resizeRect = oldRect;

  double width = oldRect.width();
  double height = oldRect.height();
  bool keepAspect = te::layout::Property::GetValueAs<bool>(getProperty("keep_aspect"));
  double factor = width / height;
  
  double dx = finalCoord.x() - initialCoord.x();
  double dy = finalCoord.y() - initialCoord.y();

  double correctionX = dx;
  double correctionY = dy;

  if (keepAspect == true)
  {
    correctionX = (dy * factor);
    correctionY = correctionX;
  }

  switch (grabbedPoint)
  {
  case TPTopRight:
    finalCoord.setX(initialCoord.x() + correctionX);
    resizeRect.setBottomRight(finalCoord);
    break;

  case TPTopLeft:
    if (keepAspect == true)
      finalCoord.setX(initialCoord.x() - correctionX);
    else
      finalCoord.setX(initialCoord.x() + correctionX);
    resizeRect.setBottomLeft(finalCoord);
    break;

  case TPLowerRight:
    if (keepAspect == true)
      finalCoord.setX(initialCoord.x() - correctionX);
    else
      finalCoord.setX(initialCoord.x() + correctionX);
    resizeRect.setTopRight(finalCoord);
    break;

  case TPLowerLeft:
    finalCoord.setX(initialCoord.x() + correctionX);
    resizeRect.setTopLeft(finalCoord);
    break;

  case TPRight:
    resizeRect.setRight(initialCoord.x() + correctionX);
    break;

  case TPLeft:
    resizeRect.setLeft(initialCoord.x() + correctionX);
    break;

  case TPTop:
    resizeRect.setBottom(initialCoord.y() + correctionY);
    break;

  case TPLower:
    resizeRect.setTop(initialCoord.y() + correctionY);
    break;

  default:
    break;
  }

  if (isLimitExceeded(resizeRect) == false)
  {    
    return resizeRect;
  }

  return oldRect;
}

bool te::layout::AbstractItemController::isLimitExceeded(QRectF resizeRect)
{
  bool result = false;

  if ((resizeRect.width() - m_marginResizePrecision) <= 0
    || (resizeRect.height() - m_marginResizePrecision) <= 0)
  {
    result = true;
  }

  return result;
}

double te::layout::AbstractItemController::getMarginResizePrecision()
{
  return m_marginResizePrecision;
}

void te::layout::AbstractItemController::updateBoundingRect(QRectF rect)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  double x = rect.x();
  double y = rect.y();

  AbstractItemView* abstractView = getView();
  QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(abstractView);
  if (item)
  {
    QPointF newPos(x, y);
    // Convert coordinated item for coordinated scene or parent
    newPos = item->mapToParent(newPos);
    x = newPos.x();
    y = newPos.y();
  }

  double width = rect.width();
  double height = rect.height();

  Property prop_x = getProperty("x");
  prop_x.setValue(x, dataType->getDataTypeDouble());

  Property prop_y = getProperty("y");
  prop_y.setValue(y, dataType->getDataTypeDouble());

  Property prop_width = getProperty("width");
  prop_width.setValue(width, dataType->getDataTypeDouble());

  Property prop_height = getProperty("height");
  prop_height.setValue(height, dataType->getDataTypeDouble());

  Properties props;
  props.addProperty(prop_x);
  props.addProperty(prop_y);
  props.addProperty(prop_width);
  props.addProperty(prop_height);

  setProperties(props);
}


te::layout::WarningManager* te::layout::AbstractItemController::getWarningManager()
{
  return m_warningManager;
}

void te::layout::AbstractItemController::validateItem()
{

}