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
#include "PaperController.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/enum/Enums.h"
#include "../../core/property/Properties.h"

// STL
#include <vector>

te::layout::PaperController::PaperController(te::layout::AbstractItemModel* model, AbstractItemView* view)
  : AbstractItemController(model, view)
{

}

te::layout::PaperController::~PaperController()
{

}

void te::layout::PaperController::setProperties(const Properties& properties)
{
  //we first copy the properties that are being set
  te::layout::Properties propertiesCopy (properties);

  const Property& pCurrentPaperWidth = getProperty("paper_width");
  const Property& pCurrentPaperHeight = getProperty("paper_height");

  double currentPaperWidth = te::layout::Property::GetValueAs<double>(pCurrentPaperWidth);
  double currentPaperHeight = te::layout::Property::GetValueAs<double>(pCurrentPaperHeight);

  if (properties.contains("paper_width") || properties.contains("paper_height"))
  {
    double newPaperWidth = currentPaperWidth;
    double newPaperHeight = currentPaperHeight;

    if (properties.contains("paper_width"))
    {
      const Property& pNewPaperWidth = properties.getProperty("paper_width");
      newPaperWidth = te::layout::Property::GetValueAs<double>(pNewPaperWidth);
    }
    if (properties.contains("paper_height"))
    {
      const Property& pNewPaperHeight = properties.getProperty("paper_height");
      newPaperHeight = te::layout::Property::GetValueAs<double>(pNewPaperHeight);
    }

    propertiesCopy = handleNewPaperSize(newPaperWidth, newPaperHeight);
    for (size_t i = 0; i < properties.getProperties().size(); ++i)
    {
      propertiesCopy.addProperty(properties.getProperties()[i]);
    }
  }
  AbstractItemController::setProperties(propertiesCopy);
}

te::layout::Properties te::layout::PaperController::handleNewPaperSize(double paperWidth, double paperHeight)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pShadowPadding = getProperty("shadow_padding");
  double shadowPadding = te::layout::Property::GetValueAs<double>(pShadowPadding);

  double width = paperWidth + shadowPadding;
  double height = paperHeight + shadowPadding;

  Properties properties;
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

  return properties;
}

