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
  \file GridMapItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridMapItem.h"

#include "../../core/property/GridSettingsConfigProperties.h"
#include "../core/ItemUtils.h"
#include "../core/Scene.h"

#include <QBrush>

#include <memory>

te::layout::GridMapItem::GridMapItem(AbstractItemController* controller, bool invertedMatrix)
  : AbstractItem<QGraphicsItem>(controller, invertedMatrix)
  , m_maxWidthTextMM(0)
  , m_maxHeigthTextMM(0)
  , m_changeSize(false)
  , m_showDebugDrawings(false)
  , m_defaultRotate(90)
{  
}

te::layout::GridMapItem::~GridMapItem()
{

}

void te::layout::GridMapItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  GridSettingsConfigProperties settingsConfig;

  const Property& pVisible = pGridSettings.containsSubProperty(settingsConfig.getVisible());
  bool visible = pVisible.getValue().toBool();

  if(visible)
  {
    drawGrid(painter);
  }
}

void te::layout::GridMapItem::drawGrid( QPainter* painter )
{

}

void te::layout::GridMapItem::configPainter( QPainter* painter )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  GridSettingsConfigProperties settingsConfig;

  const Property& pLineStyle = pGridSettings.containsSubProperty(settingsConfig.getLineStyle());
  const Property& pLineColor = pGridSettings.containsSubProperty(settingsConfig.getLineColor());
  const Property& pLineWidth = pGridSettings.containsSubProperty(settingsConfig.getLineWidth());

  const std::string& lineStyleName = pLineStyle.getOptionByCurrentChoice().toString();
  const te::color::RGBAColor& lineColor = pLineColor.getValue().toColor();
  double lineWidth = pLineWidth.getValue().toDouble();

  EnumLineStyleType* lineStyle = Enums::getInstance().getEnumLineStyleType();
  EnumType* currentLineStyle = Enums::getInstance().getEnumLineStyleType()->getEnum(lineStyleName);

  if(currentLineStyle == 0)
  {
    currentLineStyle = Enums::getInstance().getEnumLineStyleType()->searchLabel(lineStyleName);
  }

  QPen pen;

  if(currentLineStyle == lineStyle->getStyleSolid())
  {
    pen.setStyle(Qt::SolidLine);
  }
  else if(currentLineStyle == lineStyle->getStyleDot())
  {
    pen.setStyle(Qt::DotLine);
  }
  else if(currentLineStyle == lineStyle->getStyleDash())
  {
    pen.setStyle(Qt::DashLine);
  }
  else if(currentLineStyle == lineStyle->getStyleDashDot())
  {
    pen.setStyle(Qt::DashDotLine);
  }
  else if(currentLineStyle == lineStyle->getStyleDashDotDot())
  {
    pen.setStyle(Qt::DashDotDotLine);
  }

  QColor clrLine(lineColor.getRed(), lineColor.getGreen(), lineColor.getBlue(), lineColor.getAlpha());
  pen.setColor(clrLine);
  pen.setWidthF(lineWidth);

  painter->setPen(pen);
}

void te::layout::GridMapItem::configTextPainter( QPainter* painter )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  GridSettingsConfigProperties settingsConfig;

  const Property& pTextFontFamily = pGridSettings.containsSubProperty(settingsConfig.getFont());
  const Property& pTextColor = pGridSettings.containsSubProperty(settingsConfig.getTextColor());

  Font txtFont =  pTextFontFamily.getValue().toFont();
  const te::color::RGBAColor& textColor = pTextColor.getValue().toColor();

  QColor clrText(textColor.getRed(), textColor.getGreen(), textColor.getBlue(), textColor.getAlpha());

  ItemUtils::ConfigurePainterForTexts(painter, txtFont);

  QBrush brush (clrText);
  painter->setBrush(brush);
}

void te::layout::GridMapItem::clear()
{
  m_verticalLines.clear();
  m_horizontalLines.clear();
  m_topTexts.clear();
  m_bottomTexts.clear();
  m_rightTexts.clear();
  m_leftTexts.clear();

  m_boundingBox = te::gm::Envelope();
}

