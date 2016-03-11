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
  \file NorthSettingsConfigProperties.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "NorthSettingsConfigProperties.h"

te::layout::NorthSettingsConfigProperties::NorthSettingsConfigProperties():
  m_btnColor("color"),
  m_doubleSpinBoxNorthWidth("width"),
  m_doubleSpinBoxNorthHeight("height"),
  m_cbNorth("northArrow_type")
{
  
}

te::layout::NorthSettingsConfigProperties::~NorthSettingsConfigProperties()
{

}

std::string te::layout::NorthSettingsConfigProperties::getBtnColor()
{
  return m_btnColor;
}


std::string te::layout::NorthSettingsConfigProperties::getDoubleSpinBoxNorthHeight()
{
  return m_doubleSpinBoxNorthHeight;
}

std::string te::layout::NorthSettingsConfigProperties::getDoubleSpinBoxNorthWidth()
{
  return m_doubleSpinBoxNorthWidth;
}

std::string te::layout::NorthSettingsConfigProperties::getCbNorth()
{
  return m_cbNorth;
}

