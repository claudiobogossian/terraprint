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

// STL
#include <iostream>

te::layout::ScaleItem::ScaleItem(AbstractItemController* controller)
  : AbstractItem(controller)
  , m_scale(0)
  , m_gapX(0)
  , m_gapY(0)
  , m_scaleUnitGapX(0)
  , m_numberOfBreaks(0)
{  
  m_useResizePixmap = false;  // no use pixmap for resizing
  //The text size or length that exceeds the sides will be cut
  setFlag(QGraphicsItem::ItemClipsToShape);
}

te::layout::ScaleItem::~ScaleItem()
{
  
}

bool te::layout::ScaleItem::isLimitExceeded(QRectF resizeRect)
{
  bool result = true;

  if (!scene())
    return result;

  Scene* sc = dynamic_cast<Scene*>(scene());
  ItemUtils utils = sc->getItemUtils();

  std::string text = "A";
  QRectF textRect = utils.getMinimumTextBoundary(m_font.getFamily(), m_font.getPointSize(), text);

  double displacementBetweenScaleAndText = 2.;
  double height = (textRect.height() * 2) + (m_gapY * 2) + (displacementBetweenScaleAndText * 2); // because it has two sides: top and bottom
  double width = (textRect.width() * 2) + (m_gapX * 2) + (displacementBetweenScaleAndText * 2); // because it has two sides: left and right
  result = resizeRect.height() < height ? true : false;
  if (!result)
  {
    result = resizeRect.width() < width ? true : false;
  }
  return result;  
}

