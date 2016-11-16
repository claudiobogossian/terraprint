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
#include "SVGController.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "SVGItem.h"
#include <QtSvg/QSvgRenderer>


te::layout::SVGController::SVGController(te::layout::AbstractItemModel* model)
: AbstractItemController(model)
{
}

te::layout::SVGController::~SVGController()
{
}


void te::layout::SVGController::setProperties(const te::layout::Properties& properties)
{
  if (properties.contains("file_name") == false)
  {
    AbstractItemController::setProperties(properties);
    return;
  }

  const Property& pName = properties.getProperty("file_name");
  if(te::layout::Property::GetValueAs<std::string>(pName) != te::layout::Property::GetValueAs<std::string>(getProperty("file_name")))
  {
    QString fileName = ItemUtils::convert2QString(te::layout::Property::GetValueAs<std::string>(pName));
    calculateSVGRect(fileName);
    AbstractItemController::setProperties(properties);
  }
  else
  {
    AbstractItemController::setProperties(properties);
  }
}

void te::layout::SVGController::calculateSVGRect(QString& fileName)
{
  
  SVGItem *item = dynamic_cast<SVGItem*>(m_view);

  QRectF adjustBoundRect;

  if (item != 0)
  {

    QRectF boundrect = item->boundingRect();

    if (fileName.isEmpty())
    {
      return;
    }

    QSvgRenderer renderer(fileName);

    QRectF boundrectSVG = renderer.viewBoxF();

    double orientation = boundrectSVG.width() / boundrectSVG.height();
    
    if (orientation < 1.0)
    {
      adjustBoundRect.setHeight(boundrect.height());
      double newWidth = (boundrect.height() * boundrectSVG.width()) / boundrectSVG.height();
      adjustBoundRect.setWidth(newWidth);

      if (newWidth > boundrect.width())
      {
        adjustBoundRect.setWidth(boundrect.width());
        double newHeight = (boundrectSVG.height() * boundrect.width()) / boundrectSVG.width();
        adjustBoundRect.setHeight(newHeight);
      }
    }
    else
    {
      adjustBoundRect.setWidth(boundrect.width());
      double newHeight = (boundrectSVG.height() * boundrect.width()) / boundrectSVG.width();
      adjustBoundRect.setHeight(newHeight);

      if (newHeight > boundrect.height())
      {
        adjustBoundRect.setHeight(boundrect.height());
        double newWidth = (boundrect.height() * boundrectSVG.width()) / boundrectSVG.height();
        adjustBoundRect.setWidth(newWidth);
      }
    }

  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  updateBoundingRect(adjustBoundRect);
}

