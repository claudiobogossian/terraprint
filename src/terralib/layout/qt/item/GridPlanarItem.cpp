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

#include "GridPlanarController.h"
#include "../core/ItemUtils.h"
#include "../core/Scene.h"
#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/property/PlanarGridSettingsConfigProperties.h"
#include "../../core/WorldTransformer.h"
#include "../../core/Utils.h"
#include "../../item/GridPlanarModel.h"

te::layout::GridPlanarItem::GridPlanarItem()
  : GridMapItem()
{

}

te::layout::GridPlanarItem::~GridPlanarItem()
{

}

te::layout::AbstractItemModel* te::layout::GridPlanarItem::createModel() const
{
  return new GridPlanarModel();
}

te::layout::AbstractItemController* te::layout::GridPlanarItem::createController() const
{
  AbstractItemModel* model = createModel();
  return new GridPlanarController(model, (AbstractItemView*)this);
}

void te::layout::GridPlanarItem::calculateGrid()
{
  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pPlanarBox = this->getProperty("planar_box");
  const Property& pWidth = this->getProperty("width");
  const Property& pHeight = this->getProperty("height");
  const Property& pFrameThickness = this->getProperty("frame_thickness");

  const Property& pGridSettings = this->getProperty("GridSettings");

  const Property& pInitialX = pGridSettings.getSubProperty(settingsConfig.getInitialGridPointX());
  const Property& pInitialY = pGridSettings.getSubProperty(settingsConfig.getInitialGridPointY());

  double initalX = te::layout::Property::GetValueAs<double>(pInitialX);
  double initalY = te::layout::Property::GetValueAs<double>(pInitialY);

  const te::gm::Envelope& planarBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pPlanarBox);
  double width = te::layout::Property::GetValueAs<double>(pWidth);
  double height = te::layout::Property::GetValueAs<double>(pHeight);
  double frameThickness = te::layout::Property::GetValueAs<double>(pFrameThickness);

  te::gm::Envelope referenceBoxMM(0, 0, width, height);

  clear();
  
  const Property& pVerticalGap = pGridSettings.getSubProperty(settingsConfig.getLneVrtGap());

  double verticalGap = te::layout::Property::GetValueAs<double>(pVerticalGap);

  const Property& pHorizontalGap = pGridSettings.getSubProperty(settingsConfig.getLneHrzGap());

  double horizontalGap = te::layout::Property::GetValueAs<double>(pHorizontalGap);

  if (validateVrtGap(planarBox, verticalGap))
  {
    calculateVertical(planarBox, referenceBoxMM);
  }

  if (validateHrzGap(planarBox, horizontalGap))
  {
    calculateHorizontal(planarBox, referenceBoxMM);
  }

  const Property& pStyle = pGridSettings.getSubProperty(settingsConfig.getStyle());

  const std::string& style = pStyle.getOptionByCurrentChoice().toString();
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


void te::layout::GridPlanarItem::calculateVertical( const te::gm::Envelope& geoBox, const te::gm::Envelope& boxMM )
{
  const Property& pGridSettings = this->getProperty("GridSettings");

  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pVerticalGap = pGridSettings.getSubProperty(settingsConfig.getLneVrtGap());
  const Property& pVerticalDisplacement = pGridSettings.getSubProperty(settingsConfig.getLneVrtDisplacement());
  const Property& pUnit = pGridSettings.getSubProperty(settingsConfig.getUnit());
  const Property& pLeftRotate = pGridSettings.getSubProperty(settingsConfig.getLeftRotateText());
  const Property& pRightRotate = pGridSettings.getSubProperty(settingsConfig.getRightRotateText());

  const Property& pTextFontFamily = pGridSettings.getSubProperty(settingsConfig.getFont());
  Font txtFont = te::layout::Property::GetValueAs<Font>(pTextFontFamily);
  
  double verticalGap = te::layout::Property::GetValueAs<double>(pVerticalGap);
  double verticalDisplacement = te::layout::Property::GetValueAs<double>(pVerticalDisplacement);
  
  string strUnit = pUnit.getOptionByCurrentChoice().toString();
  double unitV = getUnit(strUnit);

  bool bLeftRotate = te::layout::Property::GetValueAs<bool>(pLeftRotate);
  bool bRightRotate = te::layout::Property::GetValueAs<bool>(pRightRotate);

  if (this->scene() == 0)
  {
    return;
  }

  Utils utils = ((Scene*) this->scene())->getUtils();
  
  // Draw a horizontal line and the y coordinate change(vertical)
  WorldTransformer transf = utils.getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  double y1 = initVerticalLines(geoBox);
  
  m_boundingBox.Union(boxMM);

  QFont ft = ItemUtils::convertToQfont(txtFont);
  



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


    te::gm::LineString lineString(2, te::gm::LineStringType);
    lineString.setPoint(0, llx, y);
    lineString.setPoint(1, urx, y);

    m_horizontalLines.push_back(lineString);

    m_gridLines.addPath(ItemUtils::lineToQPath(lineString));

    double number = y1 / unitV;
    QString convert = QString::number(number, 'f', 0);

    std::string text = convert.toStdString();

    const Property& pSuperscript = this->getProperty(settingsConfig.getSuperscriptText());
    bool useSuperScript = te::layout::Property::GetValueAs<bool>(pSuperscript);

    QPainterPath textObject;

    if (useSuperScript){

      std::vector<QString> textVect;
      std::vector<QFont> fontVect;

      QFont fontSScript2 = ft;
      fontSScript2.setPointSize(fontSScript2.pointSize() / 2);

      fontVect.push_back(fontSScript2);
      fontVect.push_back(ft);

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

      textObject = ItemUtils::superscriptTextToVector(textVect, fontVect, QPointF());
    }
    else{
      textObject = ItemUtils::textToVector(convert, ft, QPointF(), 0);
    }

    QRectF rectF(textObject.boundingRect());

    calculateLeft(line.p1(), rectF, convert, bLeftRotate, verticalDisplacement);
    calculateRight(line.p2(), rectF, convert, bRightRotate, verticalDisplacement);
  }
}

