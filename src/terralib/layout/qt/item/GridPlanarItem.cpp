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
  \file GridPlanarItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridPlanarItem.h"

#include "../core/ItemUtils.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/property/PlanarGridSettingsConfigProperties.h"
#include "../../core/WorldTransformer.h"
#include "../../core/Utils.h"
#include "../core/Scene.h"

te::layout::GridPlanarItem::GridPlanarItem(AbstractItemController* controller, bool invertedMatrix)
  : GridMapItem(controller, invertedMatrix)
{

}

te::layout::GridPlanarItem::~GridPlanarItem()
{

}

void te::layout::GridPlanarItem::drawGrid(QPainter* painter)
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pStyle = pGridSettings.containsSubProperty(settingsConfig.getStyle());
  
  const std::string& style = pStyle.getOptionByCurrentChoice().toString();
  EnumType* currentStyle = Enums::getInstance().getEnumGridStyleType()->getEnum(style);
  if (currentStyle != 0)
  {
    currentStyle = Enums::getInstance().getEnumGridStyleType()->searchLabel(style);
  }


  const Property& pTextFontFamily = pGridSettings.containsSubProperty(settingsConfig.getFontText());
  Font txtFont = pTextFontFamily.getValue().toFont();

  

  ItemUtils::ConfigurePainterForTexts(painter, txtFont);
  
  EnumGridStyleType* gridStyle = Enums::getInstance().getEnumGridStyleType();
  if(!gridStyle)
  {
    return;
  }
  
  if(currentStyle == gridStyle->getStyleContinuous())
  {
    drawContinuousLines(painter);
  }
  else if(currentStyle == gridStyle->getStyleCross())
  {
    drawCrossLines(painter);
  }
}

void te::layout::GridPlanarItem::calculateGrid()
{
  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pPlanarBox = m_controller->getProperty("planar_box");
  const Property& pWidth = m_controller->getProperty("width");
  const Property& pHeight = m_controller->getProperty("height");
  const Property& pFrameThickness = m_controller->getProperty("frame_thickness");

  const te::gm::Envelope& planarBox = pPlanarBox.getValue().toEnvelope();
  double width = pWidth.getValue().toDouble();
  double height = pHeight.getValue().toDouble();
  double frameThickness = pFrameThickness.getValue().toDouble();

  te::gm::Envelope referenceBoxMM(0, 0, width, height);

  clear();

  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;
  
  calculateVertical(planarBox, referenceBoxMM);
  calculateHorizontal(planarBox, referenceBoxMM);

  m_boundingBox = te::gm::Envelope(m_boundingBox.getLowerLeftX() - frameThickness, m_boundingBox.getLowerLeftY() - frameThickness, m_boundingBox.getUpperRightX() + frameThickness, m_boundingBox.getUpperRightY() + frameThickness);

  prepareGeometryChange();
}

void te::layout::GridPlanarItem::calculateVertical( const te::gm::Envelope& geoBox, const te::gm::Envelope& boxMM )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pVerticalGap = pGridSettings.containsSubProperty(settingsConfig.getLneVrtGap());
  const Property& pVerticalDisplacement = pGridSettings.containsSubProperty(settingsConfig.getLneVrtDisplacement());
  const Property& pUnit = pGridSettings.containsSubProperty(settingsConfig.getUnit());
  const Property& pLeftRotate = pGridSettings.containsSubProperty(settingsConfig.getLeftRotateText());
  const Property& pRightRotate = pGridSettings.containsSubProperty(settingsConfig.getRightRotateText());

  const Property& pTextFontFamily = pGridSettings.containsSubProperty(settingsConfig.getFontText());
  Font txtFont = pTextFontFamily.getValue().toFont();
  
  double verticalGap = pVerticalGap.getValue().toDouble();
  double verticalDisplacement = pVerticalDisplacement.getValue().toDouble();
  LayoutUnit unit = (LayoutUnit)pUnit.getValue().toInt();
  bool bLeftRotate = pLeftRotate.getValue().toBool();
  bool bRightRotate = pRightRotate.getValue().toBool();

  if (this->scene() == 0)
  {
    return;
  }

  Utils utils = ((Scene*) this->scene())->getUtils();
  ItemUtils itemUtils = ((Scene*) this->scene())->getItemUtils();

  int dpi = ((Scene*) this->scene())->getContext().getDpiX();
  
  // Draw a horizontal line and the y coordinate change(vertical)

  WorldTransformer transf = utils.getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  double y1 = initVerticalLines(geoBox);
  
  m_boundingBox.Union(boxMM);
  


  QFont ft =  itemUtils.convertToQfont(txtFont);
  
  for( ; y1 <= geoBox.getUpperRightY() ; y1 += verticalGap)
  {
    if(y1 < geoBox.getLowerLeftY())
      continue;

    double llx = 0;
    double urx = 0;
    double y = 0;
    transf.system1Tosystem2(geoBox.getLowerLeftX(), y1, llx, y);
    transf.system1Tosystem2(geoBox.getUpperRightX(), y1, urx, y);
    
    QLineF line(llx, y, urx, y);
    m_horizontalLines.push_back(line);

    double number = y1 / (double)unit;
    QString convert = QString::number(number, 'f', 0);

    QPainterPath textObject = itemUtils.textToVector(convert, ft, dpi, QPointF(), 0);

    QRectF rectF(textObject.boundingRect());

    calculateLeft(line, rectF, convert, bLeftRotate, verticalDisplacement);
    calculateRight(line, rectF, convert, bRightRotate, verticalDisplacement);
  }
}

