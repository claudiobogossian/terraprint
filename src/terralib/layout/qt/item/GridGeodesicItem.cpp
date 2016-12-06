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
  \file GridGeodesicItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridGeodesicItem.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../core/ItemUtils.h"
#include "../../core/WorldTransformer.h"
#include "../../core/enum/EnumTextFormatType.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/property/GeodesicGridSettingsConfigProperties.h"
#include "terralib/maptools/Utils.h"
#include "terralib/geometry/Utils.h"
#include "../core/Scene.h"

te::layout::GridGeodesicItem::GridGeodesicItem(AbstractItemController* controller)
  : GridMapItem(controller)
{  
  
}

te::layout::GridGeodesicItem::~GridGeodesicItem()
{

}

void te::layout::GridGeodesicItem::calculateGrid()
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");

  GeodesicGridSettingsConfigProperties settingsConfig;

  const Property& pGeographicBox = m_controller->getProperty("geographic_box");
  const Property& pWidth = m_controller->getProperty("width");
  const Property& pHeight = m_controller->getProperty("height");
  const Property& pStyle = pGridSettings.getSubProperty(settingsConfig.getStyle());
  const Property& pFrameThickness = m_controller->getProperty("frame_thickness");

  const te::gm::Envelope& geographicBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pGeographicBox);
  double width = te::layout::Property::GetValueAs<double>(pWidth);
  double height = te::layout::Property::GetValueAs<double>(pHeight);
  const std::string& style = pStyle.getOptionByCurrentChoice().toString();
  double frameThickness = te::layout::Property::GetValueAs<double>(pFrameThickness);

  te::gm::Envelope newBoxMM(0, 0, width, height);

  clear();

  if (this->scene() == 0)
  {
    return;
  }

  Utils utils = ((Scene*) this->scene())->getUtils();

  // Box necessario para desenhar a curvatura
  te::gm::Envelope planarBox = geographicBox;
  int zone = te::map::CalculatePlanarZone(geographicBox);
  if (zone < 0 || zone > 60)
  {
    return;
  }

  utils.remapToPlanar(&planarBox, zone);

  const Property& pVerticalGap = pGridSettings.getSubProperty(settingsConfig.getLneVrtGap());
  double verticalGap = te::layout::Property::GetValueAs<double>(pVerticalGap);

  if (validateVrtGap(geographicBox, verticalGap))
  {
    calculateVertical(geographicBox, planarBox, newBoxMM);
  }

  const Property& pHorizontalGap = pGridSettings.getSubProperty(settingsConfig.getLneHrzGap());
  double horizontalGap = te::layout::Property::GetValueAs<double>(pHorizontalGap);

  if (validateHrzGap(geographicBox, horizontalGap))
  {
    calculateHorizontal(geographicBox, planarBox, newBoxMM);
  }

  EnumType* currentStyle = Enums::getInstance().getEnumGridStyleType()->getEnum(style);

  EnumGridStyleType gridStyleType;

  if (currentStyle)
  {
    if (currentStyle->getName() == gridStyleType.getStyleCross()->getName())
    {
      clearLines();
      calculateCrossLines();
    }

    if (currentStyle->getName() == gridStyleType.getStyleContinuous()->getName())
    {
      clearLines();
      addGridLinesToPath();
    }
  }

  prepareGeometryChange();
  m_boundingBox = te::gm::Envelope(m_boundingBox.getLowerLeftX() - frameThickness, m_boundingBox.getLowerLeftY() - frameThickness, m_boundingBox.getUpperRightX() + frameThickness, m_boundingBox.getUpperRightY() + frameThickness);
}

