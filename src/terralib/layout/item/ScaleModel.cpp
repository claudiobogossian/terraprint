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
  \file ScaleModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ScaleModel.h"
#include "../core/enum/Enums.h"
#include "../core/enum/EnumScaleType.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"
#include "../core/property/SharedProperties.h"
#include "../core/pattern/mvc/AbstractItemView.h"
#include "../core/Font.h"
#include "../core/Utils.h"

te::layout::ScaleModel::ScaleModel()
  : AbstractItemModel()
{
  double width = 75.;
  double height = 25.;
  double scale = 250000.;
  double scaleGapX = 28.;
  double scaleGapY = 3.;
  std::string itemName = "";
  Font font;
  double scaleUnitGapX = 10.0;
  bool onlyFirstAndLastValue = false;
  bool byBreaks = true;
  int numberOfBreaks = 2;
  te::color::RGBAColor fontColor(0, 0, 0, 255);
  double lineWidth = Utils::getLineWidthMinimumValue();
  double displacement = 2.;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  SharedProperties sharedProps;

//adding properties
  {
    Property property(0);
    property.setName("scale");
    property.setLabel(TR_LAYOUT("Scale"));
    property.setValue(scale, dataType->getDataTypeDouble());
    property.setEditable(false);
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("scale_width_rect_gap");
    property.setLabel(TR_LAYOUT("Scale Gap X"));
    property.setValue(scaleGapX, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("scale_height_rect_gap");
    property.setLabel(TR_LAYOUT("Scale Gap Y"));
    property.setValue(scaleGapY, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }

  {
    std::string value = TR_LAYOUT("Settings");
    Property property(0);
    property.setName("scaleSettings");
    property.setLabel(TR_LAYOUT("Scale Settings"));
    property.setValue(value, dataType->getDataTypeScaleSettings());
    property.setSerializable(false);
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  // Observer pattern relationship. Associate: != 0 / Dissociate : == 0.
  {
    Property property(0);
    property.setName(sharedProps.getItemObserver());
    property.setLabel(TR_LAYOUT("Connection with"));
    property.setComposeWidget(true);
    property.setValue(itemName, dataType->getDataTypeItemObserver());
    m_properties.addProperty(property);
  }

  {
    EnumScaleType enumScaleType;
    EnumType* currentScaleType = enumScaleType.getDoubleAlternatingScaleBarType();

    Property property(0);
    property.setName("scale_type");
    property.setLabel(TR_LAYOUT("Graphic type"));
    property.setValue(currentScaleType->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentScaleType->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for (int i = 0; i < enumScaleType.size(); ++i)
    {
      EnumType* enumType = enumScaleType.getEnum(i);

      if (enumType == enumScaleType.getNoneType() || enumType == currentScaleType)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("line_width");
    property.setLabel(TR_LAYOUT("Line Width"));
    property.setVisible(false);
    property.setValue(lineWidth, dataType->getDataTypeDouble());
    this->m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("font");
    property.setLabel(TR_LAYOUT("Font"));
    property.setValue(font, dataType->getDataTypeFont());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("font_color");
    property.setLabel(TR_LAYOUT("Font Color"));
    property.setValue(fontColor, dataType->getDataTypeColor());
    property.setVisible(false);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("Unit");
    property.setLabel(TR_LAYOUT("Unit"));
    property.setValue(std::string("km"), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(std::string("km"), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    v.setValue(std::string("m"), dataType->getDataTypeString());
    property.addOption(v);

    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("scale_in_unit_width_rect_gap");
    property.setLabel(TR_LAYOUT("Scale Gap X In Unit"));
    property.setValue(scaleUnitGapX, dataType->getDataTypeDouble(), true, 1);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("only_first_and_last_value");
    property.setLabel(TR_LAYOUT("Only the first and the last value"));
    property.setValue(onlyFirstAndLastValue, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("by_breaks");
    property.setLabel(TR_LAYOUT("By Breaks"));
    property.setVisible(false);
    property.setValue(byBreaks, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("number_of_breaks");
    property.setLabel(TR_LAYOUT("Number of graphical scale breaks"));
    property.setValue(numberOfBreaks, dataType->getDataTypeInt());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("displacementBetweenScaleAndText");
    property.setLabel(TR_LAYOUT("Displacement Between Scale And Text"));
    property.setValue(displacement, dataType->getDataTypeDouble());
    property.setVisible(false);
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

  reparentProperties(Enums::getInstance().getEnumObjectType()->getScaleItem());
}

te::layout::ScaleModel::~ScaleModel()
{
}
