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
  \file BalloonItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BalloonItem.h"
#include "TextItem.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/enum/EnumBalloonDirectionType.h"
#include "../../core/enum/EnumBalloonType.h"

// STL
#include <algorithm>    // std::max and std::min
#include <cmath>

te::layout::BalloonItem::BalloonItem(AbstractItemController* controller)
: TextItem(controller)
{

}

te::layout::BalloonItem::~BalloonItem()
{

}

QRectF te::layout::BalloonItem::boundingRect() const
{
 return TextItem::boundingRect();
}

void te::layout::BalloonItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  if (m_isEditionMode == true)
  {
    TextItem::drawItem(painter, option, widget);
    return;
  }


  const Property& pBalloonType = m_controller->getProperty("balloon_type");

  EnumBalloonType balloonEnum;

  const std::string& label = pBalloonType.getOptionByCurrentChoice().toString();
  EnumType* currentBalloonType = balloonEnum.searchLabel(label);


  if (currentBalloonType == balloonEnum.getRectangleBalloon())
  {
    drawRectangleBalloon(painter);
  }

  if (currentBalloonType == balloonEnum.getRoundedRectangleBalloon())
  {
    drawRoundedRectangleBalloon(painter);
  }


  if (currentBalloonType == balloonEnum.getEllipseBalloon())
  {
    drawEllipseBalloon(painter);
  }


  TextItem::drawItem(painter, option, widget);
}

void te::layout::BalloonItem::drawRectangleBalloon(QPainter * painter)
{
  const Property& pMarginSize = m_controller->getProperty("margin_size");
  double margin = te::layout::Property::GetValueAs<double>(pMarginSize);
 
  painter->save();
  setPainterParameters(painter);

  QRectF adjustedBoundbox = getAdjustedBoundingRect(painter);

  QPointF p1 = QPointF(adjustedBoundbox.x(), (margin * 2));
  QPointF p2 = QPointF(adjustedBoundbox.bottomLeft().x(), adjustedBoundbox.bottomLeft().y());
  QPointF p3 = QPointF(adjustedBoundbox.bottomRight().x(), adjustedBoundbox.bottomRight().y());
  QPointF p4 = QPointF(adjustedBoundbox.bottomRight().x(), (margin * 2));
  QPointF p5 = QPointF(adjustedBoundbox.bottomRight().x() - (adjustedBoundbox.width() * 0.05), (margin * 2));
  QPointF p6 = QPointF(adjustedBoundbox.bottomRight().x() - (adjustedBoundbox.width() * 0.05), adjustedBoundbox.topRight().y());
  QPointF p7 = QPointF(adjustedBoundbox.bottomRight().x() - margin - (adjustedBoundbox.width() * 0.05), (margin * 2));

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  QPainterPath qPath;
  
  qPath.moveTo(p1);
  qPath.lineTo(p1);
  qPath.lineTo(p2);
  qPath.lineTo(p3);
  qPath.lineTo(p4);
  qPath.lineTo(p5);
  qPath.lineTo(p6);
  qPath.lineTo(p7);
  qPath.lineTo(p1);

  setBalloonDirection(qPath);

  painter->drawPath(qPath);
  painter->restore();
}

