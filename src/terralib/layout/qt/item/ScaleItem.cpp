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
#include "../../item/ScaleModel.h"
#include "ScaleController.h"

// STL
#include <iostream>

te::layout::ScaleItem::ScaleItem(te::layout::ItemInputProxy* itemInputProxy)
  : AbstractItem(itemInputProxy)
  , m_scale(0)
  , m_gapX(0)
  , m_gapY(0)
  , m_scaleUnitGapX(0)
  , m_numberOfBreaks(0)
{  
  m_useResizePixmap = false;
  setResize(false); // Disable all sides of resize
  m_isResizeTPRight = true;
  //The text size or length that exceeds the sides will be cut
  setFlag(QGraphicsItem::ItemClipsToShape);
}

te::layout::ScaleItem::~ScaleItem()
{
  
}

te::layout::AbstractItemModel* te::layout::ScaleItem::createModel() const
{
  return new ScaleModel();
}

te::layout::AbstractItemController* te::layout::ScaleItem::createController() const
{
  AbstractItemModel* model = createModel();
  return new ScaleController(model, (AbstractItemView*)this);
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
  if (validateGaps() == false)
  {
    return;
  }

  const Property& property = this->getProperty("scale_type");
  const std::string& label = property.getOptionByCurrentChoice().toString();

  EnumScaleType enumScale;
  EnumType* currentScaleType = enumScale.getEnum(label);
  if (currentScaleType == 0)
  {
    return;
  }

  const Property& pDisplacement = getProperty("displacementBetweenScaleAndText");
  const Property& pBreakWidth = getProperty("scale_width_rect_gap");
  const Property& pBreakHeight = getProperty("scale_height_rect_gap");
  const Property& pDrawTextsBelowTheBar = getProperty("draw_texts_below_the_bar");

  double displacementBetweenScaleAndText = Property::GetValueAs<double>(pDisplacement);
  double breakWidth = Property::GetValueAs<double>(pBreakWidth);
  double breakHeight = Property::GetValueAs<double>(pBreakHeight);
  bool drawTextsBelowTheBar = Property::GetValueAs<bool>(pDrawTextsBelowTheBar);

  double breakInitialX = displacementBetweenScaleAndText;
  double breakInitialY = 2 + breakHeight + displacementBetweenScaleAndText;

  double textInitialX = displacementBetweenScaleAndText;
  double textInitialY = 2;

  //if the texts are not set to be drawn below the bar, we put it above
  if (drawTextsBelowTheBar == false)
  {
    breakInitialY = 2;
    textInitialY = 2 + breakHeight + displacementBetweenScaleAndText;

    if (currentScaleType == enumScale.getDoubleAlternatingScaleBarType())
    {
      //this bar has the double of the height of the other bars
      textInitialY += breakHeight;
    }
  }

  drawBreaks(painter, m_numberOfBreaks, breakInitialX, breakInitialY, breakWidth, breakHeight);
  drawTexts(painter, m_numberOfBreaks, textInitialX, textInitialY, breakWidth, breakHeight);
}

void te::layout::ScaleItem::drawBreaks(QPainter * painter, int numberOfBreaks, double initialX, double initialY, double width, double height)
{
  const Property& property = this->getProperty("scale_type");
  const std::string& label = property.getOptionByCurrentChoice().toString();

  EnumScaleType enumScale;
  EnumType* currentScaleType = enumScale.getEnum(label);
  if (currentScaleType == 0)
  {
    return;
  }

  const Property& pLineWidth = this->getProperty("line_width");
  double lineWidth = te::layout::Property::GetValueAs<double>(pLineWidth);

  painter->save();

  QRectF breaksRect;

  for (int i = 0; i < numberOfBreaks; ++i)
  {
    double currentX = initialX + (i * width);
    double currentY = initialY;

    QColor firstRectColor(Qt::black);
    QColor secondRectColor(Qt::white);
    if (i % 2 != 0)
    {
      firstRectColor = Qt::white;
      secondRectColor = Qt::black;
    }

    if (currentScaleType == enumScale.getDoubleAlternatingScaleBarType())
    {
      painter->setPen(Qt::NoPen);

      //Lower break
      painter->setBrush(QBrush(secondRectColor));
      QRectF lowerBreakRect(currentX, currentY, width, height);
      painter->drawRect(lowerBreakRect);

      //Upper break
      painter->setBrush(QBrush(firstRectColor));
      QRectF upperBreakRect(currentX, currentY + height, width, height);
      painter->drawRect(upperBreakRect);

      breaksRect = breaksRect.unite(lowerBreakRect);
      breaksRect = breaksRect.unite(upperBreakRect);
    }
    else if (currentScaleType == enumScale.getAlternatingScaleBarType())
    {
      painter->setPen(Qt::NoPen);
      painter->setBrush(QBrush(firstRectColor));

      QRectF breakRect(currentX, currentY, width, height);
      painter->drawRect(breakRect);

      breaksRect = breaksRect.unite(breakRect);
    }
    else if (currentScaleType == enumScale.getHollowScaleBarType())
    {
      QRectF breakRect(currentX, currentY, width, height);

      QPen penScale(Qt::black, lineWidth, Qt::SolidLine);
      painter->setPen(penScale);
      painter->setBrush(Qt::NoBrush);

      //horizontal line
      if (i % 2 == 0)
      {
        QLineF horizontalLine(currentX, breakRect.center().y(), currentX + width, breakRect.center().y());
        painter->drawLine(horizontalLine);
      }

      //vertical line
      QLineF verticalLine(currentX, breakRect.topRight().y(), currentX, breakRect.bottomRight().y());
      painter->drawLine(verticalLine);

      breaksRect = breaksRect.unite(breakRect);
    }
  }

  // Draws a rect surrouding the breaks
  QPen pen(Qt::black, lineWidth);
  painter->setPen(pen);
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(breaksRect);

  painter->restore();
}