double te::layout::GridGeodesicItem::initVerticalLines( const te::gm::Envelope& geoBox )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");

  GeodesicGridSettingsConfigProperties settingsConfig;

  const Property& pInitialGridPointY = pGridSettings.getSubProperty(settingsConfig.getInitialGridPointY());
  const Property& pVerticalGap = pGridSettings.getSubProperty(settingsConfig.getLneVrtGap());

  double initialGridPointY = te::layout::Property::GetValueAs<double>(pInitialGridPointY);
  double verticalGap = te::layout::Property::GetValueAs<double>(pVerticalGap);

  double yInit = initialGridPointY;
  if(yInit < geoBox.getLowerLeftY())
  {
    double dify = geoBox.getLowerLeftY() - yInit;
    int nParts = (int)(dify/verticalGap);
    if(nParts == 0)
    {
      yInit = initialGridPointY;
    }
    else
    {
      yInit = yInit + (nParts * verticalGap);
    }
  }
  return yInit;
}

double te::layout::GridGeodesicItem::initHorizontalLines( const te::gm::Envelope& geoBox )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");

  GeodesicGridSettingsConfigProperties settingsConfig;

  const Property& pInitialGridPointX = pGridSettings.getSubProperty(settingsConfig.getInitialGridPointX());
  const Property& pHorizontalGap = pGridSettings.getSubProperty(settingsConfig.getLneHrzGap());

  double initialGridPointX = te::layout::Property::GetValueAs<double>(pInitialGridPointX);
  double horizontalGap = te::layout::Property::GetValueAs<double>(pHorizontalGap);

  double xInit = initialGridPointX;

  if(xInit < geoBox.getLowerLeftX())
  {
    double difx = geoBox.getLowerLeftX() - xInit;
    int nParts = (int)(difx/horizontalGap);
    if(nParts == 0)
    {
      xInit = initialGridPointX;
    }
    else
    {
      xInit = xInit + (nParts * horizontalGap);
    }
  }
  return xInit;
}

