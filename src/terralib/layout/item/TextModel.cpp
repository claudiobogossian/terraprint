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
  \file TextModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextModel.h"
#include "../core/enum/Enums.h"
#include "../core/enum/EnumAlignmentType.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"

te::layout::TextModel::TextModel()
  : AbstractItemModel()
{
  Font font;
  std::string text = "Text Item";
  te::color::RGBAColor color(0, 0, 0, 255);
  double width = 40.;
  double height = 20.;
  double dx = 0.;
  double dy = 0.;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

//adding properties
  {
    Property property(0);
    property.setName("font");
    property.setLabel(TR_LAYOUT("Font"));
    property.setValue(font, dataType->getDataTypeFont());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("text");
    property.setLabel(TR_LAYOUT("Text"));
    property.setValue(text, dataType->getDataTypeString());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("color");
    property.setLabel(TR_LAYOUT("Color"));
    property.setValue(color, dataType->getDataTypeColor());
    m_properties.addProperty(property);
  }


  {
    Property property(0);
    property.setName("dx");
    property.setVisible(false);
    property.setValue(dx, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }


  {
    Property property(0);
    property.setName("dy");
    property.setVisible(false);
    property.setValue(dy, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }

  
  {
    EnumAlignmentType enumAlignmentType;
    EnumType* currentAlignmentType = enumAlignmentType.getAlignmentLeftType();

    Property property(0);
    property.setName("alignment");
    property.setLabel(TR_LAYOUT("Alignment"));
    property.setValue(currentAlignmentType->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentAlignmentType->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for(int i = 0 ; i < enumAlignmentType.size() ; ++i)
    {
      EnumType* enumType = enumAlignmentType.getEnum(i);

      if(enumType == enumAlignmentType.getNoneType() || enumType == currentAlignmentType)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }
    m_properties.addProperty(property);
  }

//updating properties
  {
    Property property(0);
    property.setName("width");
    property.setValue(width, dataType->getDataTypeDouble());
    this->m_properties.updateProperty(property);
  }

  {
    Property property(0);
    property.setName("height");
    property.setValue(height, dataType->getDataTypeDouble());
    this->m_properties.updateProperty(property);
  }

  {
    Property property(0);
    property.setName("resizable");
    property.setValue(true, dataType->getDataTypeBool());
    this->m_properties.updateProperty(property);
  }

  {
    Property property(0);
    property.setName("keep_aspect");
    property.setValue(true, dataType->getDataTypeBool());
    this->m_properties.updateProperty(property);
  }
  
  {
    Property property(0);
    property.setName("editable");
    property.setValue(true, dataType->getDataTypeBool());
    this->m_properties.updateProperty(property);
  }

  reparentProperties(Enums::getInstance().getEnumObjectType()->getTextItem());
}

te::layout::TextModel::~TextModel()
{
}

