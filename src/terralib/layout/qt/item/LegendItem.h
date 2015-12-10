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
  \file LegendItem.h
   
   \brief Class that represents a graphic legend of a map.
   Its coordinate system is the same of scene (millimeters). 
   This is also the son of ItemObserver and ObjectItem, so it can become observer of a model (Observable). 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LEGEND_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_LEGEND_ITEM_H

// TerraLib
#include "AbstractItem.h"
#include "../../core/Config.h"
#include "terralib/maptools/GroupingItem.h"
#include "terralib/maptools/AbstractLayer.h"

// STL
#include <string>

#include <QGraphicsItem>
#include <QColor>
#include <QFont>

namespace te
{
  namespace gm
  {
    class Geometry;
  }
  namespace qt
  {
    namespace widgets
    {
      class Canvas;
    }
  }
  namespace se
  {
    class Symbolizer;
  }
  namespace layout
  {
    class AbstractItemController;
    /*!
    \brief Class that represents a graphic legend of a map.
        Its coordinate system is the same of scene (millimeters). 
        This is also the son of ItemObserver and ObjectItem, so it can become observer of a model (Observable).  
    
      \ingroup layout

      \sa te::layout::ObjectItem
    */
    class TELAYOUTEXPORT LegendItem : public AbstractItem<QGraphicsItem>
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        LegendItem(AbstractItemController* controller);

        /*!
          \brief Destructor
         */
        virtual ~LegendItem();

        virtual void refreshLegendProperties();

      protected:
        
        virtual void drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        virtual void drawLegend(QPainter* painter, te::map::AbstractLayerPtr layer, double x1, double& y1);

        virtual te::gm::Geometry* createGeometry(QRectF geomRect, te::se::Symbolizer* symbol);

        virtual te::gm::Geometry* createPolygonSymbolizer(QRectF geomRect, double offset = 2.0);

        virtual te::gm::Geometry* createLineSymbolizer(QRectF geomRect, double offset = 2.0);

        virtual te::gm::Geometry* createPointSymbolizer(QRectF geomRect);

        virtual void drawGeometry(QPainter* painter, QRectF geomRect, te::se::Symbolizer*symbol, te::gm::Geometry* geom);

        virtual te::qt::widgets::Canvas* createCanvas(QRectF rectMM, te::se::Symbolizer*symbol);
        
        virtual void drawLabel(QPainter* painter, QPointF point, QFont font, QColor fontColor, std::string text);

        virtual void drawTitle(QPainter* painter, double x1, double& y1, std::string title);

        virtual std::string getLabel(std::string propertyName, te::map::GroupingType type, te::map::GroupingItem* item);

        virtual void resizeMark(te::qt::widgets::Canvas* geomCanvas, te::se::Symbolizer*symbol, int pxWidth, int pxHeight);

        virtual void verticalAdjustmentBetweenPairs(double& y1, std::string label, double symbolSize);

        virtual QPointF verticalLegendTextAdjustment(double x1, double y1, std::string text);

        virtual QRectF verticalLegendGeomAdjustment(QRectF geom);
      
      protected:

        double                                m_maxHeight; //!< vertical center between text and symbol
        double                                m_displacementBetweenSymbols;
        double                                m_displacementBetweenSymbolsAndText;
        double                                m_symbolSize;
        std::list<te::map::AbstractLayerPtr>  m_layerList;
        int                                   m_borderDisplacement;
        int                                   m_dispBetweenTitleAndSymbols;

        QColor                                m_qFontLegendColor;
        QFont                                 m_qFontLegend;
        QColor                                m_qFontTitleColor;
        QFont                                 m_qFontTitle;
    };
  }
}

#endif