void te::layout::GridMapItem::drawContinuousLines( QPainter* painter )
{
  painter->save();

  configPainter(painter);

  drawVerticalLines(painter);

  drawHorizontalLines(painter);

  drawTexts(painter);

  painter->restore();
}

void te::layout::GridMapItem::drawVerticalLines( QPainter* painter )
{
  QList<te::gm::LineString>::const_iterator it = m_verticalLines.begin();
  for( ; it != m_verticalLines.end() ; ++it )
  {
    const te::gm::LineString& line = *it;

    QPainterPath path = ItemUtils::lineToQPath(line);
    painter->drawPath(path);
  }
}

void te::layout::GridMapItem::drawHorizontalLines( QPainter* painter )
{
  QList<te::gm::LineString>::const_iterator it = m_horizontalLines.begin();
  for (; it != m_horizontalLines.end(); ++it)
  {
    const te::gm::LineString& line = *it;

    QPainterPath path = ItemUtils::lineToQPath(line);
    painter->drawPath(path);
  }
}

void te::layout::GridMapItem::drawTexts( QPainter* painter )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  GridSettingsConfigProperties settingsConfig;

  const Property& pVisibleAllTexts = pGridSettings.containsSubProperty(settingsConfig.getVisibleAllTexts());
  const Property& pLeftText = pGridSettings.containsSubProperty(settingsConfig.getLeftText());
  const Property& pRightText = pGridSettings.containsSubProperty(settingsConfig.getRightText());
  const Property& pTopText = pGridSettings.containsSubProperty(settingsConfig.getTopText());
  const Property& pBottomText = pGridSettings.containsSubProperty(settingsConfig.getBottomText());

  bool visibleAllTexts = pVisibleAllTexts.getValue().toBool();
  bool leftText = pLeftText.getValue().toBool();
  bool rightText = pRightText.getValue().toBool();
  bool topText = pTopText.getValue().toBool();
  bool bottomText = pBottomText.getValue().toBool();

  if(visibleAllTexts == false)
  {
    return;
  }

  painter->save();

  configTextPainter(painter);
  
  if(leftText == true)
  {
    drawLeftTexts(painter);
  }

  if(rightText == true)
  {
    drawRightTexts(painter);
  }
  
  if(bottomText == true)
  {
    drawBottomTexts(painter);
  }

  if(topText == true)
  {
    drawTopTexts(painter);
  }

  painter->restore();  
}

void te::layout::GridMapItem::drawTopTexts( QPainter* painter )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  GridSettingsConfigProperties settingsConfig;
  const Property& pTopRotateText = pGridSettings.containsSubProperty(settingsConfig.getTopRotateText());
  const Property& pFont = pGridSettings.containsSubProperty(settingsConfig.getFont());

  bool bRotate = pTopRotateText.getValue().toBool();
  const Font& font = pFont.getValue().toFont();

  QFont qFont = ItemUtils::convertToQfont(font);

  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotate;
  }

  std::vector<TextPosition>::iterator it = m_topTexts.begin();


  const Property& pSuperscript = m_controller->getProperty(settingsConfig.getSuperscriptText());
  bool useSuperScript = pSuperscript.getValue().toBool();

  for( ; it != m_topTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;


    if (useSuperScript == true && txt.size() > 2){
      QFont fontSScript2 = qFont;
      fontSScript2.setPointSize(fontSScript2.pointSize() / 2);

      QString completeTxt(txt.c_str());
      QPainterPath completeTxtObject = ItemUtils::textToVector(completeTxt, qFont, QPointF(), 0);
      int index = txt.size() / 2;
      int indexNegative = (txt.size() - 1) / 2;

      QString txtSScript1(txt.at(0) == '-' ? txt.substr(0, indexNegative + 1).c_str() : txt.substr(0, index).c_str());
      QString txtSScript2(txt.at(0) == '-' ? txt.substr(indexNegative + 1, txt.size()).c_str() : txt.substr(index, txt.size()).c_str());

      QPainterPath textObject = ItemUtils::textToVector(txtSScript1, fontSScript2, QPointF(), 0);

      QPointF pointSScript(pt.x(), pt.y());
      pointSScript.setY(pt.y() + ((completeTxtObject.boundingRect().height() / 2)));
      drawText(pointSScript, painter, fontSScript2, txtSScript1.toStdString(), iRotate);

      pt.setX(pt.x() + textObject.boundingRect().width());

      drawText(pt, painter, qFont, txtSScript2.toStdString(), iRotate);


    }
    else{
      drawText(pt, painter, qFont, txt, iRotate);
    }


#ifdef _DEBUG
    debugDrawTextRect(painter, pt, txt, qFont, iRotate);
#endif
  }
}

