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
#include "../../core/property/GeodesicGridSettingsConfigProperties.h"
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

void te::layout::GridMapItem::addGridLinesToPath()
{
  for (int i = 0; i < m_horizontalLines.size(); i++)
  {
    m_gridLines.addPath(ItemUtils::lineToQPath(m_horizontalLines.at(i)));
  }

  for (int i = 0; i < m_verticalLines.size(); i++)
  {
    m_gridLines.addPath(ItemUtils::lineToQPath(m_verticalLines.at(i)));
  }
  calculateTexts();
}


void te::layout::GridMapItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{

  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  GridSettingsConfigProperties settingsConfig;

  const Property& pVisible = pGridSettings.containsSubProperty(settingsConfig.getVisible());
  bool visible = pVisible.getValue().toBool();

  if (visible == false)
  {
    return;
  }

  painter->save();

  configPainter(painter);

  painter->drawPath(m_gridLines);

  painter->drawPath(m_gridCrosses);

  configTextPainter(painter);


  QPen pen = painter->pen();
  pen.setWidthF(0);

  QBrush brush = painter->brush();
  brush.setStyle(Qt::SolidPattern);

  painter->setPen(pen);
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->fillPath(m_gridText, brush);

  painter->restore();

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

  m_verticalLines.clear();

  m_gridLines = QPainterPath();
  m_gridCrosses = QPainterPath();

  m_gridText = QPainterPath();
  
  m_boundingBox = te::gm::Envelope();
}

void te::layout::GridMapItem::clearLines()
{
  m_gridLines = QPainterPath();
  m_gridCrosses = QPainterPath();
  m_gridText = QPainterPath();
}



void te::layout::GridMapItem::calculateTexts()
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
  
  if(leftText == true)
  {
    calculateLeftTexts();
  }

  if(rightText == true)
  {
    calculateRightTexts();
  }
  
  if(bottomText == true)
  {
    calculateBottomTexts();
  }

  if(topText == true)
  {
    calculateTopTexts();
  }

}

void te::layout::GridMapItem::calculateTopTexts()
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
      calculateSuperScriptText(pt, qFont, txt, iRotate);
    }
    else{
      calculateText(pt, qFont, txt, iRotate);
    }


#ifdef _DEBUG
    //debugDrawTextRect(painter, pt, txt, qFont, iRotate);
#endif
  }
}

void te::layout::GridMapItem::calculateText(const QPointF& point, const QFont& font, const std::string& text, int rotate)
{
  QString qText = ItemUtils::convert2QString(text);

  m_gridText.addPath(ItemUtils::textToVector(qText, font, point, rotate));

}

void te::layout::GridMapItem::calculateSuperScriptText(const QPointF& point, const QFont& font, const std::string& text, int rotate)
{
  ItemUtils itemUtils = this->getScene()->getItemUtils();

  std::vector<QString> textVect;
  std::vector<QFont> fontVect;

  QFont fontSScript2 = font;
  fontSScript2.setPointSize(fontSScript2.pointSize() / 2);

  fontVect.push_back(fontSScript2);
  fontVect.push_back(font);

  int index = (int)text.size() / 2;
  int indexNegative = (int)(text.size() - 1) / 2;

  std::string txtSubstr1 = text.substr(0, indexNegative + 1).c_str();
  QString qTxtSubstr1 = ItemUtils::convert2QString(txtSubstr1);

  std::string txtSubstr2 = text.substr(0, index).c_str();
  QString qTxtSubstr2 = ItemUtils::convert2QString(txtSubstr2);

  std::string txtSubstr3 = text.substr(indexNegative + 1, text.size()).c_str();
  QString qTxtSubstr3 = ItemUtils::convert2QString(txtSubstr3);

  std::string txtSubstr4 = text.substr(index, text.size()).c_str();
  QString qTxtSubstr4 = ItemUtils::convert2QString(txtSubstr4);

  QString txtSScript1(text.at(0) == '-' ? qTxtSubstr1 : qTxtSubstr2);
  QString txtSScript2(text.at(0) == '-' ? qTxtSubstr3 : qTxtSubstr4);

  textVect.push_back(txtSScript1);

  textVect.push_back(txtSScript2);

  m_gridText.addPath(itemUtils.superscriptTextToVector(textVect, fontVect, point, rotate));

}


void te::layout::GridMapItem::calculateBottomTexts()
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
      this->calculateSuperScriptText(pt, qFont, txt, iRotate);
    }
    else{
      this->calculateText(pt, qFont, txt, iRotate);
    }

#ifdef _DEBUG
    //debugDrawTextRect(painter, pt, txt, qFont, iRotate);
#endif
  }
}

void te::layout::GridMapItem::calculateLeftTexts()
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

    if (useSuperScript == true && txt.size() > 2)
    {
      calculateSuperScriptText(pt, qFont, txt, iRotate);
    }
    else
    {
      this->calculateText(pt, qFont, txt, iRotate);
    }

#ifdef _DEBUG
    //debugDrawTextRect(painter, pt, txt, qFont, iRotate);
#endif

  }
}

