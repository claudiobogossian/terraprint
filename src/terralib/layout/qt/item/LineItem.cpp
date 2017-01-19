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
  \file LineItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LineItem.h"
#include "../../item/LineModel.h"
#include "LineController.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/property/Property.h"
// STL
#include <vector>

// Qt
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QObject>

te::layout::LineItem::LineItem()
  : AbstractItem(nullptr)
{
    setFlag(QGraphicsItem::ItemClipsToShape);
}

te::layout::LineItem::~LineItem()
{

}

te::layout::AbstractItemModel* te::layout::LineItem::createModel() const
{
  return new LineModel();
}

te::layout::AbstractItemController* te::layout::LineItem::createController() const
{
  AbstractItemModel* model = createModel();
  return new LineController(model, (AbstractItemView*)this);
}

void te::layout::LineItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  LineController* controller = dynamic_cast<LineController*>(getController());
  if (!controller)
  {
    return;
  }

  QPolygonF poly = controller->getQPolygon();
  if (poly.empty())
  {
    return;
  }

  const Property& pColor = this->getProperty("color");
  const te::color::RGBAColor& color = te::layout::Property::GetValueAs<te::color::RGBAColor>(pColor);
  QColor qColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

  const Property& lineWidth = this->getProperty("line_width");
  double lnew = te::layout::Property::GetValueAs<double>(lineWidth);

  painter->save();
  QPen penStyle = searchStyle();
  QPen pn(qColor, lnew, penStyle.style());
  painter->setPen(pn);
  painter->setRenderHint(QPainter::Antialiasing, true);

  QPainterPath path;
  path.addPolygon(poly);

  //draws the item
  painter->drawPath(path);
  painter->restore();
}

QPen te::layout::LineItem::searchStyle()
{
  const Property& pLineStyle = this->getProperty("line_style_type");
  QPen penStyle;

  EnumLineStyleType lineStyle;

  const std::string& label = pLineStyle.getOptionByCurrentChoice().toString();
  EnumType* currentLineStyle = lineStyle.getEnum(label);

  if (currentLineStyle)
  {
    if (currentLineStyle == lineStyle.getStyleSolid())
    {
      penStyle.setStyle(Qt::SolidLine);
    }
    if (currentLineStyle == lineStyle.getStyleDash())
    {
      penStyle.setStyle(Qt::DashLine);
    }
    if (currentLineStyle == lineStyle.getStyleDot())
    {
      penStyle.setStyle(Qt::DotLine);
    }
    if (currentLineStyle == lineStyle.getStyleDashDot())
    {
      penStyle.setStyle(Qt::DashDotLine);
    }
    if (currentLineStyle == lineStyle.getStyleDashDotDot())
    {
      penStyle.setStyle(Qt::DashDotDotLine);
    }
    if (currentLineStyle == lineStyle.getStyleCustomDash())
    {
      penStyle.setStyle(Qt::CustomDashLine);
    }
  }

  return penStyle;
}

