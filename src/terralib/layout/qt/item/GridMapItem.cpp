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

#include "../core/ItemUtils.h"
#include "../core/Scene.h"
#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/property/GridSettingsConfigProperties.h"
#include "../../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../../core/Utils.h"
#include "../../core/pattern/mvc/AbstractItemController.h"

#include <QBrush>

#include <memory>

te::layout::GridMapItem::GridMapItem()
  : AbstractItem()
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

void te::layout::GridMapItem::refresh()
{
  m_screenGreaterCache = QPixmap();
  m_screenCache = QPixmap();
  AbstractItem::refresh();
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
  /*
  const Property& pGridSettings = this->getProperty("GridSettings");

  GridSettingsConfigProperties settingsConfig;

  const Property& pVisible = pGridSettings.getSubProperty(settingsConfig.getVisible());
  bool visible = te::layout::Property::GetValueAs<bool>(pVisible);

  if (visible == false)
  {
    return;
  }

  if (m_isPrinting == true)
  {
    drawGridOnDevice(painter);
  }
  else
  {
    Scene* myScene = dynamic_cast<Scene*>(this->scene());
    if (myScene == 0)
    {
      return;
    }

    //we first calculate the size in pixels
    Utils utils = myScene->getUtils();

    QRectF boxMM = boundingRect();
    te::gm::Envelope boxViewport(0, 0, boxMM.width(), boxMM.height());
    boxViewport = utils.viewportBox(boxViewport);

    //if for any reason the size has been changed, we recreate the screen pixmap
    QSize sizeInPixels(qRound(boxViewport.getWidth()), qRound(boxViewport.getHeight()));
    if (m_screenCache.size() != sizeInPixels)
    {
      if (m_screenGreaterCache.width() < sizeInPixels.width() || m_screenGreaterCache.height() < sizeInPixels.height())
      {
        const Property& property = this->getProperty("background_color");
        const te::color::RGBAColor& color = te::layout::Property::GetValueAs<te::color::RGBAColor>(property);
        QColor qColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

        m_screenGreaterCache = QPixmap(sizeInPixels);
        m_screenGreaterCache.fill(qColor); //this is done to solve a printing problem. For some reason, the transparency is not being considered by the printer in Linux

        double xFactor = sizeInPixels.width() / boxMM.width();
        double yFactor = sizeInPixels.height() / boxMM.height();

        QTransform transform;
        transform.scale(xFactor, -yFactor);
        transform.translate(-boxMM.x(), -boxMM.height() - boxMM.y());

        QPainter cachePainter;
        cachePainter.begin(&m_screenGreaterCache);

        cachePainter.setTransform(transform);

        drawGridOnDevice(&cachePainter);
      }

      m_screenCache = m_screenGreaterCache.scaled(sizeInPixels, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    te::layout::ItemUtils::drawPixmap(this->boundingRect(), painter, m_screenCache);
  }
  */
}

void te::layout::GridMapItem::drawGridOnDevice( QPainter* painter )
{
  painter->save();

  painter->setRenderHint(QPainter::Antialiasing, true);

  configPainter(painter);

  painter->drawPath(m_gridLines);

  painter->drawPath(m_gridCrosses);

  configTextPainter(painter);

  QPen pen = painter->pen();
  pen.setWidthF(0);

  QBrush brush = painter->brush();
  brush.setStyle(Qt::SolidPattern);

  painter->setPen(pen);
  painter->fillPath(m_gridText, brush);

  painter->restore();
}

void te::layout::GridMapItem::configPainter( QPainter* painter )
{
  /*
  const Property& pGridSettings = this->getProperty("GridSettings");

  GridSettingsConfigProperties settingsConfig;

  const Property& pLineStyle = pGridSettings.getSubProperty(settingsConfig.getLineStyle());
  const Property& pLineColor = pGridSettings.getSubProperty(settingsConfig.getLineColor());
  const Property& pLineWidth = pGridSettings.getSubProperty(settingsConfig.getLineWidth());

  const std::string& lineStyleName = pLineStyle.getOptionByCurrentChoice().toString();
  const te::color::RGBAColor& lineColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pLineColor);
  double lineWidth = te::layout::Property::GetValueAs<double>(pLineWidth);

  EnumLineStyleType* lineStyle = Enums::getInstance().getEnumLineStyleType();
  EnumType* currentLineStyle = Enums::getInstance().getEnumLineStyleType()->getEnum(lineStyleName);

  QPen pen;

  if (currentLineStyle)
  {
    if (currentLineStyle == lineStyle->getStyleSolid())
    {
      pen.setStyle(Qt::SolidLine);
    }
    else if (currentLineStyle == lineStyle->getStyleDot())
    {
      pen.setStyle(Qt::DotLine);
    }
    else if (currentLineStyle == lineStyle->getStyleDash())
    {
      pen.setStyle(Qt::DashLine);
    }
    else if (currentLineStyle == lineStyle->getStyleDashDot())
    {
      pen.setStyle(Qt::DashDotLine);
    }
    else if (currentLineStyle == lineStyle->getStyleDashDotDot())
    {
      pen.setStyle(Qt::DashDotDotLine);
    }
  }

  QColor clrLine(lineColor.getRed(), lineColor.getGreen(), lineColor.getBlue(), lineColor.getAlpha());
  pen.setColor(clrLine);
  pen.setWidthF(lineWidth);

  painter->setPen(pen);
  */
}