void te::layout::GridPlanarItem::calculateHorizontal( const te::gm::Envelope& geoBox, const te::gm::Envelope& boxMM )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return;

  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pTextFontFamily = pGridSettings.containsSubProperty(settingsConfig.getFontText());
  const Property& pHorizontalGap = pGridSettings.containsSubProperty(settingsConfig.getLneHrzGap());
  const Property& pHorizontalDisplacement = pGridSettings.containsSubProperty(settingsConfig.getLneHrzDisplacement());
  const Property& pUnit = pGridSettings.containsSubProperty(settingsConfig.getUnit());
  const Property& pTopRotate = pGridSettings.containsSubProperty(settingsConfig.getTopRotateText());
  const Property& pBottomRotate = pGridSettings.containsSubProperty(settingsConfig.getBottomRotateText());

  Font txtFont = pTextFontFamily.getValue().toFont();
 

  double horizontalGap = pHorizontalGap.getValue().toDouble();
  double horizontalDisplacement = pHorizontalDisplacement.getValue().toDouble();
  LayoutUnit unit = (LayoutUnit)pUnit.getValue().toInt();
  bool bTopRotate = pTopRotate.getValue().toBool();
  bool bBottomRotate = pBottomRotate.getValue().toBool();

  if (this->scene() == 0)
  {
    return;
  }

  Utils utils = ((Scene*) this->scene())->getUtils();
  ItemUtils itemUtils = ((Scene*) this->scene())->getItemUtils();

  int dpi = ((Scene*) this->scene())->getContext().getDpiX();

  // Draw a vertical line and the x coordinate change(horizontal)

  WorldTransformer transf = utils.getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  double x1 = initHorizontalLines(geoBox);
  
  m_boundingBox.Union(boxMM);
  
  QFont ft = itemUtils.convertToQfont(txtFont);

  for( ; x1 <= geoBox.getUpperRightX() ; x1 += horizontalGap)
  {
    if(x1 < geoBox.getLowerLeftX())
      continue;

    double lly = 0;
    double ury = 0;
    double x = 0;
    transf.system1Tosystem2(x1, geoBox.getLowerLeftY(), x, lly);
    transf.system1Tosystem2(x1, geoBox.getUpperRightY(), x, ury);

    te::gm::Envelope newBox(x, lly, x, ury);

    if(lly > ury)
    {
      double ycopy = lly;
      lly = ury;
      ury = ycopy;
    }

    QLineF line(x, lly, x, ury);
    m_verticalLines.push_back(line);

    double number = x1 / (double)unit;
    QString convert = QString::number(number, 'f', 0);

    QPainterPath textObject = itemUtils.textToVector(convert, ft, dpi, QPointF(), 0);
    QRectF rectF(textObject.boundingRect());
    
    calculateTop(line, rectF, convert, bTopRotate, horizontalDisplacement);
    calculateBottom(line, rectF, convert, bBottomRotate, horizontalDisplacement);
  }
}

double te::layout::GridPlanarItem::initVerticalLines( const te::gm::Envelope& geoBox )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return 0;

  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pInitialGridPointY = pGridSettings.containsSubProperty(settingsConfig.getInitialGridPointY());
  const Property& pVerticalGap = pGridSettings.containsSubProperty(settingsConfig.getLneVrtGap());

  double initialGridPointY = pInitialGridPointY.getValue().toDouble();
  double verticalGap = pVerticalGap.getValue().toDouble();

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

double te::layout::GridPlanarItem::initHorizontalLines( const te::gm::Envelope& geoBox )
{
  const Property& pGridSettings = m_controller->getProperty("GridSettings");
  if (pGridSettings.isNull() || pGridSettings.getSubProperty().empty())
    return 0;

  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pInitialGridPointX = pGridSettings.containsSubProperty(settingsConfig.getInitialGridPointX());
  const Property& pHorizontalGap = pGridSettings.containsSubProperty(settingsConfig.getLneHrzGap());

  double initialGridPointX = pInitialGridPointX.getValue().toDouble();
  double horizontalGap = pHorizontalGap.getValue().toDouble();

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
