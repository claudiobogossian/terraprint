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
  \file GridSettingsConfigProperties.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridSettingsConfigProperties.h"

te::layout::GridSettingsConfigProperties::GridSettingsConfigProperties(const std::string& prefix) 
  : m_prefix(prefix + "_"),
    m_name(m_prefix + "grid_name"),
    m_visible(m_prefix + "grid_visible"),
    m_horizontalLineGap(m_prefix + "grid_horizontalLineGap"),
    m_verticalLineGap(m_prefix + "grid_verticalLineGap"),
    m_horizontalLineInitial(m_prefix + "grid_horizontalLineInitial"),
    m_verticalLineInitial(m_prefix + "grid_verticalLineInitial"),
    m_style(m_prefix + "grid_style"),
    m_lineStyle(m_prefix + "grid_lineStyle"),
    m_lineColor(m_prefix + "grid_lineColor"),
    m_lineWidth(m_prefix + "grid_lineWidth"),
    m_font(m_prefix + "grid_font"),
    m_fontCorner(m_prefix + "grid_font_corner"),
    m_textColor(m_prefix + "grid_textColor"),
    m_visibleAllTexts(m_prefix + "grid_visibleAllTexts"),
    m_superscriptText(m_prefix + "grid_superscriptText"),
    m_horizontalLineDisplacement(m_prefix + "grid_horizontalLineDisplacement"),
    m_verticalLineDisplacement(m_prefix + "grid_verticalLineDisplacement"),
    m_bottomText(m_prefix + "grid_bottomText"),
    m_leftText(m_prefix + "grid_leftText"),
    m_rightText(m_prefix + "grid_rightText"),
    m_topText(m_prefix + "grid_topText"),
    m_bottomRotateText(m_prefix + "grid_bottomRotateText"),
    m_leftRotateText(m_prefix + "grid_leftRotateText"),
    m_rightRotateText(m_prefix + "grid_rightRotateText"),
    m_topRotateText(m_prefix + "grid_topRotateText"),
    m_crossOffset(m_prefix + "grid_crossOffset"),
    m_borderIntersection(m_prefix + "grid_borderIntersections"),
    m_gridSettings(m_prefix + "grid_settings"),
    m_planarSRID(m_prefix + "planar_srid"),
    m_showFrame(m_prefix + "grid_showFrame"),
    m_frameThickness(m_prefix + "grid_frameThickness"),
    m_frameColor(m_prefix + "grid_frameColor")
{

}

te::layout::GridSettingsConfigProperties::~GridSettingsConfigProperties()
{

}

const std::string& te::layout::GridSettingsConfigProperties::getName() const
{
  return m_name;
}

const std::string& te::layout::GridSettingsConfigProperties::getVisible() const
{
  return m_visible;
}

const std::string& te::layout::GridSettingsConfigProperties::getHorizontalLineGap() const
{
  return m_horizontalLineGap;
}

const std::string& te::layout::GridSettingsConfigProperties::getVerticalLineGap() const
{
  return m_verticalLineGap;
}

const std::string& te::layout::GridSettingsConfigProperties::getHorizontalLineInitial() const
{
  return m_horizontalLineInitial;
}

const std::string& te::layout::GridSettingsConfigProperties::getVerticalLineInitial() const
{
  return m_verticalLineInitial;
}

const std::string& te::layout::GridSettingsConfigProperties::getStyle() const
{
  return m_style;
}

const std::string& te::layout::GridSettingsConfigProperties::getLineStyle() const
{
  return m_lineStyle;
}

const std::string& te::layout::GridSettingsConfigProperties::getLineColor() const
{
  return m_lineColor;
}

const std::string& te::layout::GridSettingsConfigProperties::getLineWidth() const
{
  return m_lineWidth;
}

const std::string& te::layout::GridSettingsConfigProperties::getFont() const
{
  return m_font;
}

const std::string& te::layout::GridSettingsConfigProperties::getFontCorner() const
{
  return m_fontCorner;
}

const std::string& te::layout::GridSettingsConfigProperties::getTextColor() const
{
  return m_textColor;
}

const std::string& te::layout::GridSettingsConfigProperties::getVisibleAllTexts() const
{
  return m_visibleAllTexts;
}

const std::string& te::layout::GridSettingsConfigProperties::getSuperscriptText() const
{
  return m_superscriptText;
}

const std::string& te::layout::GridSettingsConfigProperties::getHorizontalLineDisplacement() const
{
  return m_horizontalLineDisplacement;
}

const std::string& te::layout::GridSettingsConfigProperties::getVerticalLineDisplacement() const
{
  return m_verticalLineDisplacement;
}

const std::string& te::layout::GridSettingsConfigProperties::getBottomText() const
{
  return m_bottomText;
}

const std::string& te::layout::GridSettingsConfigProperties::getLeftText() const
{
  return m_leftText;
}

const std::string& te::layout::GridSettingsConfigProperties::getRightText() const
{
  return m_rightText;
}

const std::string& te::layout::GridSettingsConfigProperties::getTopText() const
{
  return m_topText;
}

const std::string& te::layout::GridSettingsConfigProperties::getBottomRotateText() const
{
  return m_bottomRotateText;
}

const std::string& te::layout::GridSettingsConfigProperties::getLeftRotateText() const
{
  return m_leftRotateText;
}

const std::string& te::layout::GridSettingsConfigProperties::getRightRotateText() const
{
  return m_rightRotateText;
}

const std::string& te::layout::GridSettingsConfigProperties::getTopRotateText() const
{
  return m_topRotateText;
}

const std::string& te::layout::GridSettingsConfigProperties::getCrossOffset() const
{
  return m_crossOffset;
}

const std::string& te::layout::GridSettingsConfigProperties::getBorderIntersections() const
{
  return m_borderIntersection;
}

const std::string& te::layout::GridSettingsConfigProperties::getGridSettings() const
{
  return m_gridSettings;
}

const std::string& te::layout::GridSettingsConfigProperties::getPlanarSRID() const
{
  return m_planarSRID;
}

const std::string& te::layout::GridSettingsConfigProperties::getShowFrame() const
{
  return  m_showFrame;
}

const std::string& te::layout::GridSettingsConfigProperties::getFrameThickness() const
{
  return m_frameThickness;
}

const std::string& te::layout::GridSettingsConfigProperties::getFrameColor() const
{
  return m_frameColor;
}