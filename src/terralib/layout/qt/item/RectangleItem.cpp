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
  \file RectangleItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "RectangleItem.h"
#include "../../item/RectangleModel.h"

#include "terralib/color/RGBAColor.h"
#include "../../core/enum/EnumRectangleType.h"

// Qt
#include <QColor>
#include <QPen>
#include <QRectF>

te::layout::RectangleItem::RectangleItem(te::layout::ItemInputProxy* itemInputProxy)
  : AbstractItem(itemInputProxy)
{

}

te::layout::RectangleItem::~RectangleItem()
{

}

te::layout::AbstractItemModel* te::layout::RectangleItem::createModel() const
{
  return new RectangleModel();
}

void te::layout::RectangleItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  const Property& property = this->getProperty("rectangle_type");
  EnumRectangleType enumRectangleType;

  const std::string& label = property.getOptionByCurrentChoice().toString();
  EnumType* currentRectangleType = enumRectangleType.getEnum(label);

  if (currentRectangleType)
  {
    if (currentRectangleType == enumRectangleType.getSimpleRectangleType())
    {
      drawRectangle(painter);
    }
    if (currentRectangleType == enumRectangleType.getRoundedRetangleType())
    {
      drawRoundedRectangle(painter);
    }

    if (currentRectangleType == enumRectangleType.getSingleCornerTrimmedRectangleType())
    {
      drawSingleCornerTrimmedRectangle(painter);
    }
  }
}

void te::layout::RectangleItem::drawRectangle( QPainter * painter )
{
  painter->save();

  const Property& pFillColor = this->getProperty("fill_color");
  const Property& pContourColor = this->getProperty("contour_color");
  const Property& pFrameThickness = this->getProperty("frame_thickness");

  const te::color::RGBAColor& fillColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pFillColor);
  const te::color::RGBAColor& contourColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pContourColor);
  double frameThickness = te::layout::Property::GetValueAs<double>(pFrameThickness);

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, frameThickness, Qt::SolidLine);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(pen);
  painter->setBrush(brush);

  //gets the adjusted boundigng rectangle based of the painter settings
  QRectF rectAdjusted = getAdjustedBoundingRect(painter);

  QPainterPath rect_path;
  rect_path.addRect(rectAdjusted);

  //draws the item
  painter->drawPath(rect_path);

  painter->restore();
}

void te::layout::RectangleItem::drawRoundedRectangle(QPainter * painter)
{
  painter->save();

  const Property& pFillColor = this->getProperty("fill_color");
  const Property& pContourColor = this->getProperty("contour_color");
  const Property& pFrameThickness = this->getProperty("frame_thickness");

  const te::color::RGBAColor& fillColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pFillColor);
  const te::color::RGBAColor& contourColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pContourColor);
  double frameThickness = te::layout::Property::GetValueAs<double>(pFrameThickness);

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, frameThickness, Qt::SolidLine);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(pen);
  painter->setBrush(brush);

  //gets the adjusted boundigng rectangle based of the painter settings
  QRectF rectAdjusted = getAdjustedBoundingRect(painter);

  QPainterPath rect_path;
  rect_path.addRoundRect(rectAdjusted,30,30);

  //draws the item
  painter->drawPath(rect_path);

  painter->restore();
}

void te::layout::RectangleItem::drawSingleCornerTrimmedRectangle(QPainter * painter)
{
  painter->save();

  const Property& pFillColor = this->getProperty("fill_color");
  const Property& pContourColor = this->getProperty("contour_color");
  const Property& pFrameThickness = this->getProperty("frame_thickness");

  const te::color::RGBAColor& fillColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pFillColor);
  const te::color::RGBAColor& contourColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pContourColor);
  double frameThickness = te::layout::Property::GetValueAs<double>(pFrameThickness);

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, frameThickness, Qt::SolidLine);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(pen);
  painter->setBrush(brush);

  //gets the adjusted boundigng rectangle based of the painter settings
  QRectF rectAdjusted = getAdjustedBoundingRect(painter);

  QPointF p1 = QPointF(rectAdjusted.width() - rectAdjusted.width() / 4., rectAdjusted.center().y()+ rectAdjusted.height() / 2.);
  QPointF p2 = QPointF(rectAdjusted.bottomRight().x() - rectAdjusted.bottomRight().x()/250.,rectAdjusted.height() - rectAdjusted.height() / 4.);
  QPointF p3 = QPointF(rectAdjusted.bottomRight().x(),rectAdjusted.top());
  QPointF p4 = QPointF(rectAdjusted.bottomLeft().x(),rectAdjusted.top());
  QPointF p5 = QPointF(rectAdjusted.bottomLeft().x(),rectAdjusted.bottom());
  QPolygonF rect;
  rect<<p1<<p2<<p3<<p4<<p5;

  //draws the item
  painter->drawPolygon(rect);

  painter->restore();
}
