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
  te::color::RGBAColor headerHorizontalColor(192, 192, 192, 255);
  te::color::RGBAColor headerVerticalColor(255, 255, 255, 0);
  std::string text("Body Title");
  double item_width = 190.;
  double item_height = 170.;
  Font titleFont;
  titleFont.setPointSize(20);
  Font bodyTitleFont;
  bodyTitleFont.setPointSize(20);

  Font m_font;

  te::color::RGBAColor colorTitle(0, 0, 0, 255);
  te::color::RGBAColor colorBodyTitle(0, 0, 0, 255);
  this->m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getTitleItem());

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
    Property property(0);
    property.setName("text");
    property.setValue(text, dataType->getDataTypeString());
    m_properties.updateProperty(property);
  }

  {
    Property property(0);
    property.setName("title_font");
    property.setLabel(TR_LAYOUT("Title Font"));
    property.setValue(titleFont, dataType->getDataTypeFont());
    m_properties.addProperty(property);
  }

  {
    Property property = m_properties.getProperty("font");
    property.setVisible(false);
    property.setValue(m_font, dataType->getDataTypeFont());
    m_properties.completelyUpdateProperty(property);
  }

/*  {
    Property property(0);
    property.setName("colorTitle");
    property.setLabel(TR_LAYOUT("Title Color"));
    property.setValue(colorTitle, dataType->getDataTypeColor());
    m_properties.addProperty(property);
  }
  */
  {
    Property property(0);
    property.setName("bodytitle_font");
    property.setLabel(TR_LAYOUT("Body Title Font"));
    property.setValue(bodyTitleFont, dataType->getDataTypeFont());
    m_properties.addProperty(property);
  }
  /*
  {
    Property property(0);
    property.setName("colorBodyTitle");
    property.setLabel(TR_LAYOUT("Body Title Color"));
    property.setValue(colorBodyTitle, dataType->getDataTypeColor());
    m_properties.addProperty(property);
  }
  */
  {
    Property propertyGridSettings(0);
    propertyGridSettings.setName(propertyNames.getName());
    propertyGridSettings.setLabel(TR_LAYOUT("Settings"));
    std::string sValuePlanar = "Settings";
    propertyGridSettings.setMenu(true);
    propertyGridSettings.setValue(sValuePlanar, dataType->getDataTypeTextGridSettings());


    {
      Property property(0);
      property.setName(propertyNames.getTitleBackgroundColor());
      property.setLabel(TR_LAYOUT("Title Background Color"));
      property.setValue(headerHorizontalColor, dataType->getDataTypeColor());
      propertyGridSettings.addSubProperty(property);
    }
    {
      Property property(0);
      property.setName(propertyNames.getBodyBackgroundColor());
      property.setLabel(TR_LAYOUT("Body Background Color"));
      property.setValue(headerVerticalColor, dataType->getDataTypeColor());
      propertyGridSettings.addSubProperty(property);
    }

    m_properties.addProperty(propertyGridSettings);

  }

//updating properties


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
  
}

te::layout::TitleModel::~TitleModel()
{

}
