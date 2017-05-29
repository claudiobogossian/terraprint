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
  \file NorthModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "NorthModel.h"
#include "../core/enum/Enums.h"
#include "../core/enum/EnumNorthArrowType.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"
#include "../core/Utils.h"

#include <terralib/core/translator/Translator.h>

te::layout::NorthModel::NorthModel()
  : AbstractItemModel()
{
  te::gm::Envelope boundingBox(0., 0., 20., 20.);
  te::color::RGBAColor color(0, 0, 0, 255);
  std::string name = TE_TR("Settings");
  double lineWidth = Utils::getLineWidthMinimumValue();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //adding properties
  {
    EnumNorthArrowType northArrowType;
    EnumType* currentType = northArrowType.getNorthArrowType1();

    Property property(0);
    property.setName("northArrow_type");
    property.setLabel(TE_TR("Graphic Type"));
    property.setValue(currentType->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentType->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for(int i = 0 ; i < northArrowType.size() ; ++i)
    {
      EnumType* enumType = northArrowType.getEnum(i);

      if(enumType == northArrowType.getNoneType() || enumType == currentType)
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
    property.setName("color");
    property.setLabel(TE_TR("Color"));
    property.setValue(color, dataType->getDataTypeColor());
    property.setMenu(true);
    this->m_properties.addProperty(property);
  }

  // North settings
  Property prop_northsettings(0);
  prop_northsettings.setName("NorthSettings");
  prop_northsettings.setLabel(TE_TR("North Settings"));
  prop_northsettings.setMenu(true);
  prop_northsettings.setValue(name, dataType->getDataTypeNorthSettings());
  this->m_properties.addProperty(prop_northsettings);

  //updating properties
  {
    Property property(0);
    property.setName("width");
    property.setValue(boundingBox.getWidth(), dataType->getDataTypeDouble());
    this->m_properties.updateProperty(property);
  }

  {
    Property property(0);
    property.setName("height");
    property.setValue(boundingBox.getHeight(), dataType->getDataTypeDouble());
    this->m_properties.updateProperty(property);
  }

  reparentProperties(Enums::getInstance().getEnumObjectType()->getNorthItem());
}

te::layout::NorthModel::~NorthModel()
{

}