void te::layout::GridMapItem::configTextPainter( QPainter* painter )
{
  /*/
  const Property& pGridSettings = this->getProperty("GridSettings");

  GridSettingsConfigProperties settingsConfig;

  const Property& pTextFontFamily = pGridSettings.getSubProperty(settingsConfig.getFont());
  const Property& pTextColor = pGridSettings.getSubProperty(settingsConfig.getTextColor());

  Font txtFont = te::layout::Property::GetValueAs<Font>(pTextFontFamily);
  const te::color::RGBAColor& textColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pTextColor);

  QColor clrText(textColor.getRed(), textColor.getGreen(), textColor.getBlue(), textColor.getAlpha());

  ItemUtils::ConfigurePainterForTexts(painter, txtFont);

  QBrush brush (clrText);
  painter->setBrush(brush);
  */
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
  /*
  const Property& pGridSettings = this->getProperty("GridSettings");

  GridSettingsConfigProperties settingsConfig;

  const Property& pVisibleAllTexts = pGridSettings.getSubProperty(settingsConfig.getVisibleAllTexts());
  const Property& pLeftText = pGridSettings.getSubProperty(settingsConfig.getLeftText());
  const Property& pRightText = pGridSettings.getSubProperty(settingsConfig.getRightText());
  const Property& pTopText = pGridSettings.getSubProperty(settingsConfig.getTopText());
  const Property& pBottomText = pGridSettings.getSubProperty(settingsConfig.getBottomText());

  bool visibleAllTexts = te::layout::Property::GetValueAs<bool>(pVisibleAllTexts);
  bool leftText = te::layout::Property::GetValueAs<bool>(pLeftText);
  bool rightText = te::layout::Property::GetValueAs<bool>(pRightText);
  bool topText = te::layout::Property::GetValueAs<bool>(pTopText);
  bool bottomText = te::layout::Property::GetValueAs<bool>(pBottomText);

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
  */
}

void te::layout::GridMapItem::calculateTopTexts()
{
  /*
  const Property& pGridSettings = this->getProperty("GridSettings");

  GridSettingsConfigProperties settingsConfig;
  const Property& pTopRotateText = pGridSettings.getSubProperty(settingsConfig.getTopRotateText());
  const Property& pFont = pGridSettings.getSubProperty(settingsConfig.getFont());

  bool bRotate = te::layout::Property::GetValueAs<bool>(pTopRotateText);
  const Font& font = te::layout::Property::GetValueAs<Font>(pFont);

  QFont qFont = ItemUtils::convertToQfont(font);

  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotate;
  }

  std::vector<TextPosition>::iterator it = m_topTexts.begin();


  const Property& pSuperscript = this->getProperty(settingsConfig.getSuperscriptText());
  bool useSuperScript = te::layout::Property::GetValueAs<bool>(pSuperscript);

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

  */
}

void te::layout::GridMapItem::calculateText(const QPointF& point, const QFont& font, const std::string& text, int rotate)
{
  QString qText = ItemUtils::convert2QString(text);

  m_gridText.addPath(ItemUtils::textToVector(qText, font, point, rotate));

}

void te::layout::GridMapItem::calculateSuperScriptText(const QPointF& point, const QFont& font, const std::string& text, int rotate)
{
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

  m_gridText.addPath(ItemUtils::superscriptTextToVector(textVect, fontVect, point, rotate));
}


void te::layout::GridMapItem::calculateBottomTexts()
{
  /*
  const Property& pGridSettings = this->getProperty("GridSettings");

  GridSettingsConfigProperties settingsConfig;
  const Property& pBottomRotateText = pGridSettings.getSubProperty(settingsConfig.getBottomRotateText());
  const Property& pFont = pGridSettings.getSubProperty(settingsConfig.getFont());

  bool bRotate = te::layout::Property::GetValueAs<bool>(pBottomRotateText);
  const Font& font = te::layout::Property::GetValueAs<Font>(pFont);

  QFont qFont = ItemUtils::convertToQfont(font);

  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotate;
  }

  std::vector<TextPosition>::iterator it = m_bottomTexts.begin();


  const Property& pSuperscript = this->getProperty(settingsConfig.getSuperscriptText());
  bool useSuperScript = te::layout::Property::GetValueAs<bool>(pSuperscript);

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
  */
}

