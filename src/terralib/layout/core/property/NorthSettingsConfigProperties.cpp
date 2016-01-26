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
m_lineColor("btnColor")
{
  
}
  /*m_name("grid_name"),
  m_visible("grid_visible"),
  m_lneHrzGap("grid_lneHrzGap"),
  m_lneVrtGap("grid_lneVrtGap"),
  m_initialGridPointX("grid_initialGridPointX"),
  m_initialGridPointY("grid_initialGridPointY"),
  m_style("grid_style"),
  m_lineStyle("grid_lineStyle"),
  m_lineColor("grid_lineColor"),
  m_lineWidth("grid_lineWidth"),
  m_font("grid_font"),
  m_fontCorner("grid_font_corner"),
  m_textColor("grid_textColor"),
  m_visibleAllTexts("grid_visibleAllTexts"),
  m_superscriptText("grid_superscriptText"),
  m_lneVrtDisplacement("grid_lneVrtDisplacement"),
  m_lneHrzDisplacement("grid_lneHrzDisplacement"),
  m_bottomText("grid_bottomText"),
  m_leftText("grid_leftText"),
  m_rightText("grid_rightText"),
  m_topText("grid_topText"),
  m_bottomRotateText("grid_bottomRotateText"),
  m_leftRotateText("grid_leftRotateText"),
  m_rightRotateText("grid_rightRotateText"),
  m_topRotateText("grid_topRotateText"),
  m_crossOffset("grid_crossOffset")*/


te::layout::NorthSettingsConfigProperties::~NorthSettingsConfigProperties()
{

}

std::string te::layout::NorthSettingsConfigProperties::getLineColor()
{
  return m_lineColor;
}


//std::string te::layout::GridSettingsConfigProperties::getName()
//{
//  return m_name;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getVisible()
//{
//  return m_visible;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getLneHrzGap()
//{
//  return m_lneHrzGap;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getLneVrtGap()
//{
//  return m_lneVrtGap;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getInitialGridPointX()
//{
//  return m_initialGridPointX;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getInitialGridPointY()
//{
//  return m_initialGridPointY;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getStyle()
//{
//  return m_style;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getLineStyle()
//{
//  return m_lineStyle;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getLineColor()
//{
//  return m_lineColor;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getLineWidth()
//{
//  return m_lineWidth;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getFont()
//{
//  return m_font;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getFontCorner()
//{
//  return m_fontCorner;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getTextColor()
//{
//  return m_textColor;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getVisibleAllTexts()
//{
//  return m_visibleAllTexts;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getSuperscriptText()
//{
//  return m_superscriptText;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getLneVrtDisplacement()
//{
//  return m_lneVrtDisplacement;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getLneHrzDisplacement()
//{
//  return m_lneHrzDisplacement;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getBottomText()
//{
//  return m_bottomText;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getLeftText()
//{
//  return m_leftText;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getRightText()
//{
//  return m_rightText;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getTopText()
//{
//  return m_topText;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getBottomRotateText()
//{
//  return m_bottomRotateText;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getLeftRotateText()
//{
//  return m_leftRotateText;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getRightRotateText()
//{
//  return m_rightRotateText;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getTopRotateText()
//{
//  return m_topRotateText;
//}
//
//std::string te::layout::GridSettingsConfigProperties::getCrossOffset()
//{
//  return m_crossOffset;
//}