void te::layout::ScaleItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  const Property& property = m_controller->getProperty("scale_type");

  EnumScaleType enumScale;

  const std::string& label = property.getOptionByCurrentChoice().toString();
  EnumType* currentScaleType = enumScale.getEnum(label);

  if (validateGaps() == false)
  {
    return;
  }

  if (currentScaleType)
  {
    if (currentScaleType == enumScale.getDoubleAlternatingScaleBarType())
    {
      drawDoubleAlternatingScaleBar(painter);
    }
    if (currentScaleType == enumScale.getAlternatingScaleBarType())
    {
      drawAlternatingScaleBar(painter);
    }
    if (currentScaleType == enumScale.getHollowScaleBarType())
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
  painter->setRenderHint(QPainter::Antialiasing, true);
  
  std::string strCurrentUnit;
  double unit = controller->getCurrentUnit(strCurrentUnit);
  
  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;

  const Property& prop_only_first = m_controller->getProperty("only_first_and_last_value");
  bool only_first_and_last = te::layout::Property::GetValueAs<bool>(prop_only_first);

  const Property& prop_font_color = m_controller->getProperty("font_color");
  const te::color::RGBAColor& backgroundColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(prop_font_color);
  QColor textColor(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());

  const Property& lineWidth = m_controller->getProperty("line_width");
  double lnew = te::layout::Property::GetValueAs<double>(lineWidth);

  double displacementBetweenScaleAndText = 2.;
  
  QFont qFont = ItemUtils::convertToQfont(m_font);

  ItemUtils::ConfigurePainterForTexts(painter, m_font);

  QPointF unitCoord(0, 0);

  QPointF coordText;
  QRectF rectScale;
  QRectF newBoxSecond;
  
  std::stringstream ss_value;
  ss_value.precision(15);

  double initialGap = 0;
  double gap = controller->getGap(initialGap, m_font, m_numberOfBreaks, m_gapX, strCurrentUnit);
  x1 += initialGap;

  double firstTextWidth = 0;

  x1 += displacementBetweenScaleAndText;
  for(int i = 0; i <= m_numberOfBreaks; ++i, x1 += width)
  {
    if (width == 0)
      width = m_gapX;
    else
      value += m_scaleUnitGapX;

    ss_value.str(std::string()); // clear
    ss_value.clear();
    ss_value << value;

    const std::string& text = ss_value.str();
    QRectF textRect = utils.getMinimumTextBoundary(m_font.getFamily(), m_font.getPointSize(), text);

    if (value == 0)
    {
      firstTextWidth = textRect.width();
      //x1 += displacementBetweenScaleAndText + textRect.width();
    }

    QRectF newBoxFirst;
    
    if(i < m_numberOfBreaks)
    {
      painter->setPen(Qt::NoPen);

      //Down rect
      painter->setBrush(QBrush(secondRect));
      newBoxSecond = QRectF(x1, boundRect.bottomRight().y() - (m_gapY * 2) - 5, m_gapX, m_gapY);
      painter->drawRect(newBoxSecond);

      //Up rect
      painter->setBrush(QBrush(firstRect));
      newBoxFirst = QRectF(x1, boundRect.bottomRight().y() - m_gapY - 5, m_gapX, m_gapY);
      painter->drawRect(newBoxFirst);
    }

    coordText = QPointF(x1, newBoxSecond.topLeft().y() - textRect.height() - displacementBetweenScaleAndText);
    //rectScale = QRectF(displacementBetweenScaleAndText + firstTextWidth + initialGap + boundRect.x(), boundRect.bottomRight().y() - (m_gapY * 2) - 5,
//                         boundRect.x() + newBoxSecond.right() - initialGap - displacementBetweenScaleAndText - firstTextWidth, m_gapY * 2);
    rectScale = QRectF(displacementBetweenScaleAndText + initialGap + boundRect.x(), boundRect.bottomRight().y() - (m_gapY * 2) - 5,
                       boundRect.x() + newBoxSecond.right() - initialGap - displacementBetweenScaleAndText, m_gapY * 2);
      

    QPainterPath textObject = ItemUtils::textToVector(text.c_str(), qFont, coordText, 0);
    coordText.setX(coordText.rx() - (textObject.boundingRect().width() / 2));

    QPen penScale(black, lnew, Qt::SolidLine);
    painter->setPen(penScale);
    painter->setBrush(QBrush(textColor));
    
    if (only_first_and_last)
    {
      if (value == 0)
      {
        ItemUtils::drawText(coordText, painter, qFont, text);
      }
    }
    else
    {
      ItemUtils::drawText(coordText, painter, qFont, text);
    }

    unitCoord.setX(coordText.rx() + textObject.boundingRect().width() + 2.5);
    unitCoord.setY(coordText.ry() - 0.5);

    changeColor = firstRect;
    firstRect = secondRect;
    secondRect = changeColor;
  }

  if (only_first_and_last)
  {
    // last text
    ItemUtils::drawText(coordText, painter, qFont, ss_value.str());
  }

  //Rect around scale
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(rectScale);

  //middle-bottom text
  painter->setBrush(QBrush(textColor));
  ItemUtils::drawText(unitCoord, painter, qFont, strCurrentUnit);

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
  painter->setRenderHint(QPainter::Antialiasing, true);

  double displacementBetweenScaleAndText = 2.;
  
  std::string strCurrentUnit;
  double unit = controller->getCurrentUnit(strCurrentUnit);

  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  
  const Property& prop_only_first = m_controller->getProperty("only_first_and_last_value");
  bool only_first_and_last = te::layout::Property::GetValueAs<bool>(prop_only_first);

  const Property& prop_font_color = m_controller->getProperty("font_color");
  const te::color::RGBAColor& backgroundColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(prop_font_color);
  QColor textColor(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());

  const Property& lineWidth = m_controller->getProperty("line_width");
  double lnew = te::layout::Property::GetValueAs<double>(lineWidth);

  QFont qFont = ItemUtils::convertToQfont(m_font);

  ItemUtils::ConfigurePainterForTexts(painter, m_font);

  QPointF unitCoord(0, 0);

  QPointF coordText;
  QRectF rectScale;
  QRectF newBoxSecond;
  std::stringstream ss_value;
  ss_value.precision(15);

  double initialGap = 0;
  double gap = controller->getGap(initialGap, m_font);
  x1 += initialGap;

  double firstTextWidth = 0;

  for (int i = 0; i <= m_numberOfBreaks; ++i, x1 += width)
  {
    if (width == 0)
      width = m_gapX;
    else
      value += m_scaleUnitGapX;

    ss_value.str(std::string()); // clear
    ss_value.clear();
    ss_value << value;

    const std::string& text = ss_value.str();
    QRectF textRect = utils.getMinimumTextBoundary(m_font.getFamily(), m_font.getPointSize(), text);

    if (value == 0)
    {
      firstTextWidth = textRect.width();
      x1 += displacementBetweenScaleAndText + textRect.width();
    }

    QRectF newBoxFirst;

    if (i < m_numberOfBreaks)
    {
      painter->setPen(Qt::NoPen);
      painter->setBrush(QBrush(secondRect));
      newBoxSecond = QRectF(x1, boundRect.bottomRight().y() - m_gapY - 5, m_gapX, m_gapY);
      painter->drawRect(newBoxSecond);
    }

    coordText = QPointF(x1, newBoxSecond.topLeft().y() - textRect.height() - displacementBetweenScaleAndText);
    rectScale = QRectF(displacementBetweenScaleAndText + firstTextWidth + initialGap + boundRect.x(), boundRect.bottomRight().y() - m_gapY - 5,
      boundRect.x() + newBoxSecond.right() - initialGap - displacementBetweenScaleAndText - firstTextWidth, m_gapY);

    QPainterPath textObject = ItemUtils::textToVector(text.c_str(), qFont, coordText, 0);
    coordText.setX(coordText.rx() - (textObject.boundingRect().width() / 2));

    QPen penScale(black, lnew, Qt::SolidLine);
    painter->setPen(penScale);
    painter->setBrush(QBrush(textColor));

    if (only_first_and_last)
    {
      if (value == 0)
      {
        ItemUtils::drawText(coordText, painter, qFont, text);
      }
    }
    else
    {
      ItemUtils::drawText(coordText, painter, qFont, text);
    }

    unitCoord.setX(coordText.rx() + textObject.boundingRect().width() + 2.5);
    unitCoord.setY(coordText.ry() - 0.5);

    changeColor = firstRect;
    firstRect = secondRect;
    secondRect = changeColor;
  }

  if (only_first_and_last)
  {
    // last text
    ItemUtils::drawText(coordText, painter, qFont, ss_value.str());
  }

  //Rect around scale
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(rectScale);

  //middle-bottom text
  painter->setBrush(QBrush(textColor));
  ItemUtils::drawText(unitCoord, painter, qFont, strCurrentUnit);

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
  painter->setRenderHint(QPainter::Antialiasing, true);
  
  std::string strCurrentUnit;
  double unit = controller->getCurrentUnit(strCurrentUnit);
  
  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  
  const Property& prop_only_first = m_controller->getProperty("only_first_and_last_value");
  bool only_first_and_last = te::layout::Property::GetValueAs<bool>(prop_only_first);

  const Property& prop_font_color = m_controller->getProperty("font_color");
  const te::color::RGBAColor& backgroundColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(prop_font_color);
  QColor textColor(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());

  double displacementBetweenScaleAndText = 2.;

  const Property& lineWidth = m_controller->getProperty("line_width");
  double lnew = te::layout::Property::GetValueAs<double>(lineWidth);

  QFont qFont = ItemUtils::convertToQfont(m_font);

  ItemUtils::ConfigurePainterForTexts(painter, m_font);

  QPointF unitCoord(0, 0);

  QPointF coordText;
  QRectF rectScale;
  QLineF lineVrt;
  QLineF lineHrz;
  std::stringstream ss_value;
  ss_value.precision(15);

  double initialGap = 0;
  double gap = controller->getGap(initialGap, m_font);
  x1 += initialGap;

  double firstTextWidth = 0;

  for (int i = 0; i <= m_numberOfBreaks; ++i, x1 += width)
  {
    if (width == 0)
      width = m_gapX;
    else
      value += m_scaleUnitGapX;

    ss_value.str(std::string()); // clear
    ss_value.clear();
    ss_value << value;

    const std::string& text = ss_value.str();
    QRectF textRect = utils.getMinimumTextBoundary(m_font.getFamily(), m_font.getPointSize(), text);

    if (value == 0)
    {
      firstTextWidth = textRect.width();
      x1 += displacementBetweenScaleAndText + textRect.width();
    }

    rectScale = QRectF(displacementBetweenScaleAndText + firstTextWidth + initialGap + boundRect.x(), boundRect.bottomRight().y() - (m_gapY * 2) - 5,
      boundRect.x() + lineHrz.x2() - initialGap - displacementBetweenScaleAndText - firstTextWidth, m_gapY * 2);

    if (i < m_numberOfBreaks)
    {
      QPen penScale(black, lnew, Qt::SolidLine);
      penScale.setColor(firstRect);
      painter->setPen(penScale);
      painter->setBrush(Qt::NoBrush);

      //horizontal line
      lineHrz = QLineF(x1, rectScale.center().y(), x1 + m_gapX, rectScale.center().y());
      painter->drawLine(lineHrz);

      penScale.setColor(black);
      painter->setPen(penScale);
      //vertical line
      lineVrt = QLineF(x1, rectScale.topRight().y(), x1, rectScale.bottomRight().y());
      painter->drawLine(lineVrt);
    }

    coordText = QPointF(x1, lineVrt.y1() - textRect.height() - displacementBetweenScaleAndText);

    QPainterPath textObject = ItemUtils::textToVector(text.c_str(), qFont, coordText, 0);
    coordText.setX(coordText.rx() - (textObject.boundingRect().width() / 2));

    QPen penScale(black, lnew, Qt::SolidLine);
    painter->setPen(penScale);
    painter->setBrush(QBrush(textColor));
    
    if (only_first_and_last)
    {
      if (value == 0)
      {
        ItemUtils::drawText(coordText, painter, qFont, text);
      }
    }
    else
    {
      ItemUtils::drawText(coordText, painter, qFont, text);
    }

    unitCoord.setX(coordText.rx() + textObject.boundingRect().width() + 2.5);
    unitCoord.setY(coordText.ry() - 0.5);

    changeColor = firstRect;
    firstRect = secondRect;
    secondRect = changeColor;
  }

  if (only_first_and_last)
  {
    // last text
    ItemUtils::drawText(coordText, painter, qFont, ss_value.str());
  }

  //Rect around scale
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(rectScale);

  //middle-bottom text
  painter->setBrush(QBrush(textColor));
  ItemUtils::drawText(unitCoord, painter, qFont, strCurrentUnit);

  painter->restore();
}