void te::layout::GridMapItem::drawBottomTexts( QPainter* painter )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  GridSettingsConfigProperties settingsConfig;
  const Property& pBottomRotateText = pGridSettings.containsSubProperty(settingsConfig.getBottomRotateText());
  const Property& pFont = pGridSettings.containsSubProperty(settingsConfig.getFont());

  bool bRotate = pBottomRotateText.getValue().toBool();
  const Font& font = pFont.getValue().toFont();

  QFont qFont = ItemUtils::convertToQfont(font);

  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotate;
  }

  std::vector<TextPosition>::iterator it = m_bottomTexts.begin();


  const Property& pSuperscript = m_controller->getProperty(settingsConfig.getSuperscriptText());
  bool useSuperScript = pSuperscript.getValue().toBool();

  for( ; it != m_bottomTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;


    if (useSuperScript == true && txt.size() > 2){
      QFont fontSScript2 = qFont;
      fontSScript2.setPointSize(fontSScript2.pointSize() / 2);

      QString completeTxt(txt.c_str());
      QPainterPath completeTxtObject = ItemUtils::textToVector(completeTxt, qFont, QPointF(), 0);
      int index = txt.size() / 2;
      int indexNegative = (txt.size() - 1) / 2;

      QString txtSScript1(txt.at(0) == '-' ? txt.substr(0, indexNegative + 1).c_str() : txt.substr(0, index).c_str());
      QString txtSScript2(txt.at(0) == '-' ? txt.substr(indexNegative + 1, txt.size()).c_str() : txt.substr(index, txt.size()).c_str());

      QPainterPath textObject = ItemUtils::textToVector(txtSScript1, fontSScript2, QPointF(), 0);

      QPointF pointSScript(pt.x(), pt.y());
      pointSScript.setY(pt.y() + ((completeTxtObject.boundingRect().height() / 2)));
      drawText(pointSScript, painter, fontSScript2, txtSScript1.toStdString(), iRotate);

      pt.setX(pt.x() + textObject.boundingRect().width());

      drawText(pt, painter, qFont, txtSScript2.toStdString(), iRotate);

    }
    else{
      drawText(pt, painter, qFont, txt, iRotate);
    }

#ifdef _DEBUG
    debugDrawTextRect(painter, pt, txt, qFont, iRotate);
#endif
  }
}

void te::layout::GridMapItem::drawLeftTexts( QPainter* painter )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  GridSettingsConfigProperties settingsConfig;
  const Property& pLeftRotateText = pGridSettings.containsSubProperty(settingsConfig.getLeftRotateText());
  const Property& pFont = pGridSettings.containsSubProperty(settingsConfig.getFont());

  bool bRotate = pLeftRotateText.getValue().toBool();
  const Font& font = pFont.getValue().toFont();

  QFont qFont = ItemUtils::convertToQfont(font);

  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotate;
  }

  double width = 0;
  double height = 0;
  
  std::vector<TextPosition>::iterator it = m_leftTexts.begin();

  const Property& pSuperscript = m_controller->getProperty(settingsConfig.getSuperscriptText());
  bool useSuperScript = pSuperscript.getValue().toBool();


  for( ; it != m_leftTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;

    if (useSuperScript == true && txt.size() > 2){
      QFont fontSScript2 = qFont;
      fontSScript2.setPointSize(fontSScript2.pointSize() / 2);

      QString completeTxt(txt.c_str());
      QPainterPath completeTxtObject = ItemUtils::textToVector(completeTxt, qFont, QPointF(), 0);
      int index = txt.size() / 2;
      int indexNegative = (txt.size() - 1) / 2;

      QString txtSScript1(txt.at(0) == '-' ? txt.substr(0, indexNegative + 1).c_str() : txt.substr(0, index).c_str());
      QString txtSScript2(txt.at(0) == '-' ? txt.substr(indexNegative + 1, txt.size()).c_str() : txt.substr(index, txt.size()).c_str());

      QPainterPath textObject = ItemUtils::textToVector(txtSScript1, fontSScript2, QPointF(), 0);

      QPointF pointSScript(pt.x(), pt.y());
      pointSScript.setY(pt.y() + ((completeTxtObject.boundingRect().height() / 2)));
      drawText(pointSScript, painter, fontSScript2, txtSScript1.toStdString(), iRotate);

      pt.setX(pt.x() + textObject.boundingRect().width());

      drawText(pt, painter, qFont, txtSScript2.toStdString(), iRotate);

    }
    else{
      drawText(pt, painter, qFont, txt, iRotate);
    }

#ifdef _DEBUG
    debugDrawTextRect(painter, pt, txt, qFont, iRotate);
#endif

  }
}

