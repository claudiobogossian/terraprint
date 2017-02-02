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
  \file PointModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib

#include "PointModel.h"
#include "../core/enum/Enums.h"
#include "../core/enum/EnumPointType.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"
#include "../core/Utils.h"

#include <terralib/core/translator/Translator.h>

te::layout::PointModel::PointModel()
  : AbstractItemModel()
{
  te::color::RGBAColor fillColor(0, 0, 0, 255);
  te::color::RGBAColor contourColor(0, 0, 0, 255);
  te::color::RGBAColor backgroundColor(255, 0, 0, 255);
  double lineWidth = Utils::getLineWidthMinimumValue();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //adding properties
  {
    EnumPointType pointType;
    EnumType* currentType = pointType.getCircleType();

    Property property(0);
    property.setName("point_type");
    property.setLabel(TE_TR("Graphic type"));
    property.setValue(currentType->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentType->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for(int i = 0 ; i < pointType.size() ; ++i)
    {
      EnumType* enumType = pointType.getEnum(i);

      if(enumType == pointType.getNoneType() || enumType == currentType)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    this->m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("line_width");
    property.setLabel(TE_TR("Line Width"));
    property.setVisible(false);
    property.setValue(lineWidth, dataType->getDataTypeDouble());
    this->m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("fill_color");
    property.setLabel(TE_TR("Fill Color"));
    property.setValue(fillColor, dataType->getDataTypeColor());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("contour_color");
    property.setLabel(TE_TR("Contour Color"));
    property.setValue(contourColor, dataType->getDataTypeColor());
    property.setMenu(true);
    m_properties.addProperty(property);
  }
  
  reparentProperties(Enums::getInstance().getEnumObjectType()->getPointItem());
}

te::layout::PointModel::~PointModel()
{
}