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
  \file EllipseItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EllipseItem.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../item/EllipseModel.h"
#include "terralib/color/RGBAColor.h"

te::layout::EllipseItem::EllipseItem()
: AbstractItem(nullptr)
{
}

te::layout::EllipseItem::~EllipseItem()
{

}

te::layout::AbstractItemModel* te::layout::EllipseItem::createModel() const
{
  return new EllipseModel();
}

void te::layout::EllipseItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  painter->save();

  const Property& pFillColor = this->getProperty("fill_color");
  const Property& pContourColor = this->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pFillColor);
  const te::color::RGBAColor& contourColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pContourColor);

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  const Property& lineWidth = this->getProperty("line_width");
  double lnew = te::layout::Property::GetValueAs<double>(lineWidth);

  QBrush brush(qFillColor);
  QPen pen(qContourColor, lnew, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);

  QPainterPath circle_path;
  circle_path.addEllipse(boundingRect());

  //draws the item
  painter->drawPath(circle_path);

  painter->restore();
}