void te::layout::ScaleItem::drawTexts(QPainter * painter, int numberOfBreaks, double initialX, double initialY, double width, double height)
{
  ScaleController* controller = dynamic_cast<ScaleController*>(getController());
  if (controller == 0)
    return;


  const Property& pLineWidth = this->getProperty("line_width");
  double lineWidth = te::layout::Property::GetValueAs<double>(pLineWidth);

  const Property& prop_font_color = this->getProperty("font_color");
  const te::color::RGBAColor& backgroundColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(prop_font_color);
  QColor textColor(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());

  const Property& prop_only_first = this->getProperty("only_first_and_last_value");
  bool only_first_and_last = te::layout::Property::GetValueAs<bool>(prop_only_first);

  std::string strCurrentUnit;
  double unit = controller->getCurrentUnit(strCurrentUnit);

  QFont qFont = ItemUtils::convertToQfont(m_font);

  painter->save();

  QPen penScale(Qt::black, lineWidth, Qt::SolidLine);
  painter->setPen(penScale);
  painter->setBrush(QBrush(textColor));

  ItemUtils::ConfigurePainterForTexts(painter, m_font);

  for (int i = 0; i <= numberOfBreaks; ++i)
  {
    if (only_first_and_last == true && i != 0 && i != numberOfBreaks)
    {
      continue;
    }

    double currentX = initialX + (i * width);
    double currentY = initialY;
    double currentScaleValue = i * m_scaleUnitGapX;

    //formatting the number and converting it to string
    std::stringstream ss_value;
    ss_value.precision(15);
    ss_value << currentScaleValue;
    std::string text = ss_value.str();

    //centering the text in the current position
    QPainterPath textObject = ItemUtils::textToVector(text.c_str(), qFont);
    double xAdjust = textObject.boundingRect().width() / 2.;

    currentX -= xAdjust;

    //if we are in the last text, we must also draw the unit
    //we ajust only the number to the center of the break end
    //but we draw the number and the text related to the current unit
    if (i == numberOfBreaks)
    {
      //is the last
      text += " " + strCurrentUnit;
    }

    QPointF textPosition(currentX, currentY);
    ItemUtils::drawText(textPosition, painter, qFont, text);
  }

  painter->restore();
}

void te::layout::ScaleItem::refreshScaleProperties()
{
  const Property& pScale = this->getProperty("scale");
  const Property& pScaleGapX = this->getProperty("scale_width_rect_gap");
  const Property& pScaleGapY = this->getProperty("scale_height_rect_gap");
  const Property& pTextFont = this->getProperty("font");
  const Property& pScaleUnitGapX = this->getProperty("scale_in_unit_width_rect_gap");
  const Property& pNumberOfBreaks = this->getProperty("number_of_breaks");

  m_scale = te::layout::Property::GetValueAs<double>(pScale);
  m_gapX = te::layout::Property::GetValueAs<double>(pScaleGapX);
  m_gapY = te::layout::Property::GetValueAs<double>(pScaleGapY);
  m_font = te::layout::Property::GetValueAs<Font>(pTextFont);
  m_scaleUnitGapX = te::layout::Property::GetValueAs<double>(pScaleUnitGapX);
  m_numberOfBreaks = te::layout::Property::GetValueAs<int>(pNumberOfBreaks);
}


bool te::layout::ScaleItem::validateGaps()
{

  const Property& pScaleGapX = this->getProperty("scale_width_rect_gap");
  const Property& pScaleGapY = this->getProperty("scale_height_rect_gap");
  const Property& pScaleUnitGapX = this->getProperty("scale_in_unit_width_rect_gap");

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

void te::layout::ScaleItem::setResize(bool resize)
{
  m_isResizeTPLeft = resize;
  m_isResizeTPRight = resize;
  m_isResizeTPTop = resize;
  m_isResizeTPLower = resize;
  m_isResizeTPTopLeft = resize;
  m_isResizeTPTopRight = resize;
  m_isResizeTPLowerLeft = resize;
  m_isResizeTPLowerRight = resize;
}