void te::layout::GridMapItem::calculateRightTexts()
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

    if (useSuperScript == true && txt.size() > 2)
    {
      calculateSuperScriptText(pt, qFont, txt, iRotate);
    }
    else
    {
      this->calculateText(pt, qFont, txt, iRotate);
    }
#ifdef _DEBUG
    //debugDrawTextRect(painter, pt, txt, qFont, iRotate);
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

void te::layout::GridMapItem::calculateCrossLines()
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  GridSettingsConfigProperties settingsConfig;

  const Property& pCrossOffset = pGridSettings.containsSubProperty(settingsConfig.getCrossOffset());
  double crossOffSet = pCrossOffset.getValue().toDouble();

  
  const Property& pUseBouderIntersection = pGridSettings.containsSubProperty(settingsConfig.getBouderIntersections());
  bool useBouderItersection = pUseBouderIntersection.getValue().toBool();

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
        
      if (calculateCrossIntersectMapBorder(lneVrt, lneHrz) == true)
      {
        continue;
      }

      m_gridCrosses.moveTo(lneHrz.p1().x(), lneHrz.p1().y());
      m_gridCrosses.lineTo(lneHrz.p2().x(), lneHrz.p2().y());

      m_gridCrosses.moveTo(lneVrt.p1().x(), lneVrt.p1().y());
      m_gridCrosses.lineTo(lneVrt.p2().x(), lneVrt.p2().y());
      
    }
  }

  if (useBouderItersection)
  {
    calculateBoldersSegments(crossOffSet);
  }
  
  calculateTexts();

}


void te::layout::GridMapItem::calculateBoldersSegments(double crossOffSet){

  const Property& pWidth = m_controller->getProperty("width");
  const Property& pHeight = m_controller->getProperty("height");

  double width = pWidth.getValue().toDouble();
  double height = pHeight.getValue().toDouble();

  te::gm::Envelope boxMM(0, 0, width, height);

  te::gm::LineString topBorderLine(2, te::gm::LineStringType);
  topBorderLine.setPoint(0, boxMM.getLowerLeftX(), boxMM.getUpperRightY());
  topBorderLine.setPoint(1, boxMM.getUpperRightX(), boxMM.getUpperRightY());

  te::gm::LineString bottomBorderLine(2, te::gm::LineStringType);
  bottomBorderLine.setPoint(0, boxMM.getLowerLeftX(), boxMM.getLowerLeftY());
  bottomBorderLine.setPoint(1, boxMM.getUpperRightX(), boxMM.getLowerLeftY());

  te::gm::LineString leftBorderLine(2, te::gm::LineStringType);
  leftBorderLine.setPoint(0, boxMM.getLowerLeftX(), boxMM.getLowerLeftY());
  leftBorderLine.setPoint(1, boxMM.getLowerLeftX(), boxMM.getUpperRightY());

  te::gm::LineString rightBorderLine(2, te::gm::LineStringType);
  rightBorderLine.setPoint(0, boxMM.getUpperRightX(), boxMM.getLowerLeftY());
  rightBorderLine.setPoint(1, boxMM.getUpperRightX(), boxMM.getUpperRightY());


  QList<te::gm::LineString>::iterator ith = m_horizontalLines.begin();

  for (; ith != m_horizontalLines.end(); ++ith)
  {
    const te::gm::LineString& horizontalLine = (*ith);

    te::gm::Point p;
    if (checkBolderIntersection(leftBorderLine, horizontalLine, p) == false)
    {
      continue;
    }
    
    
    bool nearToEdge = isNearEdge(p, leftBorderLine, crossOffSet);

    if (!nearToEdge)
    {
      QLineF lneHrzIntersection(p.getX(), p.getY(), p.getX() + crossOffSet, p.getY());
      
      m_gridCrosses.moveTo(lneHrzIntersection.p1().x(), lneHrzIntersection.p1().y());
      m_gridCrosses.lineTo(lneHrzIntersection.p2().x(), lneHrzIntersection.p2().y());
    }

  }

  ith = m_horizontalLines.begin();
  for (; ith != m_horizontalLines.end(); ++ith)
  {
    const te::gm::LineString& horizontalLine = (*ith);

    te::gm::Point p;

    if (checkBolderIntersection(rightBorderLine, horizontalLine, p) == false)
    {
      continue;
    }

    bool nearToEdge = isNearEdge(p, rightBorderLine, crossOffSet);

    if (!nearToEdge)
    {
      QLineF lneHrzIntersection(p.getX(), p.getY(), p.getX() - crossOffSet, p.getY());

      m_gridCrosses.moveTo(lneHrzIntersection.p1().x(), lneHrzIntersection.p1().y());
      m_gridCrosses.lineTo(lneHrzIntersection.p2().x(), lneHrzIntersection.p2().y());

    }

    

  }

  QList<te::gm::LineString>::iterator itv = m_verticalLines.begin();

  for (; itv != m_verticalLines.end(); ++itv)
  {
    const te::gm::LineString& verticalLine = (*itv);

    te::gm::Point p;
    if (checkBolderIntersection(topBorderLine, verticalLine, p) == false)
    {
      continue;
    }

   
    bool nearToEdge = isNearEdge(p, topBorderLine, crossOffSet);

    if (!nearToEdge)
    {
      QLineF lneVrtIntersection(p.getX(), p.getY(), p.getX(), p.getY() - crossOffSet);
      m_gridCrosses.moveTo(lneVrtIntersection.p1().x(), lneVrtIntersection.p1().y());
      m_gridCrosses.lineTo(lneVrtIntersection.p2().x(), lneVrtIntersection.p2().y());
      
    }

      
}

  itv = m_verticalLines.begin();

  for (; itv != m_verticalLines.end(); ++itv)
  {
    const te::gm::LineString& verticalLine = (*itv);

    te::gm::Point p;
    if (checkBolderIntersection(bottomBorderLine, verticalLine, p) == false)
    {
      continue;
    }

    QLineF lneVrtIntersection(p.getX(), p.getY(), p.getX(), p.getY() + crossOffSet);


    bool nearToEdge = isNearEdge(p, bottomBorderLine, crossOffSet);

    if (!nearToEdge)
    {
      m_gridCrosses.moveTo(lneVrtIntersection.p1().x(), lneVrtIntersection.p1().y());
      m_gridCrosses.lineTo(lneVrtIntersection.p2().x(), lneVrtIntersection.p2().y());

    }

  }

}

