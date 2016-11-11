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
  \file NorthItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "NorthItem.h"
#include "../../item/NorthModel.h"
#include "../../core/enum/EnumNorthArrowType.h"

#include "../../core/pattern/mvc/AbstractItemController.h"

te::layout::NorthItem::NorthItem(AbstractItemController* controller) 
  : AbstractItem(controller)
{

}

te::layout::NorthItem::~NorthItem()
{

}

void te::layout::NorthItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  const Property& property = m_controller->getProperty("northArrow_type");
  EnumNorthArrowType enumNorthArrowType;

  const std::string& label = property.getOptionByCurrentChoice().toString();
  EnumType* currentNorthArrowType = enumNorthArrowType.searchLabel(label);

  if(currentNorthArrowType == enumNorthArrowType.getNorthArrowType1())
  {
    drawNorthArrow1(painter);
  }
  else if (currentNorthArrowType == enumNorthArrowType.getNorthArrowType2())
  {
    drawNorthArrow2(painter);
  }
  else if(currentNorthArrowType == enumNorthArrowType.getNorthArrowType3())
  {
    drawNorthArrow3(painter);
  }
}

void te::layout::NorthItem::drawNorthArrow1(QPainter * painter)
{
  QRectF boundingBoxMM = this->boundingRect();
  double fontNQ = boundingBoxMM.height() / 2.;
  QFont font("Arial");
  font.setPointSizeF(fontNQ);

  QPainterPath northLetter = ItemUtils::textToVector("N", font);

  double northLetterX = boundingBoxMM.center().x() - (northLetter.boundingRect().width() / 2.);
  double northLetterY = boundingBoxMM.center().y() + boundingBoxMM.height() / 3.;
  northLetter.translate(northLetterX, northLetterY);

  const Property& lineWidth = m_controller->getProperty("line_width");
  double lnew = te::layout::Property::GetValueAs<double>(lineWidth);

  painter->save();
  painter->setRenderHint(QPainter::Antialiasing, true);

  QColor cpen = setBrush(painter);
  QPen pn(cpen, lnew, Qt::SolidLine);
  painter->setPen(pn);
  QPointF p1 = QPointF(boundingRect().width() / 2.,boundingRect().center().y()+boundingRect().height()/4.);
  QPointF p2 = QPointF(boundingRect().bottomRight().x()-boundingRect().bottomRight().x()/4,boundingRect().top());
  QPointF p3 = QPointF(boundingRect().width() / 2.,boundingRect().center().y()-boundingRect().height()/4.);
  QPointF p4 = QPointF(boundingRect().bottomLeft().x()+boundingRect().bottomRight().x()/4,boundingRect().top());
  QPolygonF north;
  north<<p1<<p2<<p3<<p4;

  painter->setPen(pn);
  painter->drawPolygon(north);
  painter->fillPath(northLetter, QBrush(cpen));
  painter->restore();
}
void te::layout::NorthItem::drawNorthArrow2(QPainter * painter)
{
  QRectF boundingBoxMM = this->boundingRect();
  double fontNQ = boundingBoxMM.height() / 2.;
  QFont font("Arial");
  font.setPointSizeF(fontNQ);

  QPainterPath northLetter = ItemUtils::textToVector("N", font);

  double northLetterX = boundingBoxMM.center().x() - (northLetter.boundingRect().width() / 2.);
  double northLetterY = boundingBoxMM.center().y() + boundingBoxMM.height() / 3.;
  northLetter.translate(northLetterX, northLetterY);

  const Property& lineWidth = m_controller->getProperty("line_width");
  double lnew = te::layout::Property::GetValueAs<double>(lineWidth);

  painter->save();
  painter->setRenderHint(QPainter::Antialiasing, true);
  QColor cpen = setBrush(painter);
  QPen pn(cpen, lnew, Qt::SolidLine);
  
  QColor secondPolygon(255, 255, 255, 255);

  QPointF p1 = QPointF(boundingRect().width() / 2.,boundingRect().center().y()+boundingRect().height()/4.);
  QPointF p2 = QPointF(boundingRect().bottomRight().x()-boundingRect().bottomRight().x()/4,boundingRect().top());
  QPointF p3 = QPointF(boundingRect().width() / 2.,boundingRect().center().y()-boundingRect().height()/4.);
  QPointF p4 = QPointF(boundingRect().bottomLeft().x()+boundingRect().bottomRight().x()/4,boundingRect().top());

  QPolygonF north2;
  north2<<p1<<p2<<p3;
  QPolygonF north3;
  north3<<p1<<p4<<p3;

  painter->setPen(pn);
  painter->drawPolygon(north2);
  painter->setBrush(QBrush(secondPolygon));
  painter->drawPolygon(north3);

  painter->fillPath(northLetter, QBrush(cpen));
  
  painter->restore();
}

void te::layout::NorthItem::drawNorthArrow3(QPainter * painter)
{
  QRectF boundingBoxMM = this->boundingRect();
  double fontNQ = boundingBoxMM.height() / 2.;
  QFont font("Arial");
  font.setPointSizeF(fontNQ);

  QPainterPath northLetter = ItemUtils::textToVector("N", font);

  double northLetterX = boundingBoxMM.center().x() - (northLetter.boundingRect().width() / 2.);
  double northLetterY = boundingBoxMM.center().y() + boundingBoxMM.height() / 3.;
  northLetter.translate(northLetterX, northLetterY);

  const Property& lineWidth = m_controller->getProperty("line_width");
  double lnew = te::layout::Property::GetValueAs<double>(lineWidth);

  painter->save();
  painter->setRenderHint(QPainter::Antialiasing, true);
  QColor cpen = setBrush(painter);
  QPen pn(cpen, lnew, Qt::SolidLine);
  painter->setPen(pn);

  double symbolCenterX = (boundingRect().width() / 2.);
  double symbolCenterY = (boundingRect().height() / 2.) - (boundingRect().height() / 10.); //we translate the reference a little bit to fit the North letter

  QPointF p5 = QPointF(symbolCenterX, symbolCenterY + boundingRect().height()/2.7);
  QPointF p6 = QPointF(symbolCenterX, symbolCenterY - boundingRect().height()/2.5);
  QPointF p7 = QPointF(symbolCenterX +boundingRect().width()/2.5, symbolCenterY);
  QPointF p8 = QPointF(symbolCenterX -boundingRect().width()/2.5, symbolCenterY);
  QPointF p9 = QPointF(symbolCenterX +boundingRect().width()/40, symbolCenterY);
  QPointF p10 = QPointF(symbolCenterX -boundingRect().width()/40, symbolCenterY);
  QPointF p11 = QPointF(symbolCenterX, symbolCenterY + boundingRect().height()/40);
  QPointF p12 = QPointF(symbolCenterX, symbolCenterY - boundingRect().height()/40);

  QPolygonF north4;
  north4<<p5<<p9<<p10;
  north4<<p6<<p9<<p10;

  QPolygonF north5;
  north5<<p8<<p11<<p12;
  north5<<p7<<p11<<p12;

  painter->setPen(pn);
  painter->drawPolygon(north4);
  painter->drawPolygon(north5);

  painter->fillPath(northLetter, QBrush(cpen));
  painter->restore();
}

QColor te::layout::NorthItem::setBrush(QPainter* painter)
{
  const Property& colorProperty = m_controller->getProperty("color");
  const te::color::RGBAColor& color = te::layout::Property::GetValueAs<te::color::RGBAColor>(colorProperty);
  QColor brushColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
  painter->setBrush(QBrush(brushColor));
  return brushColor;
}