void te::layout::ScaleItem::refreshScaleProperties()
{
  const Property& pScale = m_controller->getProperty("scale");
  const Property& pScaleGapX = m_controller->getProperty("scale_width_rect_gap");
  const Property& pScaleGapY = m_controller->getProperty("scale_height_rect_gap");
  const Property& pTextFont = m_controller->getProperty("font");
  const Property& pScaleUnitGapX = m_controller->getProperty("scale_in_unit_width_rect_gap");
  const Property& pNumberOfBreaks = m_controller->getProperty("number_of_breaks");

  m_scale = te::layout::Property::GetValueAs<double>(pScale);
  m_gapX = te::layout::Property::GetValueAs<double>(pScaleGapX);
  m_gapY = te::layout::Property::GetValueAs<double>(pScaleGapY);
  m_font = te::layout::Property::GetValueAs<Font>(pTextFont);
  m_scaleUnitGapX = te::layout::Property::GetValueAs<double>(pScaleUnitGapX);
  m_numberOfBreaks = te::layout::Property::GetValueAs<int>(pNumberOfBreaks);
}


bool te::layout::ScaleItem::validateGaps()
{

  const Property& pScaleGapX = m_controller->getProperty("scale_width_rect_gap");
  const Property& pScaleGapY = m_controller->getProperty("scale_height_rect_gap");
  const Property& pScaleUnitGapX = m_controller->getProperty("scale_in_unit_width_rect_gap");

  m_gapX = te::layout::Property::GetValueAs<double>(pScaleGapX);
  m_gapY = te::layout::Property::GetValueAs<double>(pScaleGapY);
  m_scaleUnitGapX = te::layout::Property::GetValueAs<double>(pScaleUnitGapX);

  QRectF boundRect = boundingRect();

  double result = boundRect.width() / m_gapX;

  if (result > 40)
  {
    return false;
  }

  if (m_gapX <= 0)
  {
    return false;
  }

  if (m_gapY <= 0)
  {
    return false;
  }


  if (m_scaleUnitGapX <= 0)
  {
    return false;
  }

  return true;

}