void te::layout::GridMapItem::calculateLeftTexts()
{
  /*
  const Property& pGridSettings = this->getProperty("GridSettings");

  GridSettingsConfigProperties settingsConfig;
  const Property& pLeftRotateText = pGridSettings.getSubProperty(settingsConfig.getLeftRotateText());
  const Property& pFont = pGridSettings.getSubProperty(settingsConfig.getFont());

  bool bRotate = te::layout::Property::GetValueAs<bool>(pLeftRotateText);
  const Font& font = te::layout::Property::GetValueAs<Font>(pFont);

  QFont qFont = ItemUtils::convertToQfont(font);

  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotate;
  }

  double width = 0;
  double height = 0;
  
  std::vector<TextPosition>::iterator it = m_leftTexts.begin();

  const Property& pSuperscript = this->getProperty(settingsConfig.getSuperscriptText());
  bool useSuperScript = te::layout::Property::GetValueAs<bool>(pSuperscript);


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
  */
}

void te::layout::GridMapItem::calculateRightTexts()
{
  /*
  const Property& pGridSettings = this->getProperty("GridSettings");

  GridSettingsConfigProperties settingsConfig;
  const Property& pRightRotateText = pGridSettings.getSubProperty(settingsConfig.getRightRotateText());
  const Property& pFont = pGridSettings.getSubProperty(settingsConfig.getFont());

  bool bRotate = te::layout::Property::GetValueAs<bool>(pRightRotateText);
  const Font& font = te::layout::Property::GetValueAs<Font>(pFont);

  QFont qFont = ItemUtils::convertToQfont(font);

  const Property& pSuperscript = this->getProperty(settingsConfig.getSuperscriptText());
  bool useSuperScript = te::layout::Property::GetValueAs<bool>(pSuperscript);

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
  */
}

QRectF te::layout::GridMapItem::boundingRect() const
{
  if (m_currentAction == RESIZE_ACTION)
  {
    return AbstractItem::boundingRect();
  }

  if(m_boundingBox.isValid())
  {
    return QRectF(m_boundingBox.getLowerLeftX(), m_boundingBox.getLowerLeftY(), m_boundingBox.getWidth(), m_boundingBox.getHeight());
  }
  return AbstractItem::boundingRect();
}

void te::layout::GridMapItem::calculateCrossLines()
{
  /*
  const Property& pGridSettings = this->getProperty("GridSettings");

  GridSettingsConfigProperties settingsConfig;

  const Property& pCrossOffset = pGridSettings.getSubProperty(settingsConfig.getCrossOffset());
  double crossOffSet = te::layout::Property::GetValueAs<double>(pCrossOffset);

  
  const Property& pUseBouderIntersection = pGridSettings.getSubProperty(settingsConfig.getBouderIntersections());
  bool useBouderItersection = te::layout::Property::GetValueAs<bool>(pUseBouderIntersection);

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
  */
}


void te::layout::GridMapItem::calculateBoldersSegments(double crossOffSet){

  const Property& pWidth = this->getProperty("width");
  const Property& pHeight = this->getProperty("height");

  double width = te::layout::Property::GetValueAs<double>(pWidth);
  double height = te::layout::Property::GetValueAs<double>(pHeight);

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

te::gm::LineString te::layout::GridMapItem::roundLineString(const te::gm::LineString& lines)
{

  double x1 = Utils::round(lines.getPointN(0)->getX(), 6);
  double y1 = Utils::round(lines.getPointN(0)->getY(), 6);
  

  double x2 = Utils::round(lines.getPointN(1)->getX(), 6);
  double y2 = Utils::round(lines.getPointN(1)->getY(), 6);

  te::gm::LineString line(2, te::gm::LineStringType);


  line.setPoint(0, x1, y1);
  line.setPoint(1, x2, y2);
  return line;

}

bool te::layout::GridMapItem::checkBolderIntersection(const te::gm::LineString& bolderLine, const te::gm::LineString& gridLine, te::gm::Point& intersectionPoint)
{

  std::auto_ptr<te::gm::Geometry> interGeometry;
  te::gm::Point* interPoint = 0;

  te::gm::LineString bLine = roundLineString(bolderLine);
  te::gm::LineString gLine = roundLineString(gridLine);


  try
  {
    interGeometry.reset(bLine.intersection(&gLine));
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
  return true;
  /*
  bool result = false;

  const Property& pGridSettings = this->getProperty("GridSettings");

  GridSettingsConfigProperties settingsConfig;

  const Property& pWidth = this->getProperty("width");
  const Property& pHeight = this->getProperty("height");
  const Property& pCrossOffset = pGridSettings.getSubProperty(settingsConfig.getCrossOffset());

  double width = te::layout::Property::GetValueAs<double>(pWidth);
  double height = te::layout::Property::GetValueAs<double>(pHeight);
  double crossOffSet = te::layout::Property::GetValueAs<double>(pCrossOffset);

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
  */
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

  const Property& pGridSettings = this->getProperty("GridSettings");

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

bool te::layout::GridMapItem::validateHrzGap(const te::gm::Envelope& geoBox, double gap){

  double result = geoBox.getWidth()/gap;

  if (result > 30.0)
  {
    return false;
  }
  
  return true;
}

bool te::layout::GridMapItem::validateVrtGap(const te::gm::Envelope& geoBox, double gap){
  
  double result = geoBox.getHeight() / gap;

  if (result > 30.0)
  {
    return false;
  }

  return true;

}