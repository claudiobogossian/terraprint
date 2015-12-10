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

double te::layout::ScaleItem::getGap(double& initialGap)
{
  std::string fontFamily("Arial");
  int fontSize = 10;
  Font txtFont = Font();
  txtFont.setFamily(fontFamily);
  txtFont.setPointSize(fontSize);
  QFont qFont = ItemUtils::convertToQfont(txtFont);

  std::string text = "0";
  QPainterPath firstTextObject = ItemUtils::textToVector(text.c_str(), qFont, QPointF(0, 0));
  initialGap = firstTextObject.boundingRect().width() / 2;

  std::string strUnit;
  double unit = getUnit(strUnit);
  QPainterPath unitTextObject = ItemUtils::textToVector(strUnit.c_str(), qFont, QPointF(0, 0));
  double unitGap = unitTextObject.boundingRect().width();

  const Property& pScale = m_controller->getProperty("scale");
  const Property& pScaleGapX = m_controller->getProperty("scale_width_rect_gap");

  double scale = pScale.getValue().toDouble();
  double gapX = pScaleGapX.getValue().toDouble();
  //convert millimeters to centimeters
  double mmToCm = gapX / 10.;

  double spacing = scale / 100.;

  double value = 0.;
  double width = 0.;
  QRectF boundRect = boundingRect();
  double x1 = boundRect.bottomLeft().x();
  double gap = unitGap + 2.5;
  for (; (x1 + gap) <= boundRect.topRight().x(); x1 += width)
  {
    if (width == 0)
      width = gapX;
    else
      value += (spacing * mmToCm) / unit;

    std::stringstream ss_value;
    ss_value << value;

    text = ss_value.str();
  }
  QPainterPath lastTextObject = ItemUtils::textToVector(text.c_str(), qFont, QPointF(0, 0));
  double finalGap = lastTextObject.boundingRect().width() / 2;
  gap = finalGap + unitGap + 2.5;
  return gap;
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
  double mmToCm = gapX / 10.;

  double spacing = scale / 100.;

  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  QColor textColor(0, 0, 0, 255);

  std::string fontFamily("Arial");
  int fontSize = 10;
  Font txtFont = Font();
  txtFont.setFamily(fontFamily);
  txtFont.setPointSize(fontSize);

  QFont qFont = ItemUtils::convertToQfont(txtFont);

  ItemUtils::ConfigurePainterForTexts(painter, txtFont);

  QPointF unitCoord(0, 0);

  QPointF coordText;
  QRectF rectScale;
  QRectF newBoxSecond;

  double initialGap;
  double gap = getGap(initialGap);
  x1 += initialGap;

  for (; (x1 + gap) <= boundRect.topRight().x(); x1 += width)
  {
    QRectF newBoxFirst;

    if ((x1 + gapX + gap) < boundRect.topRight().x())
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

    coordText = QPointF(x1, newBoxSecond.topLeft().y() - 5);
    rectScale = QRectF(initialGap + boundRect.x(), boundRect.center().y() - gapY, boundRect.x() + newBoxSecond.right() - initialGap, gapY * 2);

    if (width == 0)
      width = gapX;
    else
      value += (spacing * mmToCm) / unit;

    std::stringstream ss_value;
    ss_value << value;

    const std::string& text = ss_value.str();

    QPainterPath textObject = ItemUtils::textToVector(text.c_str(), qFont, coordText, 0);
    coordText.setX(coordText.rx() - (textObject.boundingRect().width() / 2));

    QPen penScale(black, 0, Qt::SolidLine);
    painter->setPen(penScale);
    painter->setBrush(QBrush(textColor));
    drawText(coordText, painter, qFont, ss_value.str());

    unitCoord.setX(coordText.rx() + textObject.boundingRect().width() + 2.5);
    unitCoord.setY(coordText.ry() - 0.5);

    changeColor = firstRect;
    firstRect = secondRect;
    secondRect = changeColor;
  }

  //Rect around scale
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(rectScale);

  //middle-bottom text
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
  double mmToCm = gapX / 10.;

  double spacing = scale / 100.;

  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  QColor textColor(0, 0, 0, 255);

  std::string fontFamily("Arial");
  int fontSize = 10;
  Font txtFont = Font();
  txtFont.setFamily(fontFamily);
  txtFont.setPointSize(fontSize);

  QFont qFont = ItemUtils::convertToQfont(txtFont);


  ItemUtils::ConfigurePainterForTexts(painter, txtFont);

  QPointF unitCoord(0, 0);

  QPointF coordText;
  QRectF rectScale;
  QRectF newBoxSecond;

  double initialGap;
  double gap = getGap(initialGap);
  x1 += initialGap;

  for (; (x1 + gap) <= boundRect.topRight().x(); x1 += width)
  {
    QRectF newBoxFirst;

    if ((x1 + gapX + gap) < boundRect.topRight().x())
    {
      painter->setPen(Qt::NoPen);

      painter->setBrush(QBrush(secondRect));
      newBoxSecond = QRectF(x1, boundRect.center().y() - gapY / 2, gapX, gapY);
      painter->drawRect(newBoxSecond);
    }

    coordText = QPointF(x1, newBoxSecond.topLeft().y() - 5);
    rectScale = QRectF(initialGap + boundRect.x(), boundRect.center().y() - gapY / 2, boundRect.x() + newBoxSecond.right() - initialGap, gapY);

    if (width == 0)
      width = gapX;
    else
      value += (spacing * mmToCm) / unit;

    std::stringstream ss_value;
    ss_value << value;

    const std::string& text = ss_value.str();

    QPainterPath textObject = ItemUtils::textToVector(text.c_str(), qFont, coordText, 0);
    coordText.setX(coordText.rx() - (textObject.boundingRect().width() / 2));

    QPen penScale(black, 0, Qt::SolidLine);
    painter->setPen(penScale);
    painter->setBrush(QBrush(textColor));
    drawText(coordText, painter, qFont, ss_value.str());

    unitCoord.setX(coordText.rx() + textObject.boundingRect().width() + 2.5);
    unitCoord.setY(coordText.ry() - 0.5);

    changeColor = firstRect;
    firstRect = secondRect;
    secondRect = changeColor;
  }

  //Rect around scale
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(rectScale);

  //middle-bottom text
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
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  QColor textColor(0, 0, 0, 255);

  std::string fontFamily("Arial");
  int fontSize = 10;
  Font txtFont = Font();
  txtFont.setFamily(fontFamily);
  txtFont.setPointSize(fontSize);

  QFont qFont = ItemUtils::convertToQfont(txtFont);

  ItemUtils::ConfigurePainterForTexts(painter, txtFont);

  QPointF unitCoord(0, 0);

  QPointF coordText;
  QRectF rectScale;
  QLineF lineVrt;
  QLineF lineHrz;

  double initialGap;
  double gap = getGap(initialGap);
  x1 += initialGap;

  for (; (x1 + gap) <= boundRect.topRight().x(); x1 += width)
  {
    if ((x1 + gapX + gap) < boundRect.topRight().x())
    {
      QPen penScale(black, 0, Qt::SolidLine);
      penScale.setColor(firstRect);
      painter->setPen(penScale);
      painter->setBrush(Qt::NoBrush);

      //horizontal line
      lineHrz = QLineF(x1, boundRect.center().y(), x1 + gapX, boundRect.center().y());
      painter->drawLine(lineHrz);

      penScale.setColor(black);
      painter->setPen(penScale);
      //vertical line
      lineVrt = QLineF(x1, boundRect.center().y() - gapY, x1, boundRect.center().y() + gapY);
      painter->drawLine(lineVrt);
    }

    coordText = QPointF(x1, lineVrt.y1() - 5);
    rectScale = QRectF(initialGap + boundRect.x(), boundRect.center().y() - gapY, boundRect.x() + lineHrz.x2() - initialGap, gapY * 2);

    if (width == 0)
      width = gapX;
    else
      value += (spacing * mmToCm) / unit;

    std::stringstream ss_value;
    ss_value << value;

    const std::string& text = ss_value.str();
    QPainterPath textObject = ItemUtils::textToVector(text.c_str(), qFont, coordText, 0);
    coordText.setX(coordText.rx() - (textObject.boundingRect().width() / 2));

    QPen penScale(black, 0, Qt::SolidLine);
    painter->setPen(penScale);
    painter->setBrush(QBrush(textColor));
    drawText(coordText, painter, qFont, ss_value.str());

    unitCoord.setX(coordText.rx() + textObject.boundingRect().width() + 2.5);
    unitCoord.setY(coordText.ry() - 0.5);

    changeColor = firstRect;
    firstRect = secondRect;
    secondRect = changeColor;
  }

  //Rect around scale
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(rectScale);

  //middle-bottom text
  painter->setBrush(QBrush(textColor));
  drawText(unitCoord, painter, qFont, strUnit);

  painter->restore();
}


