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
#include "../../core/enum/Enums.h"
#include "../../core/property/Properties.h"

te::layout::BalloonController::BalloonController(AbstractItemModel* model, AbstractItemView* view)
: TextController(model, view)
{

}

te::layout::BalloonController::~BalloonController()
{

}

void te::layout::BalloonController::calculateSize(const te::layout::Properties& properties, QSizeF& sizeMM, double& dx, double& dy)
{

  TextController::calculateSize(properties, sizeMM, dx, dy);
  Property pMarginSize;

  if (properties.contains("margin_size") == false)
  {
    pMarginSize = this->getProperty("margin_size");
  }
  else
  {
    pMarginSize = properties.getProperty("margin_size");
  }

  double marginSize = te::layout::Property::GetValueAs<double>(pMarginSize);
  
  qreal newHeight = sizeMM.height() + (marginSize * 4);

  sizeMM.setHeight(newHeight);

  qreal newWidth = sizeMM.width() + (marginSize * 2);
  sizeMM.setWidth(newWidth);

  dx = marginSize;
  dy = marginSize * 3;

}

bool  te::layout::BalloonController::needUpdateBox(const te::layout::Properties& properties)
{
  if (properties.contains("text") == false && properties.contains("font") == false && properties.contains("margin_size") == false)
  {
    return false;
  }
  return true;
}

void te::layout::BalloonController::setProperties(const Properties& properties)
{
  //we first copy the properties that are being set
  Properties propertiesCopy(properties);

  bool hasName = properties.contains("name");
  bool hasText = properties.contains("text");
  if (hasName && !hasText)
  {
    const Property& propName = properties.getProperty("name");
    const Property& propText = getProperty("text");
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    std::string name = te::layout::Property::GetValueAs<std::string>(propName); 
    std::string text = te::layout::Property::GetValueAs<std::string>(propText);

    if (name.compare("") != 0 && text.compare("") == 0)
    {
      Property propText(0);
      propText.setName("text");
      propText.setValue(name, dataType->getDataTypeString());
      propertiesCopy.addProperty(propText);
    }
  }

  TextController::setProperties(propertiesCopy);
}

