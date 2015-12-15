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
#include "../core/Scene.h"
#include "../item/ScaleController.h"

te::layout::ScaleItem::ScaleItem(AbstractItemController* controller, bool invertedMatrix)
  : AbstractItem<QGraphicsItem>(controller, invertedMatrix)
  , m_scale(0)
  , m_gapX(0)
  , m_gapY(0)
{  
  //The text size or length that exceeds the sides will be cut
  setFlag(QGraphicsItem::ItemClipsToShape);
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

void te::layout::ScaleItem::drawDoubleAlternatingScaleBar( QPainter * painter )
{
  if (!scene())
    return;

  Scene* sc = dynamic_cast<Scene*>(scene());
  ItemUtils utils = sc->getItemUtils();

  ScaleController* controller = dynamic_cast<ScaleController*>(m_controller);
  if (!controller)
    return;

  QRectF boundRect = boundingRect();

  painter->save();
  
  std::string strUnit;
  double unit = controller->getUnit(strUnit);

  //convert millimeters to centimeters
  double mmToCm = m_gapX / 10.;

  double spacing = m_scale / 100.;

  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  QColor textColor(0, 0, 0, 255);

  double displacementBetweenScaleAndText = 2.;
  
  QFont qFont = ItemUtils::convertToQfont(m_font);

  ItemUtils::ConfigurePainterForTexts(painter, m_font);

  QPointF unitCoord(0, 0);

  QPointF coordText;
  QRectF rectScale;
  QRectF newBoxSecond;

  double initialGap;
  double gap = controller->getGap(initialGap);
  x1 += initialGap;

  double firtTextWidth = 0;

  for (; (x1 + gap) <= boundRect.topRight().x(); x1 += width)
  {
    if (width == 0)
      width = m_gapX;
    else
      value += (spacing * mmToCm) / unit;

    std::stringstream ss_value;
    ss_value << value;

    const std::string& text = ss_value.str();
    QRectF textRect = utils.getMinimumTextBoundary(m_font.getFamily(), m_font.getPointSize(), text);

    if (value == 0)
    {
      firtTextWidth = textRect.width();
      x1 += displacementBetweenScaleAndText + textRect.width();
    }

    QRectF newBoxFirst;

    if ((x1 + m_gapX + gap) < boundRect.topRight().x())
    {
      painter->setPen(Qt::NoPen);

      //Down rect
      painter->setBrush(QBrush(secondRect));
      newBoxSecond = QRectF(x1, boundRect.center().y() - m_gapY, m_gapX, m_gapY);
      painter->drawRect(newBoxSecond);

      //Up rect
      painter->setBrush(QBrush(firstRect));
      newBoxFirst = QRectF(x1, boundRect.center().y(), m_gapX, m_gapY);
      painter->drawRect(newBoxFirst);
    }

    coordText = QPointF(x1, newBoxSecond.topLeft().y() - textRect.height() - displacementBetweenScaleAndText);
    rectScale = QRectF(displacementBetweenScaleAndText + firtTextWidth + initialGap + boundRect.x(), boundRect.center().y() - m_gapY, 
      boundRect.x() + newBoxSecond.right() - initialGap - displacementBetweenScaleAndText - firtTextWidth, m_gapY * 2);

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
  if (!scene())
    return;

  Scene* sc = dynamic_cast<Scene*>(scene());
  ItemUtils utils = sc->getItemUtils();

  ScaleController* controller = dynamic_cast<ScaleController*>(m_controller);
  if (!controller)
    return;
  
  QRectF boundRect = boundingRect();

  painter->save();

  double displacementBetweenScaleAndText = 2.;
  
  std::string strUnit;
  double unit = controller->getUnit(strUnit);

  //convert millimeters to centimeters
  double mmToCm = m_gapX / 10.;

  double spacing = m_scale / 100.;

  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  QColor textColor(0, 0, 0, 255);

  QFont qFont = ItemUtils::convertToQfont(m_font);

  ItemUtils::ConfigurePainterForTexts(painter, m_font);

  QPointF unitCoord(0, 0);

  QPointF coordText;
  QRectF rectScale;
  QRectF newBoxSecond;

  double initialGap;
  double gap = controller->getGap(initialGap);
  x1 += initialGap;

  double firtTextWidth = 0;

  for (; (x1 + gap) <= boundRect.topRight().x(); x1 += width)
  {
    if (width == 0)
      width = m_gapX;
    else
      value += (spacing * mmToCm) / unit;

    std::stringstream ss_value;
    ss_value << value;

    const std::string& text = ss_value.str();
    QRectF textRect = utils.getMinimumTextBoundary(m_font.getFamily(), m_font.getPointSize(), text);

    if (value == 0)
    {
      firtTextWidth = textRect.width();
      x1 += displacementBetweenScaleAndText + textRect.width();
    }

    QRectF newBoxFirst;

    if ((x1 + m_gapX + gap) < boundRect.topRight().x())
    {
      painter->setPen(Qt::NoPen);
      painter->setBrush(QBrush(secondRect));
      newBoxSecond = QRectF(x1, boundRect.center().y() - m_gapY / 2, m_gapX, m_gapY);
      painter->drawRect(newBoxSecond);
    }

    coordText = QPointF(x1, newBoxSecond.topLeft().y() - textRect.height() - displacementBetweenScaleAndText);
    rectScale = QRectF(displacementBetweenScaleAndText + firtTextWidth + initialGap + boundRect.x(), boundRect.center().y() - m_gapY / 2, 
      boundRect.x() + newBoxSecond.right() - initialGap - displacementBetweenScaleAndText - firtTextWidth, m_gapY);

    QPainterPath textObject = ItemUtils::textToVector(text.c_str(), qFont, coordText, 0);
    coordText.setX(coordText.rx() - (textObject.boundingRect().width() / 2));

    QPen penScale(black, 0, Qt::SolidLine);
    painter->setPen(penScale);
    painter->setBrush(QBrush(textColor));
    drawText(coordText, painter, qFont, text);

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
  if (!scene())
    return;

  Scene* sc = dynamic_cast<Scene*>(scene());
  ItemUtils utils = sc->getItemUtils();

  ScaleController* controller = dynamic_cast<ScaleController*>(m_controller);
  if (!controller)
    return;

  QRectF boundRect = boundingRect();

  painter->save();
  
  std::string strUnit;
  double unit = controller->getUnit(strUnit);

  //convert millimeters to centimeters
  double mmToCm = m_gapX / 10.;

  double spacing = m_scale / 100.;

  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  QColor textColor(0, 0, 0, 255);

  double displacementBetweenScaleAndText = 2.;

  QFont qFont = ItemUtils::convertToQfont(m_font);

  ItemUtils::ConfigurePainterForTexts(painter, m_font);

  QPointF unitCoord(0, 0);

  QPointF coordText;
  QRectF rectScale;
  QLineF lineVrt;
  QLineF lineHrz;

  double initialGap;
  double gap = controller->getGap(initialGap);
  x1 += initialGap;

  double firtTextWidth = 0;

  for (; (x1 + gap) <= boundRect.topRight().x(); x1 += width)
  {
    if (width == 0)
      width = m_gapX;
    else
      value += (spacing * mmToCm) / unit;

    std::stringstream ss_value;
    ss_value << value;

    const std::string& text = ss_value.str();
    QRectF textRect = utils.getMinimumTextBoundary(m_font.getFamily(), m_font.getPointSize(), text);

    if (value == 0)
    {
      firtTextWidth = textRect.width();
      x1 += displacementBetweenScaleAndText + textRect.width();
    }

    if ((x1 + m_gapX + gap) < boundRect.topRight().x())
    {
      QPen penScale(black, 0, Qt::SolidLine);
      penScale.setColor(firstRect);
      painter->setPen(penScale);
      painter->setBrush(Qt::NoBrush);

      //horizontal line
      lineHrz = QLineF(x1, boundRect.center().y(), x1 + m_gapX, boundRect.center().y());
      painter->drawLine(lineHrz);

      penScale.setColor(black);
      painter->setPen(penScale);
      //vertical line
      lineVrt = QLineF(x1, boundRect.center().y() - m_gapY, x1, boundRect.center().y() + m_gapY);
      painter->drawLine(lineVrt);
    }

    coordText = QPointF(x1, lineVrt.y1() - textRect.height() - displacementBetweenScaleAndText);
    rectScale = QRectF(displacementBetweenScaleAndText + firtTextWidth + initialGap + boundRect.x(), boundRect.center().y() - m_gapY, 
      boundRect.x() + lineHrz.x2() - initialGap - displacementBetweenScaleAndText - firtTextWidth, m_gapY * 2);

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

void te::layout::ScaleItem::refreshScaleProperties()
{
  const Property& pScale = m_controller->getProperty("scale");
  const Property& pScaleGapX = m_controller->getProperty("scale_width_rect_gap");
  const Property& pScaleGapY = m_controller->getProperty("scale_height_rect_gap");
  const Property& pTextFont = m_controller->getProperty("font");

  m_scale = pScale.getValue().toDouble();
  m_gapX = pScaleGapX.getValue().toDouble();
  m_gapY = pScaleGapY.getValue().toDouble();
  m_font = pTextFont.getValue().toFont();
}