bool te::layout::GridMapItem::isNearEdge(const te::gm::Point& p, const te::gm::LineString& line, const double& offSet){


  te::gm::Point bottonPoint(line.getPointN(0)->getX(), line.getPointN(0)->getY());
  te::gm::Point upperPoint(line.getPointN(1)->getX(), line.getPointN(1)->getY());
  double distance1 = p.distance(&bottonPoint);
  double distance2 = p.distance(&upperPoint);

  if (distance1 <= offSet)
  {
    return true;
  }
  if (distance2 <= offSet)
  {
    return true;
  }

  return false;

}



bool te::layout::GridMapItem::checkBolderIntersection(const te::gm::LineString& bolderLine, const te::gm::LineString& gridLine, te::gm::Point& intersectionPoint)
{

  std::auto_ptr<te::gm::Geometry> interGeometry;
  te::gm::Point* interPoint = 0;
  try
  {
    interGeometry.reset(bolderLine.intersection(&gridLine));
  }
  catch (...)
  {
    return false;
  }

  if (interGeometry.get() == 0 || interGeometry->isValid() == false)
  {
    return false;
  }

  if (interGeometry->getGeomTypeId() != te::gm::PointType)
  {
    return false;
  }

  interPoint = dynamic_cast<te::gm::Point*>(interGeometry.get());

  intersectionPoint.setX(interPoint->getX());
  intersectionPoint.setY(interPoint->getY());

  return true;

}
bool te::layout::GridMapItem::calculateCrossIntersectMapBorder( QLineF vrt, QLineF hrz)
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

    m_gridCrosses.moveTo(borderLine.p1().x(), borderLine.p1().y());
    m_gridCrosses.lineTo(borderLine.p2().x(), borderLine.p2().y());

  }
  if (topLine.intersect(vrt, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x(), intersectionPoint.y() - crossOffSet);
    intersects = true;

    m_gridCrosses.moveTo(borderLine.p1().x(), borderLine.p1().y());
    m_gridCrosses.lineTo(borderLine.p2().x(), borderLine.p2().y());

  }
  if (leftLine.intersect(hrz, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x() + crossOffSet, intersectionPoint.y());
    intersects = true;

    m_gridCrosses.moveTo(borderLine.p1().x(), borderLine.p1().y());
    m_gridCrosses.lineTo(borderLine.p2().x(), borderLine.p2().y());

  }
  if (rightLine.intersect(hrz, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x() - crossOffSet, intersectionPoint.y());
    intersects = true;

    m_gridCrosses.moveTo(borderLine.p1().x(), borderLine.p1().y());
    m_gridCrosses.lineTo(borderLine.p2().x(), borderLine.p2().y());

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

  QString qText = ItemUtils::convert2QString(text);

  //creates the rect
  QPainterPath textObject = ItemUtils::textToVector(qText, font, point, rotate);

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

  std::string stdText = ItemUtils::convert2StdString(text);
  m_topTexts.push_back(TextPosition(stdText, ptTop));

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

  std::string stdText = ItemUtils::convert2StdString(text);
  m_bottomTexts.push_back(TextPosition(stdText, ptBottom));

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

  std::string stdText = ItemUtils::convert2StdString(text);
  m_rightTexts.push_back(TextPosition(stdText, ptRight));

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

  std::string stdText = ItemUtils::convert2StdString(text);
  m_leftTexts.push_back(TextPosition(stdText, ptLeft));

  te::gm::Envelope leftTextBox(ptLeft.x(), ptLeft.y(), ptLeft.x() + textWidth, ptLeft.y() + textHeight);
  m_boundingBox.Union(leftTextBox);

  return leftTextBox;
}
