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

// TerraLib
#include "LineController.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "terralib/geometry/LineString.h"
#include "terralib/geometry/Coord2D.h"
#include "terralib/geometry/Point.h"
#include "LineItem.h"

// Qt
#include <QPolygonF>
#include <QRectF>
#include <QVector>
#include <QPointF>

te::layout::LineController::LineController(te::layout::AbstractItemModel* model)
: AbstractItemController(model), lastHalfLineWidth(0)
{

}

te::layout::LineController::~LineController()
{

}

void te::layout::LineController::update(const Subject* subject)
{
  verifyPolygon();
}

void te::layout::LineController::verifyPolygon()
{
  if (!isGeometrySizeChange())
  {
    return;
  }

  te::gm::GeometryShrPtr line = getGeometry();;

  if (line.get()->getNPoints() <= 0)
  {
    return;
  }

  //update with new points
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  Property property;
  property.setName("geometry");
  property.setValue(line, dataType->getDataTypeGeometry());
  m_model->setProperty(property);
}

te::gm::GeometryShrPtr te::layout::LineController::getGeometry()
{
  QPolygonF poly = recalculatePolygon();
  
  int polySize = poly.size();

  te::gm::LineString* lineString = new te::gm::LineString(polySize, te::gm::LineStringType);
  te::gm::GeometryShrPtr line(lineString);

  if (poly.empty())
  {
    return line;
  }

  double x = 0;
  double y = 0;
  
  unsigned int count = 0;

  foreach(QPointF pt, poly)
  {
    lineString->setPointN(count, te::gm::Point(pt.x(), pt.y()));
    ++count;
  }

  return line;
}

QPolygonF te::layout::LineController::getQPolygon()
{
  QPolygonF poly;

  const Property& pLine = m_model->getProperty("geometry");

  if (pLine.isNull())
    return poly;

  const te::gm::GeometryShrPtr geometryPtr = pLine.getValue().toGeometry();

  te::gm::LineString const* lineString = dynamic_cast< te::gm::LineString const * > (geometryPtr.get());
  std::size_t nPoints = lineString->size();
  te::gm::Coord2D const* coordsPtr = lineString->getCoordinates();
  for (std::size_t pIdx = 0; pIdx < nPoints; ++pIdx)
  {
    QPointF pt(coordsPtr[pIdx].getX(), coordsPtr[pIdx].getY());
    poly.push_back(pt);
  }
  return poly;
}

QPolygonF te::layout::LineController::recalculatePolygon()
{
  QPolygonF poly = getQPolygon();
  QRectF polygonRect = poly.boundingRect();

  if (poly.empty())
  {
    return poly;
  }

  const Property& pWidth = m_model->getProperty("width");
  const Property& pHeight = m_model->getProperty("height");

  double width = pWidth.getValue().toDouble();
  double height = pHeight.getValue().toDouble();

  double halfLineWidth = m_model->getProperty("line_width").getValue().toDouble();

  double xScale = width / (polygonRect.width() + halfLineWidth);
  double yScale = height / (polygonRect.height() + halfLineWidth);

  QTransform transf;
  transf.scale(xScale, yScale);
  transf.translate(-lastHalfLineWidth / 2., -lastHalfLineWidth / 2.);
  transf.translate(halfLineWidth / 2., halfLineWidth / 2.);
  QPolygonF transfPoly = transf.map(poly);
  //poly.translate(halfLineWidth * 2, halfLineWidth * 2);
  lastHalfLineWidth = halfLineWidth;
  return transfPoly;
}

bool te::layout::LineController::isGeometrySizeChange()
{
  bool result = false;

  QPolygonF poly = getQPolygon();
  QRectF polygonRect = poly.boundingRect();

  if (poly.empty())
  {
    return result;
  }

  const Property& pWidth = m_model->getProperty("width");
  const Property& pHeight = m_model->getProperty("height");

  double width = pWidth.getValue().toDouble();
  double height = pHeight.getValue().toDouble();

  double halfLineWidth = m_model->getProperty("line_width").getValue().toDouble();

  if ((width == polygonRect.width() + halfLineWidth) && (height == polygonRect.height() + halfLineWidth))
  {
    return result;
  }

  return true;
}

