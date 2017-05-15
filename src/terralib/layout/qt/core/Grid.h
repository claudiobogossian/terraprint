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
\file Grid.h

\brief A class that handles the creation and drawing of grids.
\ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRID_H
#define __TERRALIB_LAYOUT_INTERNAL_GRID_H

// TerraLib
#include "../../core/Config.h"

#include <cstdlib>
#include <vector>

#include <QPainterPath>

namespace te
{
  namespace gm
  {
    class Envelope;
    class LineString;
    class Point;
  }

  namespace layout
  {
    class Properties;
    class Property;
    class GridSettingsConfigProperties;

    /*!
    \brief Class that represents a grid.
    */
    class TELAYOUTEXPORT Grid
    {
      protected:
        typedef std::pair<std::string, QPointF> TextPosition;

      public:

        /*!
        \brief Constructor
        */
        Grid();

        /*!
        \brief Destructor
        */
        virtual ~Grid();

        void clear();

        QSizeF getSize() const;

        QPointF getOrigin() const;

        QPointF getFinal() const;

        virtual void initialize(const te::layout::Properties& gridSettings) = 0;

        virtual void drawGrid(QPainter* painter, const te::layout::Properties& properties, const GridSettingsConfigProperties& settingsConfig) const;

        /*
        \brief Larger text size on left
        */
        double getBiggestLeftText();

        /*
        \brief Larger text size on the right
        */
        double getBiggestRightText();

        /*
          \brief Larger text size at top
        */
        double getBiggestTopText();

        /*
          \brief Larger text size at bottom
        */
        double getBiggestBottomText();

        /*
        \brief Size of the space between the map and the left text
        */
        double getLeftSide();

        /*
        \brief Size of the space between the map and the right text
        */
        double getRightSide();

        /*
          \brief Size of the space between the map and the top text
        */
        double getTopSide();

        /*
        \brief Size of the space between the map and the bottom text
        */
        double getBottomSide();

    protected:

        //! Initializes the horizontal lines parameters
        virtual double initHorizontalLines(const te::gm::Envelope& geoBox, const Properties& gridSettings, const GridSettingsConfigProperties& settingsConfig) const;

        //! Initializes the vertical lines parameters
        virtual double initVerticalLines(const te::gm::Envelope& geoBox, const Properties& gridSettings, const GridSettingsConfigProperties& settingsConfig) const;

        virtual QPainterPath convertLinesToPath(const std::vector<te::gm::LineString>& verticalLines, const std::vector<te::gm::LineString>& horizontalLines) const;
        virtual QPainterPath convertCrossesToPath(const std::vector<te::gm::LineString>& verticalLines, const std::vector<te::gm::LineString>& horizontalLines, const Properties& properties, const GridSettingsConfigProperties& settingsConfig) const;

        virtual bool calculateCrossIntersectMapBorder(const QLineF& verticalLine, const QLineF& horizontalLine, const Properties& properties, const GridSettingsConfigProperties& settingsConfig, QPainterPath& crossLinesGrid) const;
        virtual void calculateBordersSegments(double crossOffSet, const std::vector<te::gm::LineString>& verticalLines, const std::vector<te::gm::LineString>& horizontalLines, const Properties& properties, const GridSettingsConfigProperties& settingsConfig, QPainterPath& crossLinesGrid) const;
        virtual bool checkBorderIntersection(const te::gm::LineString& bolderLine, const te::gm::LineString& gridLine, te::gm::Point& intersectionPoint) const;
        virtual bool isNearEdge(const te::gm::Point& p, const te::gm::LineString& line, const double& offSet) const;
        virtual te::gm::LineString roundLineString(const te::gm::LineString& lines) const;

        virtual te::gm::Envelope calculateTop(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double verticalLineDisplacement);
        virtual te::gm::Envelope calculateBottom(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double verticalLineDisplacement);
        virtual te::gm::Envelope calculateRight(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double horizontalLineDisplacement);
        virtual te::gm::Envelope calculateLeft(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double horizontalLineDisplacement);

        virtual void addTextToPath(const QPointF& point, const QFont& font, const std::string& text, int rotate);

        virtual void calculateSuperScriptText(const QPointF& point, const QFont& font, const std::string& text, int rotate);

        virtual void calculateTopTexts(const Properties& properties, const GridSettingsConfigProperties& settingsConfig);

        virtual void calculateBottomTexts(const Properties& properties, const GridSettingsConfigProperties& settingsConfig);

        virtual void calculateLeftTexts(const Properties& properties, const GridSettingsConfigProperties& settingsConfig);

        virtual void calculateRightTexts(const Properties& properties, const GridSettingsConfigProperties& settingsConfig);

        virtual void calculateTexts(const Properties& properties, const GridSettingsConfigProperties& settingsConfig);

        virtual void configPainterForGrid(QPainter* painter, const te::layout::Properties& pGridSettings, const GridSettingsConfigProperties& settingsConfig) const;

        virtual void configTextPainter(QPainter* painter, const te::layout::Properties& properties, const GridSettingsConfigProperties& settingsConfig) const;

    protected:

      std::size_t                     m_maxGaps; //!< Maximum number of gaps in each axis
      double                          m_defaultRotation; //!< Stores the default rotation of the texts
      QSizeF                          m_size;
      QPointF                         m_origin;
      QPointF                         m_final;

      QPainterPath                    m_gridLines; //!< Stores all the grid lines
      QPainterPath                    m_gridText; //!< Stores all the texts

      std::vector<TextPosition>       m_topTexts;
      std::vector<TextPosition>       m_bottomTexts;
      std::vector<TextPosition>       m_rightTexts;
      std::vector<TextPosition>       m_leftTexts;

      double                          m_biggestLeftText;
      double                          m_biggestRightText;
      double                          m_biggestTopText;
      double                          m_biggestBottomText;
    };
  }
}


#endif //__TERRALIB_LAYOUT_INTERNAL_GRID_H