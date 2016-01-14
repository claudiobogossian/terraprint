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

#include "BalloonController.h"

#include "../core/pattern/command/ChangePropertyCommand.h"
#include "../../core/enum/EnumAlignmentType.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"

te::layout::BalloonController::BalloonController(AbstractItemModel* model)
: TextController(model)
{

}

bool te::layout::BalloonController::sync()
{

  const Property& pWidth = getProperty("balloonwidth");
  const Property& pHeight = getProperty("balloonheight");

  //gets the current size of the item
  QSizeF currentSize(pWidth.getValue().toDouble(), pHeight.getValue().toDouble());

  //updates the view with the current model configuration
  QSizeF newSize = updateView();

  //if the size of the view needed to be changed, we update the model
  if (currentSize != newSize)
  {
    updateModel(newSize);
    return true;
  }

  return false;
}

QSizeF te::layout::BalloonController::updateView(){

  QSizeF boundingBalloonRect = TextController::updateView();

  boundingBalloonRect.setHeight(boundingBalloonRect.height() + boundingBalloonRect.height()*0.2);
  boundingBalloonRect.setWidth(boundingBalloonRect.width() + boundingBalloonRect.width()*0.1);
  
  return boundingBalloonRect;

}

void te::layout::BalloonController::updateModel(const QSizeF& size)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property propertyWidth(0);
  propertyWidth.setName("balloonwidth");
  propertyWidth.setValue(size.width(), dataType->getDataTypeDouble());

  Property propertyHeight(0);
  propertyHeight.setName("balloonheight");
  propertyHeight.setValue(size.height(), dataType->getDataTypeDouble());

  Properties properties("");
  properties.addProperty(propertyWidth);
  properties.addProperty(propertyHeight);

  m_model->setProperties(properties);
}

