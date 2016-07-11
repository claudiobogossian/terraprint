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
  \file GridGeodesicModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridGeodesicModel.h"

#include "../core/enum/EnumTextFormatType.h"
#include "../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../core/property/SharedProperties.h"
#include "../core/property/Properties.h"

// STL
#include <string>

te::layout::GridGeodesicModel::GridGeodesicModel() 
  : GridMapModel()
{
  te::gm::Envelope geographicBox(0, 0, 10000, 10000);
  bool showDegreesText = true;
  bool showMinutesText = true;
  bool showSecondsText = true;

  int secPresicion = 0;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  GeodesicGridSettingsConfigProperties settingsConfig;

  //adding properties
  // Reference Settings
  {
    Property property(0);
    property.setName("geographic_box");
    property.setLabel(TR_LAYOUT("Geographic Box"));
    property.setVisible(false);
    property.setValue(geographicBox, dataType->getDataTypeEnvelope());
    m_properties.addProperty(property);
  }

  Property prop_gridsettings = m_properties.getProperty("GridSettings");
  
  {
    Property property(0);
    property.setName(settingsConfig.getDegreesText());
    property.setLabel(TR_LAYOUT("Show Degrees Text"));
    property.setValue(showDegreesText, dataType->getDataTypeBool());
    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }
  {
    Property property(0);
    property.setName(settingsConfig.getMinutesText());
    property.setLabel(TR_LAYOUT("Show Minutes Text"));
    property.setValue(showMinutesText, dataType->getDataTypeBool());
    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }
  {
    Property property(0);
    property.setName(settingsConfig.getSecondsText());
    property.setLabel(TR_LAYOUT("Show Seconds Text"));
    property.setValue(showSecondsText, dataType->getDataTypeBool());
    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }
  {
    //seconds precision
    Property property(0);
    property.setName(settingsConfig.getSecondsPrecisionText());
    property.setLabel(TR_LAYOUT("Seconds Text Precision"));
    property.setValue(secPresicion, dataType->getDataTypeInt());
    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }

  // Text Format
  {
    EnumTextFormatType textFormatType;
    EnumType* defaultTextFormat = textFormatType.getDefaultFormat();

    Property property(0);
    property.setName(settingsConfig.getTextFormat());
    property.setLabel(TR_LAYOUT("Text Format"));
    property.setComposeWidget(true);
    property.setValue(defaultTextFormat->getLabel(), dataType->getDataTypeStringList());

    for (int i = 0; i < textFormatType.size(); ++i)
    {
      EnumType* enumType = textFormatType.getEnum(i);

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);

      if (defaultTextFormat->getName() == enumType->getName())
      {
        property.setOptionChoice(v);
      }
    }

    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }

  reparentProperties(Enums::getInstance().getEnumObjectType()->getGridGeodesicItem());
}

te::layout::GridGeodesicModel::~GridGeodesicModel()
{
}