void te::layout::GridPlanarItem::generateGridCrossStyle(){

}


void te::layout::GridPlanarItem::calculateHorizontal( const te::gm::Envelope& geoBox, const te::gm::Envelope& boxMM )
{
  const Property& pGridSettings = this->getProperty("GridSettings");

  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pTextFontFamily = pGridSettings.getSubProperty(settingsConfig.getFont());
  const Property& pHorizontalGap = pGridSettings.getSubProperty(settingsConfig.getLneHrzGap());
  const Property& pHorizontalDisplacement = pGridSettings.getSubProperty(settingsConfig.getLneHrzDisplacement());
  const Property& pUnit = pGridSettings.getSubProperty(settingsConfig.getUnit());
  const Property& pTopRotate = pGridSettings.getSubProperty(settingsConfig.getTopRotateText());
  const Property& pBottomRotate = pGridSettings.getSubProperty(settingsConfig.getBottomRotateText());

  Font txtFont = te::layout::Property::GetValueAs<Font>(pTextFontFamily);
 
  double horizontalGap = te::layout::Property::GetValueAs<double>(pHorizontalGap);
  double horizontalDisplacement = te::layout::Property::GetValueAs<double>(pHorizontalDisplacement);

  string unitStr = pUnit.getOptionByCurrentChoice().toString();
  double unitH = getUnit(unitStr);

  bool bTopRotate = te::layout::Property::GetValueAs<bool>(pTopRotate);
  bool bBottomRotate = te::layout::Property::GetValueAs<bool>(pBottomRotate);

  if (this->scene() == 0)
  {
    return;
  }

  Utils utils = ((Scene*) this->scene())->getUtils();

  // Draw a vertical line and the x coordinate change(horizontal)
  WorldTransformer transf = utils.getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  double x1 = initHorizontalLines(geoBox);
  
  m_boundingBox.Union(boxMM);
  
  QFont ft = ItemUtils::convertToQfont(txtFont);

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


    te::gm::LineString lineString(2, te::gm::LineStringType);
    lineString.setPoint(0, x, lly);
    lineString.setPoint(1, x, ury);
    m_verticalLines.push_back(lineString);

    m_gridLines.addPath(ItemUtils::lineToQPath(lineString));

    double number = x1 / unitH;
    QString convert = QString::number(number, 'f', 0);

    std::string text = convert.toStdString();

    const Property& pSuperscript = this->getProperty(settingsConfig.getSuperscriptText());
    bool useSuperScript = te::layout::Property::GetValueAs<bool>(pSuperscript);

    QPainterPath textObject;

    if (useSuperScript){

      std::vector<QString> textVect;
      std::vector<QFont> fontVect;

      QFont fontSScript2 = ft;
      fontSScript2.setPointSize(fontSScript2.pointSize() / 2);

      fontVect.push_back(fontSScript2);
      fontVect.push_back(ft);

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

      textObject = ItemUtils::superscriptTextToVector(textVect, fontVect, QPointF());
    }
    else{
      textObject = ItemUtils::textToVector(convert, ft, QPointF(), 0);
    }

    QRectF rectF(textObject.boundingRect());
    
    calculateTop(line.p2(), rectF, convert, bTopRotate, horizontalDisplacement);
    calculateBottom(line.p1(), rectF, convert, bBottomRotate, horizontalDisplacement);
  }
}

double te::layout::GridPlanarItem::initVerticalLines( const te::gm::Envelope& geoBox )
{
  const Property& pGridSettings = this->getProperty("GridSettings");

  PlanarGridSettingsConfigProperties settingsConfig;

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

double te::layout::GridPlanarItem::initHorizontalLines( const te::gm::Envelope& geoBox )
{
  const Property& pGridSettings = this->getProperty("GridSettings");

  PlanarGridSettingsConfigProperties settingsConfig;

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


double te::layout::GridPlanarItem::getUnit(std::string strUnit)
{
  double unit = 1000.0;
  strUnit = "(" + strUnit + ")";

  if (strUnit == "(m)")
  {
    unit = 1.0;
  }
  return unit;
}