void te::layout::GridGeodesicItem::calculateVertical(const te::gm::Envelope& geoBox, const te::gm::Envelope& planarBox, const te::gm::Envelope& boxMM )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");

  GeodesicGridSettingsConfigProperties settingsConfig;

  const Property& pTextFontFamily = pGridSettings.getSubProperty(settingsConfig.getFont());

  const Property& pVerticalGap = pGridSettings.getSubProperty(settingsConfig.getLneVrtGap());
  const Property& pHorizontalGap = pGridSettings.getSubProperty(settingsConfig.getLneHrzGap());
  const Property& pShowDegreesText = pGridSettings.getSubProperty(settingsConfig.getDegreesText());
  const Property& pShowMinutesText = pGridSettings.getSubProperty(settingsConfig.getMinutesText());
  const Property& pShowSecondsText = pGridSettings.getSubProperty(settingsConfig.getSecondsText());
  const Property& pTextFormat = pGridSettings.getSubProperty(settingsConfig.getTextFormat());
  const Property& pHorizontalDisplacement = pGridSettings.getSubProperty(settingsConfig.getLneHrzDisplacement());
  const Property& pVerticalDisplacement = pGridSettings.getSubProperty(settingsConfig.getLneVrtDisplacement());
  const Property& pLeftRotate = pGridSettings.getSubProperty(settingsConfig.getLeftRotateText());
  const Property& pRightRotate = pGridSettings.getSubProperty(settingsConfig.getRightRotateText());

  const Property& pSecPrecisionText = pGridSettings.getSubProperty(settingsConfig.getSecondsPrecisionText());
  
  double verticalGap = te::layout::Property::GetValueAs<double>(pVerticalGap);
  double horizontalGap = te::layout::Property::GetValueAs<double>(pHorizontalGap);
  bool showDegreesText = te::layout::Property::GetValueAs<bool>(pShowDegreesText);
  bool showMinutesText = te::layout::Property::GetValueAs<bool>(pShowMinutesText);
  bool showSecondsText = te::layout::Property::GetValueAs<bool>(pShowSecondsText);
  std::string textFormat = pTextFormat.getOptionByCurrentChoice().toString();
  double horizontalDisplacement = te::layout::Property::GetValueAs<double>(pHorizontalDisplacement);
  double verticalDisplacement = te::layout::Property::GetValueAs<double>(pVerticalDisplacement);
  bool bLeftRotate = te::layout::Property::GetValueAs<bool>(pLeftRotate);
  bool bRightRotate = te::layout::Property::GetValueAs<bool>(pRightRotate);
  Font txtFont = te::layout::Property::GetValueAs<Font>(pTextFontFamily);

  int secPrecision = te::layout::Property::GetValueAs<int>(pSecPrecisionText);

  // Draw a horizontal line and the y coordinate change(vertical)
  Utils utils = ((Scene*) this->scene())->getUtils();

  WorldTransformer transf = utils.getTransformGeo(planarBox, boxMM);
  transf.setMirroring(false);

  int zone = te::map::CalculatePlanarZone(geoBox);
  if(zone < 0 || zone > 60)
  {
    return;
  }

  std::string fontName = txtFont.getFamily();
  QString qFontName = ItemUtils::convert2QString(fontName);

  QFont ft(qFontName, txtFont.getPointSize());

  double y1 = initVerticalLines(geoBox);
  double y2 = geoBox.getUpperRightY();

  y1 -= 3 * verticalGap;
  y2 += 3 * verticalGap;

  for( ; y1 <= y2 ; y1 += verticalGap)
  {
    te::gm::Envelope env(geoBox.getLowerLeftX() - 3 * horizontalGap, y1, geoBox.getUpperRightX() + 3 * horizontalGap, y1);

    te::gm::LinearRing* line = 0;
    line = utils.addCoordsInX(env, y1, verticalGap);
    
    // Line curvature: of latlong to planar;
    // Draw line: planar to mm
    utils.remapToPlanar(line, zone);
    utils.convertToMillimeter(transf, line);

    //here we clip the line using the boundaries of the item in MM
    te::gm::Geometry* rectPolygon = te::gm::GetGeomFromEnvelope(&boxMM, line->getSRID());
    te::gm::Geometry* clippedGeometry = line->intersection(rectPolygon);
    if (clippedGeometry == 0 || clippedGeometry->isValid() == false)
    {
      delete rectPolygon;
      delete clippedGeometry;
      continue;
    }

    te::gm::LineString* lineString = dynamic_cast<te::gm::LineString*>(clippedGeometry);
    if (lineString == 0)
    {
      delete rectPolygon;
      delete clippedGeometry;
      continue;
    }

    size_t numCoords = lineString->size();
    te::gm::Point* firstPoint = lineString->getPointN(0);
    te::gm::Point* lastPoint = lineString->getPointN(numCoords - 1);

    te::gm::LineString leftBorderLine(2, te::gm::LineStringType);
    leftBorderLine.setPoint(0, boxMM.getLowerLeftX(), boxMM.getLowerLeftY());
    leftBorderLine.setPoint(1, boxMM.getLowerLeftX(), boxMM.getUpperRightY());

    te::gm::LineString rightBorderLine(2, te::gm::LineStringType);
    rightBorderLine.setPoint(0, boxMM.getUpperRightX(), boxMM.getLowerLeftY());
    rightBorderLine.setPoint(1, boxMM.getUpperRightX(), boxMM.getUpperRightY());

    m_horizontalLines.push_back(te::gm::LineString(*lineString));

    EnumTextFormatType textFormatEnum;
    EnumType* currentTextFormat = textFormatEnum.getEnum(textFormat);

    std::string text = "";

    if (currentTextFormat)
    {
      if (currentTextFormat->getName() == textFormatEnum.getDefaultFormat()->getName())
      {
        text = utils.convertDecimalToDegree(y1, showDegreesText, showMinutesText, showSecondsText, secPrecision);
      }
      else if (currentTextFormat->getName() == textFormatEnum.getANPFormat()->getName())
      {
        text = utils.convertDecimalToDegreeANP(y1, showDegreesText, showMinutesText, showSecondsText, secPrecision);
      }
    }

    QString qText = ItemUtils::convert2QString(text);
    
    QPainterPath textObject = ItemUtils::textToVector(qText, ft, QPointF(), 0);

    QRectF rectF(textObject.boundingRect());


    //as the grid lines an be curved, texts must only de drawn in the cases that the grid line reaches the top or the botton of the item bounding rect
    if (lineString->intersects(&leftBorderLine) == true)
    {
      calculateLeft(QPointF(firstPoint->getX(), firstPoint->getY()), rectF, qText, bLeftRotate, verticalDisplacement);
    }
    if (lineString->intersects(&rightBorderLine) == true)
    {
      calculateRight(QPointF(lastPoint->getX(), lastPoint->getY()), rectF, qText, bRightRotate, verticalDisplacement);
    }

    if(line)
    {
      delete line;
      line = 0;
    }
  }
}

