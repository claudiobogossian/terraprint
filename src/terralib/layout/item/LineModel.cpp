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
  \file LineModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LineModel.h"
#include "../core/enum/Enums.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/enum/EnumLineStyleType.h"
#include "../core/enum/EnumType.h"
#include "../core/Utils.h"

#include <terralib/core/translator/Translator.h>

te::layout::LineModel::LineModel() 
  : AbstractItemModel()
{
  te::color::RGBAColor color(0, 0, 0, 255);
  te::gm::LineString* lineString = new te::gm::LineString(0, te::gm::LineStringType);
  double lineWidth = Utils::getLineWidthMinimumValue();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //adding properties
  {
    EnumLineStyleType lineStyleType;
    EnumType* currentType = lineStyleType.getStyleSolid();

    Property property(0);
    property.setName("line_style_type");
    property.setLabel(TE_TR("Line Style"));
    property.setValue(currentType->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentType->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for(int i = 0; i < lineStyleType.size(); i++)
    {
      EnumType* enumType = lineStyleType.getEnum(i);

      if(enumType == lineStyleType.getStyleNone() || enumType == currentType || enumType == lineStyleType.getStyleCustomDash())
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
    property.setValue(lineWidth, dataType->getDataTypeDouble());
    this->m_properties.addProperty(property);
  }
  
  {
    Property property(0);
    property.setName("color");
    property.setLabel(TE_TR("Color"));
    property.setValue(color, dataType->getDataTypeColor());
    property.setMenu(true);
    this->m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("geometry");
    property.setLabel(TE_TR("Geometry"));
    property.setVisible(false);
    property.setValue<te::gm::Geometry*>(lineString, dataType->getDataTypeGeometry());
    this->m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("resizable");
    property.setLabel(TE_TR("Resizable"));
    property.setValue<bool>(true, dataType->getDataTypeBool());
    this->m_properties.updateProperty(property);
  }
  
  reparentProperties(Enums::getInstance().getEnumObjectType()->getLineItem());
}

te::layout::LineModel::~LineModel()
{

}