void te::layout::GridMapItem::drawRightTexts( QPainter* painter )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  GridSettingsConfigProperties settingsConfig;
  const Property& pRightRotateText = pGridSettings.containsSubProperty(settingsConfig.getRightRotateText());
  const Property& pFont = pGridSettings.containsSubProperty(settingsConfig.getFont());

  bool bRotate = pRightRotateText.getValue().toBool();
  const Font& font = pFont.getValue().toFont();

  QFont qFont = ItemUtils::convertToQfont(font);

  const Property& pSuperscript = m_controller->getProperty(settingsConfig.getSuperscriptText());
  bool useSuperScript = pSuperscript.getValue().toBool();

  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotate;
  }
  
  std::vector<TextPosition>::iterator it = m_rightTexts.begin();
  for( ; it != m_rightTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;  

    if (useSuperScript == true && txt.size() > 2){
      QFont fontSScript2 = qFont;
      fontSScript2.setPointSize(fontSScript2.pointSize() / 2);

      QString completeTxt(txt.c_str());
      QPainterPath completeTxtObject = ItemUtils::textToVector(completeTxt, qFont, QPointF(), 0);
      int index = txt.size() / 2;
      int indexNegative = (txt.size() - 1) / 2;

      QString txtSScript1(txt.at(0) == '-' ? txt.substr(0, indexNegative + 1).c_str() : txt.substr(0, index).c_str());
      QString txtSScript2(txt.at(0) == '-' ? txt.substr(indexNegative + 1, txt.size()).c_str() : txt.substr(index, txt.size()).c_str());

      QPainterPath textObject = ItemUtils::textToVector(txtSScript1, fontSScript2, QPointF(), 0);

      QPointF pointSScript(pt.x(), pt.y());
      pointSScript.setY(pt.y() + ((completeTxtObject.boundingRect().height() / 2)));
      drawText(pointSScript, painter, fontSScript2, txtSScript1.toStdString(), iRotate);

      pt.setX(pt.x() + textObject.boundingRect().width());

      drawText(pt, painter, qFont, txtSScript2.toStdString(), iRotate);

    }
    else{

      drawText(pt, painter, qFont, txt, iRotate);

    }
#ifdef _DEBUG
    debugDrawTextRect(painter, pt, txt, qFont, iRotate);
#endif
  }
}

QRectF te::layout::GridMapItem::boundingRect() const
{
  if (m_currentAction == RESIZE_ACTION)
  {
    return AbstractItem<QGraphicsItem>::boundingRect();
  }

  if(m_boundingBox.isValid())
  {
    return QRectF(m_boundingBox.getLowerLeftX(), m_boundingBox.getLowerLeftY(), m_boundingBox.getWidth(), m_boundingBox.getHeight());
  }
  return AbstractItem<QGraphicsItem>::boundingRect();
}

