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
  \file GeodesicGridSettingsConfigProperties.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GeodesicGridSettingsConfigProperties.h"

te::layout::GeodesicGridSettingsConfigProperties::GeodesicGridSettingsConfigProperties() 
  : GridSettingsConfigProperties("geodesic"),
  m_pointTextSizeCorner(m_prefix + "grid_pointTextSizeCorner"),
  m_fontTextCorner(m_prefix + "grid_fontTextCorner"),
  m_textColorCorner(m_prefix + "grid_textColorCorner"),
  m_degreesText(m_prefix + "grid_degreesText"),
  m_minutesText(m_prefix + "grid_minutesText"),
  m_secondsText(m_prefix + "grid_secondsText"),
  m_lneCornerHrzDisplacement(m_prefix + "grid_lneCornerHrzDisplacement"),
  m_lneCornerVrtDisplacement(m_prefix + "grid_lneCornerVrtDisplacement"),
  m_lowerRightCornerText(m_prefix + "grid_lowerRightCornerText"),
  m_upperRightCornerText(m_prefix + "grid_upperRightCornerText"),
  m_lowerLeftCornerText(m_prefix + "grid_lowerLeftCornerText"),
  m_upperLeftCornerText(m_prefix + "grid_upperLeftCornerText"),
  m_visibleCornerTextsText(m_prefix + "grid_visibleCornerTextsText"),
  m_secondsPrecisionText(m_prefix + "grid_seconds_precisionText"),
  m_textFormat(m_prefix + "text_format"),

  /* Geodesic: Topographic Map */

  m_defineScale(m_prefix + "grid_defineScale"),
  m_clip(m_prefix + "grid_clip"),
  m_scale(m_prefix + "grid_scale"),
  m_lneX1(m_prefix + "grid_lneX1"),
  m_lneX2(m_prefix + "grid_lneX2"),
  m_lneY1(m_prefix + "grid_lneY1"),
  m_lneY2(m_prefix + "grid_lneY2"),
  m_lneX3(m_prefix + "grid_lneX3"),
  m_lneX4(m_prefix + "grid_lneX4"),
  m_lneY3(m_prefix + "grid_lneY3"),
  m_lneY4(m_prefix + "grid_lneY4"),
  m_geodesicSRID(m_prefix + "geodesic_srid")
{
  
}

te::layout::GeodesicGridSettingsConfigProperties::~GeodesicGridSettingsConfigProperties( void )
{

}

std::string te::layout::GeodesicGridSettingsConfigProperties::getPointTextSizeCorner()
{
  return m_pointTextSizeCorner;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getFontTextCorner()
{
  return m_fontTextCorner;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getTextColorCorner()
{
  return m_textColorCorner;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getDegreesText()
{
  return m_degreesText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getMinutesText()
{
  return m_minutesText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getSecondsText()
{
  return m_secondsText;
}



std::string te::layout::GeodesicGridSettingsConfigProperties::getLneCornerHrzDisplacement()
{
  return m_lneCornerHrzDisplacement;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneCornerVrtDisplacement()
{
  return m_lneCornerVrtDisplacement;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLowerRightCornerText()
{
  return m_lowerRightCornerText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getUpperRightCornerText()
{
  return m_upperRightCornerText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLowerLeftCornerText()
{
  return m_lowerLeftCornerText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getUpperLeftCornerText()
{
  return m_upperLeftCornerText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getVisibleCornerTextsText()
{
  return m_visibleCornerTextsText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getSecondsPrecisionText()
{
  return m_secondsPrecisionText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getTextFormat()
{
  return m_textFormat;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getDefineScale()
{
  return m_defineScale;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getClip()
{
  return m_clip;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getScale()
{
  return m_scale;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneX1()
{
  return m_lneX1;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneX2()
{
  return m_lneX2;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneY1()
{
  return m_lneY1;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneY2()
{
  return m_lneY2;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneX3()
{
  return m_lneX3;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneX4()
{
  return m_lneX4;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneY3()
{
  return m_lneY3;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneY4()
{
  return m_lneY4;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getGeodesicSRID()
{
  return m_geodesicSRID;
}

