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
  \file GridSettingsConfigProperties.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_CONFIG_PROPERTIES_H
#define __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_CONFIG_PROPERTIES_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    class TELAYOUTEXPORT GridSettingsConfigProperties
    {
      public:

        virtual ~GridSettingsConfigProperties();
        
        const std::string& getName() const;
        const std::string& getVisible() const;
        const std::string& getHorizontalLineGap() const;
        const std::string& getVerticalLineGap() const;
        const std::string& getHorizontalLineInitial() const;
        const std::string& getVerticalLineInitial() const;
        const std::string& getStyle() const;
        const std::string& getLineStyle() const;
        const std::string& getLineColor() const;
        const std::string& getLineWidth() const;
        const std::string& getFont() const;
        const std::string& getFontCorner() const;
        const std::string& getTextColor() const;
        const std::string& getVisibleAllTexts() const;
        const std::string& getSuperscriptText() const;
        const std::string& getHorizontalLineDisplacement() const;
        const std::string& getVerticalLineDisplacement() const;        
        const std::string& getBottomText() const;
        const std::string& getLeftText() const;
        const std::string& getRightText() const;
        const std::string& getTopText() const;
        const std::string& getBottomRotateText() const;
        const std::string& getLeftRotateText() const;
        const std::string& getRightRotateText() const;
        const std::string& getTopRotateText() const;
        const std::string& getCrossOffset() const;
        const std::string& getBorderIntersections() const;
        const std::string& getGridSettings() const;
        const std::string& getPlanarSRID() const;
              
      protected:

        //! Protected constructor
        GridSettingsConfigProperties(const std::string& prefix);

      protected:

        std::string m_prefix;

        std::string m_name;

        /* Grid */

        std::string m_visible;
        std::string m_horizontalLineGap;
        std::string m_verticalLineGap;
        std::string m_horizontalLineInitial;
        std::string m_verticalLineInitial;

        /* Just one is visible */
        std::string m_style;

        /* Line */
        std::string m_lineStyle;
        std::string m_lineColor;
        std::string m_lineWidth;

        /*Text: Basic Configuration*/
        std::string m_font;
        std::string m_fontCorner;
        std::string m_textColor;

        /*Text: Advanced configuration*/
        std::string m_visibleAllTexts;
        std::string m_superscriptText;
        std::string m_horizontalLineDisplacement;
        std::string m_verticalLineDisplacement;
        std::string m_bottomText;
        std::string m_leftText;
        std::string m_rightText;
        std::string m_topText;
        std::string m_bottomRotateText;
        std::string m_leftRotateText;
        std::string m_rightRotateText;
        std::string m_topRotateText;
        std::string m_crossOffset;

        std::string m_borderIntersection;
        std::string m_gridSettings;
        std::string m_planarSRID;
    };
  }
}

#endif