void te::layout::BalloonItem::drawRoundedRectangleBalloon(QPainter * painter)
{
  const Property& pMarginSize = m_controller->getProperty("margin_size");
  double margin = te::layout::Property::GetValueAs<double>(pMarginSize);

  painter->save();
  setPainterParameters(painter);

  QRectF adjustedBoundbox = getAdjustedBoundingRect(painter);

  QPainterPath qPath;

  QPointF bLeft = adjustedBoundbox.bottomLeft();
  QPointF tRight = adjustedBoundbox.topRight();

  tRight.setY(tRight.y() + (margin * 2));

  QRectF fullRect(bLeft, tRight);

  QRectF leftTopRect(fullRect.topLeft().x(), fullRect.topLeft().y() - margin, margin, margin);
  QRectF leftBottomRect(fullRect.bottomLeft().x(), fullRect.bottomLeft().y(), margin, margin);
  QRectF rightTopRect(fullRect.topRight().x() - margin, fullRect.topRight().y() - margin, margin, margin);
  QRectF rightBottomRect(fullRect.bottomRight().x() - margin, fullRect.bottomRight().y(), margin, margin);


  qPath.moveTo(leftBottomRect.topRight());
  qPath.quadTo(leftBottomRect.topLeft(), leftBottomRect.bottomLeft());
  qPath.lineTo(leftTopRect.topLeft());
  qPath.moveTo(leftTopRect.topLeft());
  qPath.quadTo(leftTopRect.bottomLeft(), leftTopRect.bottomRight());
  qPath.lineTo(rightTopRect.bottomLeft());
  qPath.quadTo(rightTopRect.bottomRight(), rightTopRect.topRight());
  qPath.lineTo(rightBottomRect.bottomRight());
  qPath.quadTo(rightBottomRect.topRight(), rightBottomRect.topLeft());
  qPath.lineTo(rightBottomRect.topLeft().x(), rightBottomRect.topLeft().y() - (margin * 2));
  qPath.lineTo(rightBottomRect.topLeft().x() - margin, rightBottomRect.topLeft().y());
  qPath.lineTo(leftBottomRect.topRight());


  setBalloonDirection(qPath);

  painter->drawPath(qPath);
  painter->restore();
  
}


void te::layout::BalloonItem::setPainterParameters(QPainter* painter)
{
  const Property& colorProperty = m_controller->getProperty("color");
  const te::color::RGBAColor& color = te::layout::Property::GetValueAs<te::color::RGBAColor> (colorProperty);

  const Property& pFillColor = m_controller->getProperty("fill_color");
  const Property& pContourColor = m_controller->getProperty("contour_color");

  const Property& pContourThickness = m_controller->getProperty("contour_thickness");
  double contourThickness = te::layout::Property::GetValueAs<double>(pContourThickness);

  const te::color::RGBAColor& fillColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pFillColor);
  const te::color::RGBAColor& contourColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pContourColor);

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());
  
  QColor brushColor(qFillColor);
  QPen pen(qContourColor, Qt::SolidLine);
  pen.setWidthF(contourThickness);

  painter->setPen(pen);

  painter->setBrush(QBrush(brushColor));
  painter->setRenderHint(QPainter::Antialiasing, true);
}

void te::layout::BalloonItem::drawEllipseBalloon(QPainter * painter)
{
  const Property& pMarginSize = m_controller->getProperty("margin_size");
  double margin = te::layout::Property::GetValueAs<double>(pMarginSize);

  painter->save();
  setPainterParameters(painter);

  QRectF adjustedBoundbox = getAdjustedBoundingRect(painter);


  QPointF p2 = QPointF(adjustedBoundbox.x(), adjustedBoundbox.y() + adjustedBoundbox.height());
  QPointF p3 = QPointF(adjustedBoundbox.x() + adjustedBoundbox.width(), adjustedBoundbox.y() + (margin * 2));

  QPainterPath qPath;
  
  QRectF fullRect(p2, p3);

  qPath.arcMoveTo(fullRect, 280);
  qPath.arcTo(fullRect, 280, -340);
  qPath.lineTo(qPath.currentPosition().x(), adjustedBoundbox.y());
  qPath.arcTo(fullRect, 280, 0);

  setBalloonDirection(qPath);

  painter->drawPath(qPath);
  painter->restore();
}

void  te::layout::BalloonItem::setBalloonDirection(QPainterPath& qpainterpath)
{
  const Property& pWidth = m_controller->getProperty("width");
  double width = te::layout::Property::GetValueAs<double>(pWidth);

  EnumBalloonDirectionType balloonDirection;
  const Property& pDirection = m_controller->getProperty("balloon_direction");
  const std::string& label = pDirection.getOptionByCurrentChoice().toString();
  EnumType* currentBalloonDirectionType = balloonDirection.searchLabel(label);

  if (currentBalloonDirectionType == balloonDirection.getLeft())
  {

    QTransform transform;
    transform.scale(-1, 1);
    transform.translate(-width, 0);
    qpainterpath = transform.map(qpainterpath);
  }
}
