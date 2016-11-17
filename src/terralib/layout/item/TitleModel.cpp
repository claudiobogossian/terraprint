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
  \file TitleModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TitleModel.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/color/RGBAColor.h"
#include "../core/enum/Enums.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/property/TextGridSettingsConfigProperties.h"

te::layout::TitleModel::TitleModel()
  : TextModel()
{
  TextGridSettingsConfigProperties propertyNames;

  std::string title("Title");
  std::string text("Text");
  double item_width = 190.;
  double item_height =  170.;
  double spacing = 1;
  double padding = 1;
  double width = 100;
  bool addRow = false;
  int rowNumber = 2;
  bool addColumn = false;
  int columnNumber = 1;
  double columnWidth = 100;
  double borderWidth = 0.5;
  te::gm::Envelope box(0., 0., 190., 170.);
  te::color::RGBAColor tableColor(0, 0, 0, 255);
  te::color::RGBAColor borderGridColor(0, 0, 0, 255);
  te::color::RGBAColor headerHorizontalColor(255, 255, 255, 0);
  te::color::RGBAColor headerVerticalColor(192, 192, 192, 255);
  te::color::RGBAColor evenRow;
  te::color::RGBAColor oddRow;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

//adding properties
  {
    Property property(0);
    property.setName(propertyNames.getTitle());
    property.setLabel("Title");
    property.setValue(title, dataType->getDataTypeString());
    m_properties.addProperty(property);
  }

  {
    Property propertyGridSettings(0);  
    propertyGridSettings.setName(propertyNames.getName());
    propertyGridSettings.setLabel(TR_LAYOUT("Settings"));
    std::string sValuePlanar = TR_LAYOUT("Settings");
    propertyGridSettings.setMenu(true);
    propertyGridSettings.setValue(sValuePlanar, dataType->getDataTypeTextGridSettings());

    //adding subproperties
    {
      Property property(0);
      property.setName(propertyNames.getSpacing());
      property.setLabel(TR_LAYOUT("Spacing"));
      property.setValue(spacing, dataType->getDataTypeDouble());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getPadding());
      property.setLabel(TR_LAYOUT("Padding"));
      property.setValue(padding, dataType->getDataTypeDouble());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getTableColor());
      property.setLabel(TR_LAYOUT("Color"));
      property.setValue(tableColor, dataType->getDataTypeColor());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getWidth());
      property.setLabel(TR_LAYOUT("Width"));
      property.setValue(width, dataType->getDataTypeDouble());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getRowNumber());
      property.setLabel(TR_LAYOUT("Number of rows"));
      property.setValue(rowNumber, dataType->getDataTypeInt());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getEvenRow());
      property.setLabel(TR_LAYOUT("Even Row Color"));
      property.setValue(evenRow, dataType->getDataTypeColor());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getOddRow());
      property.setLabel(TR_LAYOUT("Odd Row Color"));
      property.setValue(oddRow, dataType->getDataTypeColor());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getColumnNumber());
      property.setLabel(TR_LAYOUT("Number of columns"));
      property.setValue(columnNumber, dataType->getDataTypeInt());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getColumnWidth());
      property.setLabel(TR_LAYOUT("Column Width"));
      property.setValue(columnWidth, dataType->getDataTypeDouble());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getBorderWidth());
      property.setLabel(TR_LAYOUT("Border Width"));
      property.setValue(borderWidth, dataType->getDataTypeDouble());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getBorderGridColor());
      property.setLabel(TR_LAYOUT("Border Grid Color"));
      property.setValue(borderGridColor, dataType->getDataTypeColor());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getHeaderHorizontalColor());
      property.setLabel(TR_LAYOUT("Horizontal Header Color"));
      property.setValue(headerHorizontalColor, dataType->getDataTypeColor());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getHeaderVerticalColor());
      property.setLabel(TR_LAYOUT("Vertical Header Color"));
      property.setValue(headerVerticalColor, dataType->getDataTypeColor());
      propertyGridSettings.addSubProperty(property);
    }

    m_properties.addProperty(propertyGridSettings);
  }

//updating properties
  {
    Property property(0);
    property.setName("text");
    property.setValue(text, dataType->getDataTypeString());
    m_properties.updateProperty(property);
  }
  {
    Property property(0);
    property.setName("width");
    property.setValue(item_width, dataType->getDataTypeDouble());
    m_properties.updateProperty(property);
  }
  {
    Property property(0);
    property.setName("height");
    property.setValue(item_height, dataType->getDataTypeDouble());
    m_properties.updateProperty(property);
  }
  
  reparentProperties(Enums::getInstance().getEnumObjectType()->getTitleItem());
}

te::layout::TitleModel::~TitleModel()
{

}
