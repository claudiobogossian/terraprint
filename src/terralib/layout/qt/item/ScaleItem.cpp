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
  \file LegendItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ScaleItem.h"
#include "terralib/color/RGBAColor.h"
#include "../../core/enum/EnumScaleType.h"
#include "../core/ItemUtils.h"

te::layout::ScaleItem::ScaleItem(AbstractItemController* controller, bool invertedMatrix)
  : AbstractItem<QGraphicsItem>(controller, invertedMatrix)
{  
}

te::layout::ScaleItem::~ScaleItem()
{

}

void te::layout::ScaleItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  const Property& property = m_controller->getProperty("scale_type");
  if(property.isNull() == false)
  {
    EnumScaleType enumScale;

    const std::string& label = property.getOptionByCurrentChoice().toString();
    EnumType* currentScaleType = enumScale.searchLabel(label);

    if(currentScaleType == enumScale.getDoubleAlternatingScaleBarType())
    {
      drawDoubleAlternatingScaleBar(painter);
    }
    if(currentScaleType == enumScale.getAlternatingScaleBarType())
    {
      drawAlternatingScaleBar(painter);
    }
    if(currentScaleType == enumScale.getHollowScaleBarType())
    {
      drawHollowScaleBar(painter);
    }
  }
}

double te::layout::ScaleItem::getUnit(std::string& strUnit)
{
  double unit = 1000.0;
  strUnit = "(" + m_controller->getProperty("Unit").getOptionByCurrentChoice().toString() + ")";

  if (strUnit == "(m)")
  {
    unit = 1.0;
  }
  return unit;
}

void te::layout::ScaleItem::drawDoubleAlternatingScaleBar( QPainter * painter )
{
  QRectF boundRect = boundingRect();
  
  painter->save();

  const Property& pScale = m_controller->getProperty("scale");
  const Property& pScaleGapX = m_controller->getProperty("scale_width_rect_gap");
  const Property& pScaleGapY = m_controller->getProperty("scale_height_rect_gap");
  
  double scale = pScale.getValue().toDouble();
  double gapX = pScaleGapX.getValue().toDouble();
  double gapY = pScaleGapY.getValue().toDouble();

  std::string strUnit;
  double unit = getUnit(strUnit);

  //convert millimeters to centimeters
  double mmToCm = gapX/10.;

  double spacing = scale/100.;
  
  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  QColor textColor(0, 0, 0, 255);

  QRectF newBoxFirst;
  QRectF newBoxSecond;

  std::string fontFamily("Arial");
  int fontSize = 10;

  Font txtFont = Font();
  txtFont.setFamily(fontFamily);
  txtFont.setPointSize(fontSize);

  QFont qFont = ItemUtils::convertToQfont(txtFont);

  ItemUtils::ConfigurePainterForTexts(painter, txtFont);

  QPointF unitCoord(0, 0);

  for( ; x1 <= boundRect.topRight().x(); x1 += width)
  {
    if(x1+gapX < boundRect.topRight().x())
    {
      painter->setPen(Qt::NoPen);

      //Down rect
      painter->setBrush(QBrush(secondRect));
      newBoxSecond = QRectF(x1, boundRect.center().y() - gapY, gapX, gapY);
      painter->drawRect(newBoxSecond);

      //Up rect
      painter->setBrush(QBrush(firstRect));
      newBoxFirst = QRectF(x1, boundRect.center().y(), gapX, gapY);
      painter->drawRect(newBoxFirst);
    }

    if(width == 0)
      width = gapX;
    else
      value += (spacing * mmToCm)/unit;

    std::stringstream ss_value;
    ss_value << value;

    std::string s_value = ss_value.str();

    painter->setPen(QPen(textColor));
    QPointF coordText(x1, newBoxSecond.topLeft().y() - 5);

    painter->setBrush(QBrush(textColor));

    const std::string& text = ss_value.str();

    QPainterPath textObject = ItemUtils::textToVector(text.c_str(), qFont, coordText, 0);
    coordText.setX(coordText.rx() - (textObject.boundingRect().width() / 2));
    
    drawText(coordText, painter, qFont, ss_value.str());


    unitCoord.setX(coordText.rx() + textObject.boundingRect().width() + 2.5);
    unitCoord.setY(coordText.ry() - 0.5);

    changeColor = firstRect;
    firstRect = secondRect;
    secondRect = changeColor;
  }
  
  QRectF rectScale = QRectF(boundRect.x(), boundRect.center().y() - gapY, boundRect.x() + newBoxSecond.right(), gapY*2);

  //Rect around scale
  QPen penBackground(black, 0, Qt::SolidLine);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(penBackground);
  painter->drawRect(rectScale);

  //middle-bottom text
  double centerX = rectScale.center().x();
  painter->setPen(QPen(textColor));
    
  QPointF coordText(centerX, boundRect.topLeft().y() + 1);


  painter->setBrush(QBrush(textColor));
  drawText(unitCoord, painter, qFont, strUnit);

  painter->restore();
}

