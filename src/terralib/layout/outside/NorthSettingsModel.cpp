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
  \file NorthSettingsModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "NorthSettingsModel.h"
#include "../core/enum/Enums.h"

te::layout::NorthSettingsModel::NorthSettingsModel() :
  AbstractOutsideModel()
{
  m_type = Enums::getInstance().getEnumObjectType()->getNorthSettings();

  m_box = te::gm::Envelope(0., 0., 200., 200.);
}

te::layout::NorthSettingsModel::~NorthSettingsModel()
{

}

void te::layout::NorthSettingsModel::setNorthProperties(te::layout::Properties properties)
{
  m_properties = properties;
}

te::layout::Property te::layout::NorthSettingsModel::containsProperty(std::string name, EnumType* enumType)
{
  Property prop;

  if (!containsNorth(enumType))
  {
    return prop;
  }

  prop = m_properties.getProperty(name);
  return prop;
}

bool te::layout::NorthSettingsModel::updateProperty(Property prop, EnumType* enumType)
{
  bool result = false;
  if (!containsNorth(enumType))
  {
    return result;
  }
  
  result = m_properties.updateProperty(prop);
  return result;
}

bool te::layout::NorthSettingsModel::containsNorth(EnumType* enumType)
{
  bool result = false;
  if (m_properties.getTypeObj() == enumType)
  {
    result = true;
  }
  return result;
}