void te::layout::GridMapItem::drawCrossLines(QPainter* painter)
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  painter->save();

  configPainter(painter);

  GridSettingsConfigProperties settingsConfig;

  const Property& pCrossOffset = pGridSettings.containsSubProperty(settingsConfig.getCrossOffset());
  double crossOffSet = pCrossOffset.getValue().toDouble();

  QList<te::gm::LineString>::iterator itv = m_verticalLines.begin();
  for( ; itv != m_verticalLines.end() ; ++itv )
  {
    const te::gm::LineString& verticalLine = (*itv);

    QList<te::gm::LineString>::iterator ith = m_horizontalLines.begin();
    for( ; ith != m_horizontalLines.end() ; ++ith )
    {
      const te::gm::LineString& horizontalLine = (*ith);

      std::auto_ptr<te::gm::Geometry> interGeometry;
      try
      {
        interGeometry.reset(verticalLine.intersection(&horizontalLine));
      }
      catch (...)
      {
        continue;
      }
      
      if (interGeometry.get() == 0 || interGeometry->isValid() == false)
      {
        continue;
      }

      if (interGeometry->getGeomTypeId() != te::gm::PointType)
      {
        continue;
      }

      te::gm::Point* interPoint = dynamic_cast<te::gm::Point*>(interGeometry.get());
      if (interPoint == 0)
      {
        continue;
      }

      QLineF lneHrz(interPoint->getX() - crossOffSet, interPoint->getY(), interPoint->getX() + crossOffSet, interPoint->getY());
      QLineF lneVrt(interPoint->getX(), interPoint->getY() - crossOffSet, interPoint->getX(), interPoint->getY() + crossOffSet);
        
      if(drawCrossIntersectMapBorder(lneVrt, lneHrz, painter) == true)
      {
        continue;
      }

      painter->drawLine(lneHrz);
      painter->drawLine(lneVrt);
    }
  }

  configPainter(painter);
  
  drawTexts(painter);

  painter->restore();
}

bool te::layout::GridMapItem::drawCrossIntersectMapBorder( QLineF vrt, QLineF hrz, QPainter* painter )
{
  bool result = false;

  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return result;

  GridSettingsConfigProperties settingsConfig;

  const Property& pWidth = m_controller->getProperty("width");
  const Property& pHeight = m_controller->getProperty("height");
  const Property& pCrossOffset = pGridSettings.containsSubProperty(settingsConfig.getCrossOffset());

  double width = pWidth.getValue().toDouble();
  double height = pHeight.getValue().toDouble();
  double crossOffSet = pCrossOffset.getValue().toDouble();

  //if the cross intersects the border of the reference rect, a line must be drawn instead of the cross
  te::gm::Envelope boxMM(0, 0, width, height);

  QPointF intersectionPoint;

  QLineF bottomLine(boxMM.m_llx, boxMM.m_lly, boxMM.m_urx, boxMM.m_lly);
  QLineF topLine(boxMM.m_llx, boxMM.m_ury, boxMM.m_urx, boxMM.m_ury);
  QLineF leftLine(boxMM.m_llx, boxMM.m_lly, boxMM.m_llx, boxMM.m_ury);
  QLineF rightLine(boxMM.m_urx, boxMM.m_lly, boxMM.m_urx, boxMM.m_ury);

  bool intersects = false;
  if (bottomLine.intersect(vrt, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x(), intersectionPoint.y() + crossOffSet);
    intersects = true;

    painter->drawLine(borderLine);
  }
  if (topLine.intersect(vrt, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x(), intersectionPoint.y() - crossOffSet);
    intersects = true;

    painter->drawLine(borderLine);
  }
  if (leftLine.intersect(hrz, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x() + crossOffSet, intersectionPoint.y());
    intersects = true;

    painter->drawLine(borderLine);
  }
  if (rightLine.intersect(hrz, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x() - crossOffSet, intersectionPoint.y());
    intersects = true;

    painter->drawLine(borderLine);
  }

  return intersects;
}

void te::layout::GridMapItem::debugDrawTextRect(QPainter* painter, const QPointF& point, const std::string& text, QFont font, int rotate)
{
  if (m_showDebugDrawings == false)
  {
    return;
  }

  if (scene() == 0)
  {
    return;
  }

  AbstractScene* myScene = dynamic_cast<AbstractScene*>(scene());
  if (myScene == 0)
  {
    return;
  }

  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

 //creates the rect
  QPainterPath textObject = ItemUtils::textToVector(text.c_str(), font, point, rotate);

//draws the rect
  painter->save();

  QPen pen;
  pen.setWidth(0);
  pen.setColor(QColor(255, 0, 0));
  painter->setPen(pen);
  painter->setBrush(Qt::NoBrush);

  painter->drawRect(textObject.boundingRect());

  painter->restore();
}