void te::layout::ScaleItem::drawAlternatingScaleBar( QPainter * painter )
{
  QRectF boundRect = boundingRect();

  painter->save();

  const Property& pScale = m_controller->getProperty("scale");
  const Property& pScaleGapX = m_controller->getProperty("scale_width_rect_gap");
  const Property& pScaleGapY = m_controller->getProperty("scale_height_rect_gap");
  
  double scale = pScale.getValue().toDouble();
  double gapX = pScaleGapX.getValue().toDouble();
  double gapY = pScaleGapY.getValue().toDouble();

  std::string strUnit;
  double unit = getUnit(strUnit);

  //convert millimeters to centimeters
  double mmToCm = gapX/10;

  double spacing = scale/100;

  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  QColor textColor(0, 0, 0, 255);

  QRectF newBoxFirst;
  QRectF newBoxSecond;

  std::string fontFamily("Arial");

  int fontSize = 10;
  Font txtFont = Font();

  txtFont.setFamily(fontFamily);
  txtFont.setPointSize(fontSize);

  QFont qFont = ItemUtils::convertToQfont(txtFont);


  ItemUtils::ConfigurePainterForTexts(painter, txtFont);

  QPointF unitCoord(0, 0);

  for( ; x1 <= boundRect.topRight().x(); x1 += width)
  {
    if(x1+gapX < boundRect.topRight().x())
    {
      painter->setPen(Qt::NoPen);

      painter->setBrush(QBrush(secondRect));
      newBoxSecond = QRectF(x1, boundRect.center().y() - gapY/2, gapX, gapY);
      painter->drawRect(newBoxSecond);
    }

    if(width == 0)
      width = gapX;
    else
      value += (spacing * mmToCm)/unit;

    std::stringstream ss_value;
    ss_value << value;

    std::string s_value = ss_value.str();

    QPen pn(textColor);
    pn.setWidthF(0.5);
    painter->setPen(pn);
    QPointF coordText(x1, newBoxSecond.topLeft().y() - 5);
    
    const std::string& text = ss_value.str();

    QPainterPath textObject = ItemUtils::textToVector(text.c_str(), qFont, coordText, 0);
    coordText.setX(coordText.rx() - (textObject.boundingRect().width() / 2));

    painter->setBrush(QBrush(textColor));
    drawText(coordText, painter, qFont, ss_value.str());

    unitCoord.setX(coordText.rx() + textObject.boundingRect().width() + 2.5);
    unitCoord.setY(coordText.ry() - 0.5);

    changeColor = firstRect;
    firstRect = secondRect;
    secondRect = changeColor;


  }

  QRectF rectScale = QRectF(boundRect.x(), boundRect.center().y() - gapY/2, boundRect.x() + newBoxSecond.right(), gapY);

  //Rect around scale
  QPen penBackground(black, 0, Qt::SolidLine);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(penBackground);
  painter->drawRect(rectScale);

  //middle-bottom text
  double centerX = rectScale.center().x();  
  painter->setPen(QPen(textColor));
  
  QPointF coordText(centerX, boundRect.topLeft().y() + 1);
  painter->setBrush(QBrush(textColor));
  drawText(unitCoord, painter, qFont, strUnit);

  painter->restore();
}

