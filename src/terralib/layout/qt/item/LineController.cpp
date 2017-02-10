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

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
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

te::layout::LineController::LineController(te::layout::AbstractItemModel* model, AbstractItemView* view)
  : AbstractItemController(model, view)
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

  te::gm::Geometry* line = getGeometry();;

  if (line->getNPoints() <= 0)
  {
    return;
  }

  //update with new points
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  Property property;
  property.setName("geometry");
  property.setValue<te::gm::Geometry*>(line, dataType->getDataTypeGeometry());
  this->setProperty(property);
}

te::gm::Geometry* te::layout::LineController::getGeometry()
{
  QPolygonF poly = recalculatePolygon();
  
  int polySize = poly.size();

  te::gm::LineString* lineString = new te::gm::LineString(polySize, te::gm::LineStringType);

  if (poly.empty())
  {
    return lineString;
  }

  double x = 0;
  double y = 0;
  
  unsigned int count = 0;

  foreach(QPointF pt, poly)
  {
    lineString->setPointN(count, te::gm::Point(pt.x(), pt.y()));
    ++count;
  }

  return lineString;
}

QPolygonF te::layout::LineController::getQPolygon()
{
  QPolygonF poly;

  const Property& pLine = m_model->getProperty("geometry");
  const te::gm::Geometry* geometryPtr = te::layout::Property::GetValueAs<te::gm::Geometry*>(pLine);

  te::gm::LineString const* lineString = dynamic_cast< te::gm::LineString const * > (geometryPtr);
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

  if (poly.empty() || !polygonRect.isValid())
  {
    return poly;
  }

  const Property& pWidth = m_model->getProperty("width");
  const Property& pHeight = m_model->getProperty("height");

  double width = te::layout::Property::GetValueAs<double>(pWidth);
  double height = te::layout::Property::GetValueAs<double>(pHeight);

  if ((width == polygonRect.width()) && (height == polygonRect.height()))
  {
    return poly;
  }

  double xScale = width / polygonRect.width();
  double yScale = height / polygonRect.height();

  QTransform transf;
  transf = transf.scale(xScale, yScale);
  QPolygonF transfPoly = transf.map(poly);

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

  double width = te::layout::Property::GetValueAs<double>(pWidth);
  double height = te::layout::Property::GetValueAs<double>(pHeight);

  if ((width == polygonRect.width()) && (height == polygonRect.height()))
  {
    return result;
  }

  return true;
}

void te::layout::LineController::setProperties(const te::layout::Properties& properties)
{
  Properties propertiesCopy = properties; 
    
  if (propertiesCopy.contains("geometry"))
  {
    const Property& property = propertiesCopy.getProperty("geometry");

    te::gm::Geometry* geometryPtr = te::layout::Property::GetValueAs<te::gm::Geometry*>(property);
    const te::gm::Envelope* env = geometryPtr->getMBR();
    double width = env->getWidth();
    double height = env->getHeight();
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    {
      Property property(0);
      property.setName("width");
      property.setValue(width, dataType->getDataTypeDouble());
      propertiesCopy.addProperty(property);
    }
    {
      Property property(0);
      property.setName("height");
      property.setValue(height, dataType->getDataTypeDouble());
      propertiesCopy.addProperty(property);
    }
    
  }
  AbstractItemController::setProperties(propertiesCopy);
}