void te::layout::GridGeodesicItem::calculateHorizontal( const te::gm::Envelope& geoBox, const te::gm::Envelope& planarBox, const te::gm::Envelope& boxMM )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");

  GeodesicGridSettingsConfigProperties settingsConfig;

  const Property& pHorizontalGap = pGridSettings.getSubProperty(settingsConfig.getLneHrzGap());
  const Property& pVerticalGap = pGridSettings.getSubProperty(settingsConfig.getLneVrtGap());
  const Property& pShowDegreesText = pGridSettings.getSubProperty(settingsConfig.getDegreesText());
  const Property& pShowMinutesText = pGridSettings.getSubProperty(settingsConfig.getMinutesText());
  const Property& pShowSecondsText = pGridSettings.getSubProperty(settingsConfig.getSecondsText());
  const Property& pTextFormat = pGridSettings.getSubProperty(settingsConfig.getTextFormat());
  const Property& pVerticalDisplacement = pGridSettings.getSubProperty(settingsConfig.getLneVrtDisplacement());
  const Property& pHorizontalDisplacement = pGridSettings.getSubProperty(settingsConfig.getLneHrzDisplacement());
  const Property& pTopRotate = pGridSettings.getSubProperty(settingsConfig.getTopRotateText());
  const Property& pBottomRotate = pGridSettings.getSubProperty(settingsConfig.getBottomRotateText());
  const Property& pTextFontFamily = pGridSettings.getSubProperty(settingsConfig.getFont());
  const Property& pSecPrecisionText = pGridSettings.getSubProperty(settingsConfig.getSecondsPrecisionText());
  
  Font txtFont = te::layout::Property::GetValueAs<Font>(pTextFontFamily);