te::gm::Envelope te::layout::GridMapItem::calculateTop(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double horizontalDisplacement)
{
  double textX = textBoundingRect.x();
  double textY = textBoundingRect.y();
  double textWidth = textBoundingRect.width();
  double textHeight = textBoundingRect.height();

  //if the 90 degrees rotation was applied, we transpose the bounding rect
  if (rotate)
  {
    textX = textBoundingRect.y();
    textY = textBoundingRect.x();
    textWidth = textBoundingRect.height();
    textHeight = textBoundingRect.width();
  }

  double x = referencePoint.x();
  double ury = referencePoint.y();

  // text top
  QPointF ptTop(x - (textWidth / 2.), ury + horizontalDisplacement);
  m_topTexts.push_back(TextPosition(text.toStdString(), ptTop));

  te::gm::Envelope topTextBox(ptTop.x(), ptTop.y(), ptTop.x() + textWidth, ptTop.y() + textHeight);
  m_boundingBox.Union(topTextBox);

  return topTextBox;
}

te::gm::Envelope te::layout::GridMapItem::calculateBottom(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double horizontalDisplacement)
{
  double textX = textBoundingRect.x();
  double textY = textBoundingRect.y();
  double textWidth = textBoundingRect.width();
  double textHeight = textBoundingRect.height();

  //if the 90 degrees rotation was applied, we transpose the bounding rect
  if (rotate)
  {
    textX = textBoundingRect.y();
    textY = textBoundingRect.x();
    textWidth = textBoundingRect.height();
    textHeight = textBoundingRect.width();
  }

  double x = referencePoint.x();
  double lly = referencePoint.y();

  // text bottom
  QPointF ptBottom(x - (textWidth / 2.), lly - textHeight - horizontalDisplacement);
  m_bottomTexts.push_back(TextPosition(text.toStdString(), ptBottom));

  te::gm::Envelope bottomTextBox(ptBottom.x(), ptBottom.y(), ptBottom.x() + textWidth, ptBottom.y() + textHeight);
  m_boundingBox.Union(bottomTextBox);

  return bottomTextBox;
}

te::gm::Envelope te::layout::GridMapItem::calculateRight(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double verticalDisplacement)
{
  double textX = textBoundingRect.x();
  double textY = textBoundingRect.y();
  double textWidth = textBoundingRect.width();
  double textHeight = textBoundingRect.height();

  double urx = referencePoint.x();
  double y = referencePoint.y();

  if (rotate)
  {
    textX = textBoundingRect.y();
    textY = textBoundingRect.x();
    textWidth = textBoundingRect.height();
    textHeight = textBoundingRect.width();
  }

  double yReference = y + textY - (textHeight / 2.);

  // text right
  QPointF ptRight(urx + verticalDisplacement, yReference);
  m_rightTexts.push_back(TextPosition(text.toStdString(), ptRight));

  te::gm::Envelope rightTextBox(ptRight.x(), ptRight.y(), ptRight.x() + textWidth, ptRight.y() + textHeight);
  m_boundingBox.Union(rightTextBox);

  return rightTextBox;
}

te::gm::Envelope te::layout::GridMapItem::calculateLeft(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double verticalDisplacement)
{
  double textX = textBoundingRect.x();
  double textY = textBoundingRect.y();
  double textWidth = textBoundingRect.width();
  double textHeight = textBoundingRect.height();

  //if the 90 degrees rotation was applied, we transpose the bounding rect
  if (rotate)
  {
    textX = textBoundingRect.y();
    textY = textBoundingRect.x();
    textWidth = textBoundingRect.height();
    textHeight = textBoundingRect.width();
  }

  double llx = referencePoint.x();
  double y = referencePoint.y();

  double yReference = y + textY - (textHeight / 2.);

  // text left
  QPointF ptLeft(llx - textX - textWidth - verticalDisplacement, yReference);
  m_leftTexts.push_back(TextPosition(text.toStdString(), ptLeft));

  te::gm::Envelope leftTextBox(ptLeft.x(), ptLeft.y(), ptLeft.x() + textWidth, ptLeft.y() + textHeight);
  m_boundingBox.Union(leftTextBox);

  return leftTextBox;
}
