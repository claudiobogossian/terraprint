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
  \file GridMapItem.h
   
   \brief Class that represents a graphic GridMap. 
      Its coordinate system is the same of scene (millimeters). 
      This is also son of ItemObserver and ObjectItem, so it can become observer of a model (Observable). 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRIDMAP_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_GRIDMAP_ITEM_H

// TerraLib
#ifndef Q_MOC_RUN
#include "AbstractItem.h"
#include "../../core/Config.h"
#include "terralib/geometry/LineString.h"
#endif

namespace te
{
  namespace layout
  {
    class Observable;
    class AbstractItemController;

    /*!
      \brief Class that represents a graphic GridMap. 
        Its coordinate system is the same of scene (millimeters). 
        He is also the son of ItemObserver and AbstractItem, so it can become observer of a model (Observable). 
    
      \ingroup layout

      \sa te::layout::AbstractItem
    */
    class TELAYOUTEXPORT GridMapItem : public AbstractItem
    {
      protected:
        typedef std::pair<std::string, QPointF> TextPosition;


      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        GridMapItem(AbstractItemController* controller);

        /*!
          \brief Destructor
         */
        virtual ~GridMapItem();

        /*!
        \brief This function is called every time the view must be updated
        */
        virtual void refresh();

        /*!
          \brief For any specific drawing, the item must reimplement this function
        */
        virtual void drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        /*!
          \brief Reimplemented from ParentItem
         */
        virtual QRectF boundingRect() const;

        virtual void calculateGrid() = 0;

      protected:

        /*!
          \brief Draws the grid
        */
        virtual void drawGridOnDevice(QPainter* painter);
        
        virtual void configPainter(QPainter* painter);

        virtual void configTextPainter(QPainter* painter);

        virtual void clear();

        virtual void clearLines();

        virtual void calculateTexts();

        virtual void calculateTopTexts();

        virtual void calculateBottomTexts();

        virtual void calculateLeftTexts();

        virtual void calculateRightTexts();

        virtual void calculateCrossLines();

        virtual bool calculateCrossIntersectMapBorder(QLineF vrt, QLineF hrz);

        virtual void calculateBoldersSegments(double crossOffSe);

        virtual void debugDrawTextRect(QPainter* painter, const QPointF& point, const std::string& text, QFont font, int rotate = 0);

        virtual te::gm::Envelope calculateTop(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double horizontalDisplacement);

        virtual te::gm::Envelope calculateBottom(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double horizontalDisplacement);

        virtual te::gm::Envelope calculateRight(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double verticalDisplacement);

        virtual te::gm::Envelope calculateLeft(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double verticalDisplacement);

        virtual bool checkBolderIntersection(const te::gm::LineString& bolderLine, const te::gm::LineString& gridLine, te::gm::Point& intersectionPoint);
        
        virtual void addGridLinesToPath();

        virtual bool isNearEdge(const te::gm::Point& p, const te::gm::LineString& line, const double& offSet);
        
        virtual void calculateText(const QPointF& point, const QFont& font, const std::string& text, int rotate);
        
        virtual void calculateSuperScriptText(const QPointF& point, const QFont& font, const std::string& text, int rotate);

        virtual  te::gm::LineString roundLineString(const te::gm::LineString& lines);
    
    
    protected:

        virtual bool validateHrzGap(const te::gm::Envelope& geoBox, double gap);

        virtual bool validateVrtGap(const te::gm::Envelope& geoBox, double gap);

        double                          m_maxWidthTextMM;
        double                          m_maxHeigthTextMM;
        bool                            m_changeSize;

        QList<te::gm::LineString>       m_verticalLines;
        QList<te::gm::LineString>       m_horizontalLines;

        std::vector<TextPosition>       m_topTexts;
        std::vector<TextPosition>       m_bottomTexts;
        std::vector<TextPosition>       m_rightTexts;
        std::vector<TextPosition>       m_leftTexts;

        te::gm::Envelope                m_boundingBox;
        int                             m_defaultRotate;

        bool                            m_showDebugDrawings;

        QPainterPath                    m_gridLines;
        QPainterPath                    m_gridCrosses;
        QPainterPath                    m_gridText;
        QPixmap                         m_screenCache; //!< A cache to avoid unnecessary full redraw
    };
  }
}

#endif
