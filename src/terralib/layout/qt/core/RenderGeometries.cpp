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
  \file RenderGeometries.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "RenderGeometries.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/property/Properties.h"
#include "../../core/enum/Enums.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/geometry/Coord2D.h"
#include "terralib/geometry/Point.h"
#include "../core/Scene.h"
#include "../core/View.h"

// Qt
#include <QPainterPath>
#include <QPixmap>
#include <QPointF>
#include <QPolygonF>
#include <QPainter>

te::layout::RenderGeometries::RenderGeometries(View* view, DrawGeometries type) :
  m_view(view),
  m_geometryType(type)
{
 
}

te::layout::RenderGeometries::~RenderGeometries()
{

}

void te::layout::RenderGeometries::drawGeometries(QPaintDevice* device, const std::vector<te::gm::Point>& coords)
{
  QPainter p;
  p.begin(device);

  p.setPen(m_pen);
  p.setBrush(m_brush);

  QTransform transf = m_view->viewportTransform();
  p.setTransform(transf);

  draw(p, coords);

  p.end();
}

void te::layout::RenderGeometries::draw(QPainter& p, const std::vector<te::gm::Point>& coords)
{
  if (m_geometryType == te::layout::DRAW_POINT)
  {
    QVector<QPointF> points = getQPoints(coords);
    if (!points.isEmpty())
      p.drawPoints(points);
  }
  if (m_geometryType == te::layout::DRAW_LINE)
  {
    QPainterPath path = getQLines(coords);
    if (!path.isEmpty())
      p.drawPath(path);
  }
  if (m_geometryType == te::layout::DRAW_POLYGON)
  {
    QPolygonF poly = getQPolygon(coords);
    if (!poly.isEmpty())
      p.drawPolygon(poly);
  }
}

QVector<QPointF> te::layout::RenderGeometries::getQPoints(const std::vector<te::gm::Point>& coords)
{
  QVector<QPointF> points;

  std::size_t nPoints = coords.size();

  for (std::size_t pIdx = 0; pIdx < nPoints; ++pIdx)
  {
    QPointF pt(coords[pIdx].getX(), coords[pIdx].getY());
    points.append(pt);
  }

  return points;
}

QPainterPath te::layout::RenderGeometries::getQLines(const std::vector<te::gm::Point>& coords)
{
  QVector<QPointF> points = getQPoints(coords);
  QPainterPath path;

  if (!points.isEmpty())
  {
    path = QPainterPath(points.front());

    for (int i = 1; i < points.size(); ++i)
    {
      path.lineTo(points[i]);
    }
  }
  return path;
}

QPolygonF te::layout::RenderGeometries::getQPolygon(const std::vector<te::gm::Point>& coords)
{
  QPolygonF poly;

  std::size_t nPoints = coords.size();

  for (std::size_t pIdx = 0; pIdx < nPoints; ++pIdx)
  {
    QPointF pt(coords[pIdx].getX(), coords[pIdx].getY());
    poly.push_back(pt);
  }

  return poly;
}

void te::layout::RenderGeometries::setPen(const QPen& pen)
{
  m_pen = pen;
}

void te::layout::RenderGeometries::setBrush(const QBrush& brush)
{
  m_brush = brush;
}

te::layout::DrawGeometries te::layout::RenderGeometries::getTypeGeometry()
{
  return m_geometryType;
}

void te::layout::RenderGeometries::setTypeGeometry(DrawGeometries type)
{
  m_geometryType = type;
}

