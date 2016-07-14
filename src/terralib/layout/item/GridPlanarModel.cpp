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
  \file GridPlanarModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridPlanarModel.h"

#include "../core/property/GridSettingsConfigProperties.h"
#include "../core/property/SharedProperties.h"
#include "../core/property/Properties.h"
#include "../core/property/PlanarGridSettingsConfigProperties.h"


// STL
#include <string>

te::layout::GridPlanarModel::GridPlanarModel()
  : GridMapModel()
{
  LayoutUnit unit(StyleKilometer);
  te::gm::Envelope planarBox(0, 0, 10000, 10000);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  PlanarGridSettingsConfigProperties settingsConfig;

  Property prop_gridsettings = m_properties.getProperty("GridSettings");

  bool superscript = false;

  //adding properties
  {
    Property property(0);
    property.setName(settingsConfig.getUnit());
    property.setLabel(TR_LAYOUT("Unit"));
    property.setValue(std::string("km"), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(std::string("km"), dataType->getDataTypeString());
    property.addOption(v);

    v.setValue(std::string("m"), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }

  // Reference Settings
  {
    Property property(0);
    property.setName("planar_box");
    property.setLabel(TR_LAYOUT("Planar Box"));
    property.setVisible(false);
    property.setValue(planarBox, dataType->getDataTypeEnvelope());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getSuperscriptText());
    property.setLabel(TR_LAYOUT("Superscript"));
    property.setComposeWidget(true);
    property.setVisible(true); 
    property.setValue(superscript, dataType->getDataTypeBool());
    m_properties.completelyUpdateProperty(property);
  }

  reparentProperties(Enums::getInstance().getEnumObjectType()->getGridPlanarItem());
}

te::layout::GridPlanarModel::~GridPlanarModel()
{
}
