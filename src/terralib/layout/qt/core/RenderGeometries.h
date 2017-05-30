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
  \file RenderGeometries.h
   
  \brief Class for drawing coordinates, according to the configured shape (Point, Line or Polygon), in scene coordinates.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_RENDER_GEOMETRIES_H
#define __TERRALIB_LAYOUT_INTERNAL_RENDER_GEOMETRIES_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/enum/AbstractType.h"

// STL
#include <vector>

// Qt
#include <QVector>
#include <QPen>
#include <QBrush>

class QPaintDevice;
class QPointF;
class QPainterPath;
class QPolygonF;
class QPainter;

namespace te
{
  namespace gm
  {
    class Point;
  }
  namespace layout
  {
    class View;
    /*!
    \brief Class for drawing coordinates, according to the configured shape (Point, Line or Polygon), in scene coordinates.
    
    \ingroup layout
    */
    class TELAYOUTEXPORT RenderGeometries
    {
      public:

        /*!
          \brief Constructor
        */
        RenderGeometries(View* view, DrawGeometries type);

        /*!
          \brief Destructor
        */
        virtual ~RenderGeometries();

        DrawGeometries getTypeGeometry();

        void setTypeGeometry(DrawGeometries type);

        void setPen(const QPen& pen);

        void setBrush(const QBrush& brush);

        virtual void drawGeometries(QPaintDevice* device, const std::vector<te::gm::Point>& coords);

      protected:

        virtual void draw(QPainter& p, const std::vector<te::gm::Point>& coords);

        virtual QVector<QPointF> getQPoints(const std::vector<te::gm::Point>& coords);

        virtual QPainterPath getQLines(const std::vector<te::gm::Point>& coords);

        virtual QPolygonF getQPolygon(const std::vector<te::gm::Point>& coords);

      protected:
        
        View*   m_view;
        QPen    m_pen;   //!< The pen used to draw the device shape.
        QBrush  m_brush; //!< The brush used to draw the device shape.

      private:

        DrawGeometries  m_geometryType;
    };
  }
}

#endif 