void te::layout::ScaleItem::drawHollowScaleBar( QPainter * painter )
{
  QRectF boundRect = boundingRect();

  painter->save();

  const Property& pScale = m_controller->getProperty("scale");
  const Property& pScaleGapX = m_controller->getProperty("scale_width_rect_gap");
  const Property& pScaleGapY = m_controller->getProperty("scale_height_rect_gap");

  double scale = pScale.getValue().toDouble();
  double gapX = pScaleGapX.getValue().toDouble();
  double gapY = pScaleGapY.getValue().toDouble();

  std::string strUnit;
  double unit = getUnit(strUnit);

  //convert millimeters to centimeters
  double mmToCm = gapX / 10.;

  double spacing = scale / 100.;

  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor verticalLineColor = black;
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  QColor textColor(0, 0, 0, 255);

  QLineF lineHrz;
  QLineF lineVrt;

  std::string fontFamily("Arial");
  int fontSize = 10;

  Font txtFont = Font();
  
  txtFont.setFamily(fontFamily);
  txtFont.setPointSize(fontSize);

  QFont qFont = ItemUtils::convertToQfont(txtFont);

  ItemUtils::ConfigurePainterForTexts(painter, txtFont);

  QPen penScale(black, 0, Qt::SolidLine);
  painter->setPen(penScale);
  painter->setBrush(Qt::NoBrush);
  
  QPointF unitCoord(0, 0);

  for (; x1 <= boundRect.topRight().x(); x1 += width)
  {
    if (x1 + gapX < boundRect.topRight().x())
    {
      penScale.setColor(firstRect);
      painter->setPen(penScale);
      painter->setBrush(Qt::NoBrush);

      //horizontal line
      lineHrz = QLineF(x1, boundRect.center().y(), x1 + gapX, boundRect.center().y());
      painter->drawLine(lineHrz);

      penScale.setColor(verticalLineColor);
      painter->setPen(penScale);

      //vertical line
      lineVrt = QLineF(x1, boundRect.center().y() - gapY, x1, boundRect.center().y() + gapY);
      painter->drawLine(lineVrt);
    }

    if (width == 0)
      width = gapX;
    else
      value += (spacing * mmToCm) / unit;

    std::stringstream ss_value;
    ss_value << value;

    std::string s_value = ss_value.str();

    painter->setPen(penScale);
    QPointF coordText(x1, lineVrt.y1() - 5);

    const std::string& text = ss_value.str();
    QPainterPath textObject = ItemUtils::textToVector(text.c_str(), qFont, coordText, 0);
    coordText.setX(coordText.rx() - (textObject.boundingRect().width() / 2));

    painter->setBrush(QBrush(textColor));
    drawText(coordText, painter, qFont, ss_value.str());

    unitCoord.setX(coordText.rx() + textObject.boundingRect().width() + 2.5);
    unitCoord.setY(coordText.ry() - 0.5);

    changeColor = firstRect;
    firstRect = secondRect;
    secondRect = changeColor;
  }

  QRectF rectScale = QRectF(boundRect.x(), boundRect.center().y() - gapY, boundRect.x() + lineHrz.x2(), gapY * 2);

  //Rect around scale
  QPen penBackground(black, 0, Qt::SolidLine);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(penBackground);
  painter->drawRect(rectScale);

  //middle-bottom text
  double centerX = rectScale.center().x();
  painter->setPen(QPen(textColor));

  QPointF coordText(centerX, boundRect.topLeft().y() + 1);
  drawText(unitCoord, painter, qFont, strUnit);

  painter->restore();
}


