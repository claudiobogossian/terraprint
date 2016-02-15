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

#include "AbstractItemModel.h"
#include "AbstractItemView.h"
#include "../factory/ItemParamsCreate.h"
#include "../factory/AbstractItemFactory.h"
#include "../../property/SharedProperties.h"
#include "../../property/GenericVariant.h"
#include "../../AbstractScene.h"

te::layout::AbstractItemController::AbstractItemController(AbstractItemModel* model)
  : Observer()
  , m_model(model)
  , m_view(0)
{
  if(m_model != 0)
  {
    m_model->attach(this);
  }
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
  bool hasWidth = !propertiesCopy.getProperty("width").isNull();
  if (hasWidth)
  {
    double newWidth = propertiesCopy.getProperty("width").getValue().toDouble();
    double currentWidth = m_model->getProperty("width").getValue().toDouble();
    if (newWidth != currentWidth)
    {
      hasGeometryChanged = true;
    }
  }
  bool hasHeight = !propertiesCopy.getProperty("height").isNull();
  if (hasHeight)
  {
    double newHeight = propertiesCopy.getProperty("height").getValue().toDouble();
    double currentHeight = m_model->getProperty("height").getValue().toDouble();
    if (newHeight != currentHeight)
    {
      hasGeometryChanged = true;
    }
  }
  if (hasGeometryChanged)
  {
    m_view->prepareGeometryChange();
  }
  m_model->setProperties(propertiesCopy);
  if (hasGeometryChanged)
  {
    m_view->updateChildren(); // update children size
  }
}

void te::layout::AbstractItemController::attach(te::layout::AbstractItemController* controller)
{
  Observer* observer = dynamic_cast<Observer*>(controller->getModel());
  if(observer == 0)
  {
    return;
  }

  this->getModel()->attach(observer);
}

void te::layout::AbstractItemController::detach(AbstractItemController* controller)
{
  Observer* observer = dynamic_cast<Observer*>(controller->getModel());
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
  if(property.getValue().toDouble() != m_view->getItemRotation())
  {
    m_view->setItemRotation(property.getValue().toDouble());
  }

  m_view->refresh();
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

void te::layout::AbstractItemController::resized(const double& width, const double& height)
{
  Properties properties;
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  {
    Property property(0);
    property.setName("width");
    property.setValue(width, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("height");
    property.setValue(height, dataType->getDataTypeDouble());
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
  // do nothing
}

bool te::layout::AbstractItemController::syncItemPos(Properties& properties)
{
  if (properties.getProperties().empty())
  {
    return false;
  }

  Property prop_x = properties.getProperty("x");
  Property prop_y = properties.getProperty("y");

  if (prop_x.isNull() && prop_y.isNull())
  {
    return false;
  }

  if (prop_x.isNull())
  {
    prop_x = m_model->getProperty("x");
  }

  if (prop_y.isNull())
  {
    prop_y = m_model->getProperty("y");
  }

  double x = prop_x.getValue().toDouble();
  double y = prop_y.getValue().toDouble();
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

  Property pZValue = properties.getProperty("zValue");

  if (pZValue.isNull() == true)
  {
    return false;
  }

  double zValue = (double)pZValue.getValue().toInt();
  
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
  const Property& pNewObserver = properties.getProperty(sharedPropertiesName.getItemObserver());
  const Property& pCurrentObserver = m_model->getProperty(sharedPropertiesName.getItemObserver());
  if (pNewObserver.isNull() == true)
  {
    return false;
  }
  if (pCurrentObserver.isNull() == true)
  {
    return false;
  }

  AbstractScene* scene = m_view->getScene();
  if (scene == 0)
  {
    return false;
  }

  std::string strNewObserver = pNewObserver.getValue().toString();
  std::string strCurrentObserver = pCurrentObserver.getValue().toString();
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