//  std::string fontFamily = pFontFamily.getValue().toString();
//  int textPointSize = pTextPointSize.getValue().toInt();
  double horizontalGap = te::layout::Property::GetValueAs<double>(pHorizontalGap);
  double verticalGap = te::layout::Property::GetValueAs<double>(pVerticalGap);
  bool showDegreesText = te::layout::Property::GetValueAs<bool>(pShowDegreesText);
  bool showMinutesText = te::layout::Property::GetValueAs<bool>(pShowMinutesText);
  bool showSecondsText = te::layout::Property::GetValueAs<bool>(pShowSecondsText);
  std::string textFormat = pTextFormat.getOptionByCurrentChoice().toString();
  double verticalDisplacement = te::layout::Property::GetValueAs<double>(pVerticalDisplacement);
  double horizontalDisplacement = te::layout::Property::GetValueAs<double>(pHorizontalDisplacement);
  bool bTopRotate = te::layout::Property::GetValueAs<bool>(pTopRotate);
  bool bBottomRotate = te::layout::Property::GetValueAs<bool>(pBottomRotate);

  int secPrecision = te::layout::Property::GetValueAs<int>(pSecPrecisionText);

  // Draw a vertical line and the x coordinate change(horizontal)

  Utils utils = ((Scene*) this->scene())->getUtils();

  WorldTransformer transf = utils.getTransformGeo(planarBox, boxMM);
  transf.setMirroring(false);

  int zone = te::map::CalculatePlanarZone(geoBox);
  if(zone < 0 || zone > 60)
  {
    return;
  }

  std::string fontName = txtFont.getFamily();
  QString qFontName = ItemUtils::convert2QString(fontName);

  QFont ft(qFontName, txtFont.getPointSize());

  //as the geographic grid comes from the result of a reprojection, the geographic box sometimes can be smaller than the planar box, maninly when the distance from equator gets higher.
  //when this happens, these calculated lines do not reach the boundaries of the item, and consequently the texts are not drawn in the correct position
  //to fix this, we calculate a grid bigger than we need, and then we clip the lines by making an intertection operation with the item
  double x1 = initHorizontalLines(geoBox);
  double x2 = geoBox.getUpperRightX();

  x1 -= 3 * horizontalGap;
  x2 += 3 * horizontalGap;

  for( ; x1 <= x2 ; x1 += horizontalGap)
  {
    te::gm::Envelope env(x1, geoBox.getLowerLeftY() - 3 * verticalGap, x1, geoBox.getUpperRightY() + 3 * verticalGap);
    te::gm::LinearRing* line = 0;
    line = utils.addCoordsInY(env, x1, horizontalGap);

    // Line curvature: of latlong to planar;
    // Draw line: planar to mm
    utils.remapToPlanar(line, zone);
    utils.convertToMillimeter(transf, line);

    //here we clip the line using the boundaries of the item in MM
    te::gm::Geometry* rectPolygon = te::gm::GetGeomFromEnvelope(&boxMM, line->getSRID());
    te::gm::Geometry* clippedGeometry = line->intersection(rectPolygon);
    if (clippedGeometry == 0 || clippedGeometry->isValid() == false)
    {
      delete rectPolygon;
      delete clippedGeometry;
      continue;
    }

    te::gm::LineString* lineString = dynamic_cast<te::gm::LineString*>(clippedGeometry);
    if (lineString == 0)
    {
      delete rectPolygon;
      delete clippedGeometry;
      continue;
    }

    size_t numCoords = lineString->size();
    te::gm::Point* firstPoint = lineString->getPointN(0);
    te::gm::Point* lastPoint = lineString->getPointN(numCoords - 1);

    te::gm::LineString topBorderLine(2, te::gm::LineStringType);
    topBorderLine.setPoint(0, boxMM.getLowerLeftX(), boxMM.getUpperRightY());
    topBorderLine.setPoint(1, boxMM.getUpperRightX(), boxMM.getUpperRightY());

    te::gm::LineString bottomBorderLine(2, te::gm::LineStringType);
    bottomBorderLine.setPoint(0, boxMM.getLowerLeftX(), boxMM.getLowerLeftY());
    bottomBorderLine.setPoint(1, boxMM.getUpperRightX(), boxMM.getLowerLeftY());

    m_verticalLines.push_back(te::gm::LineString(*lineString));

    EnumTextFormatType textFormatEnum;
    EnumType* currentTextFormat = textFormatEnum.getEnum(textFormat);

    std::string text = "";

    if (currentTextFormat)
    {
      if (currentTextFormat->getName() == textFormatEnum.getDefaultFormat()->getName())
      {
        text = utils.convertDecimalToDegree(x1, showDegreesText, showMinutesText, showSecondsText, secPrecision);
      }
      else if (currentTextFormat->getName() == textFormatEnum.getANPFormat()->getName())
      {
        text = utils.convertDecimalToDegreeANP(x1, showDegreesText, showMinutesText, showSecondsText, secPrecision);
      }
    }

    QString qText = ItemUtils::convert2QString(text);

    QPainterPath textObject = ItemUtils::textToVector(qText, ft, QPointF(), 0);
    QRectF rectF(textObject.boundingRect());

    //as the grid lines an be curved, texts must only de drawn in the cases that the grid line reaches the top or the botton of the item bounding rect
    if (lineString->intersects(&bottomBorderLine) == true)
    {
      calculateBottom(QPointF(firstPoint->getX(), firstPoint->getY()), rectF, qText, bBottomRotate, horizontalDisplacement);
    }
    if (lineString->intersects(&topBorderLine) == true)
    {
      calculateTop(QPointF(lastPoint->getX(), lastPoint->getY()), rectF, qText, bTopRotate, horizontalDisplacement);
    }

    if(line)
    {
      delete line;
      line = 0;
    }

    delete rectPolygon;
    delete clippedGeometry;
  }
}
