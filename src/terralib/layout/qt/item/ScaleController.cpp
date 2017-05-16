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

// TerraLib
#include "ScaleController.h"
#include "ScaleItem.h"

#include "../core/Scene.h"
#include "../../core/enum/Enums.h"
#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/EnumScaleType.h"
#include "../../core/property/SharedProperties.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"

// STL
#include <algorithm>
#include <cmath>

te::layout::ScaleController::ScaleController(te::layout::AbstractItemModel* model, AbstractItemView* view)
  : AbstractItemController(model, view)
{
}

te::layout::ScaleController::~ScaleController()
{
}

void te::layout::ScaleController::update(const Subject* subject)
{
  if (m_model == subject)
  {
    ScaleItem* scaleItem = dynamic_cast<ScaleItem*>(this->getView());
    if (scaleItem == 0)
    {
      return;
    }

    scaleItem->refreshScaleProperties();
    AbstractItemController::update(subject);
    return;
  }

  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if (subjectModel == 0)
  {
    return;
  }

  const Property& pScaleNew = subjectModel->getProperty("scale");
  const Property& pScaleCurrent = this->getProperty("scale");
  double scaleNew = te::layout::Property::GetValueAs<double>(pScaleNew);
  double scaleCurrent = te::layout::Property::GetValueAs<double>(pScaleCurrent);

  if (scaleNew != scaleCurrent)
  {
    setProperty(pScaleNew);
  }
}

double te::layout::ScaleController::getCurrentUnit(std::string& strUnit)
{
  double unit = 1000.0;
  strUnit = "(" + getProperty("Unit").getOptionByCurrentChoice().toString() + ")";

  if (strUnit == "(m)")
  {
    unit = 1.0;
  }
  return unit;
}

double te::layout::ScaleController::getUnitValue(std::string strUnit)
{
  double unit = 1000.0;
  if (strUnit == "(m)" || strUnit == "m")
  {
    unit = 1.0;
  }
  return unit;
}

double te::layout::ScaleController::getGap(double& initialGap, Font font, int numberOfBreaks, double scaleWidthX, std::string strUnit, double scale)
{
  ScaleItem* scaleItem = dynamic_cast<ScaleItem*>(this->getView());
  if (scaleItem == 0)
  {
    return 0;
  }

  QFont qFont = ItemUtils::convertToQfont(font);

  std::string text = "0";
  QPainterPath firstTextObject = ItemUtils::textToVector(text.c_str(), qFont, QPointF(0, 0));
  initialGap = firstTextObject.boundingRect().width() / 2;

  std::string strCurrentUnit;
  double unit = getCurrentUnit(strCurrentUnit);

  if (strUnit.compare("") != 0)
  {
    unit = getUnitValue(strUnit);
    strCurrentUnit = strUnit;
  }

  QPainterPath unitTextObject = ItemUtils::textToVector(strCurrentUnit.c_str(), qFont, QPointF(0, 0));

  double unitGap = unitTextObject.boundingRect().width();
  double gap = unitGap + 2.5;

  QPainterPath lastTextObject;
  if (numberOfBreaks > 0)
  {
    lastTextObject = getLastTextByBreaks(numberOfBreaks, font, scaleWidthX, strUnit, scale);
  }
  else
  {
    lastTextObject  = getLastText();
  }

  double finalGap = lastTextObject.boundingRect().width() / 2;
  gap = finalGap + unitGap + 2.5;
  return gap;
}

QPainterPath te::layout::ScaleController::getLastText()
{
  QPainterPath lastTextObject;
  ScaleItem* scaleItem = dynamic_cast<ScaleItem*>(this->getView());
  if (scaleItem == 0)
  {
    return lastTextObject;
  }

  const Property& pTextFont = getProperty("font");
  Font txtFont = te::layout::Property::GetValueAs<Font>(pTextFont);
  QFont qFont = ItemUtils::convertToQfont(txtFont);

  std::string text = "0";

  std::string strUnit;
  double unit = getCurrentUnit(strUnit);
  QPainterPath unitTextObject = ItemUtils::textToVector(strUnit.c_str(), qFont, QPointF(0, 0));
  double unitGap = unitTextObject.boundingRect().width();

  const Property& pScale = getProperty("scale");
  const Property& pScaleGapX = getProperty("scale_width_rect_gap");

  double scale = te::layout::Property::GetValueAs<double>(pScale);
  double gapX = te::layout::Property::GetValueAs<double>(pScaleGapX);
  //convert millimeters to centimeters
  double mmToCm = gapX / 10.;

  double spacing = scale / 100.;

  double value = 0.;
  double width = 0.;
  QRectF boundRect = scaleItem->boundingRect();
  double x1 = boundRect.bottomLeft().x();
  double gap = unitGap + 2.5;
  for (; (x1 + gap) <= boundRect.topRight().x(); x1 += width)
  {
    if (width == 0)
      width = gapX;
    else
      value += (spacing * mmToCm) / unit;

    std::stringstream ss_value;
    ss_value.precision(15);
    ss_value << value;

    text = ss_value.str();
  }
  lastTextObject = ItemUtils::textToVector(text.c_str(), qFont, QPointF(0, 0));
  return lastTextObject;
}

QPainterPath te::layout::ScaleController::getLastTextByBreaks(int numberOfBreaks, Font font, double scaleWidthX, std::string strUnit, double scale)
{
  QPainterPath lastTextObject;
  ScaleItem* scaleItem = dynamic_cast<ScaleItem*>(this->getView());
  if (scaleItem == 0)
  {
    return lastTextObject;
  }

  QFont qFont = ItemUtils::convertToQfont(font);

  std::string text = "0";

  std::string strCurrentUnit;
  double unit = getCurrentUnit(strCurrentUnit);

  if (strUnit.compare("") != 0)
  {
    unit = getUnitValue(strUnit);
    strCurrentUnit = strUnit;
  }

  QPainterPath unitTextObject = ItemUtils::textToVector(strCurrentUnit.c_str(), qFont, QPointF(0, 0));
  double unitGap = unitTextObject.boundingRect().width();

  //convert millimeters to centimeters
  double mmToCm = scaleWidthX / 10.;
  double spacing = scale / 100.;

  double value = 0.;

  for (int i = 0; i < numberOfBreaks; ++i)
  {
    value += (spacing * mmToCm) / unit;

    std::stringstream ss_value;
    ss_value.precision(15);
    ss_value << value; 
    text = ss_value.str();
  }
  lastTextObject = ItemUtils::textToVector(text.c_str(), qFont, QPointF(0, 0));
  return lastTextObject;
}

void te::layout::ScaleController::setProperty(const te::layout::Property& property)
{
  te::layout::Properties properties;
  properties.addProperty(property);

  setProperties(properties);
}

void te::layout::ScaleController::setProperties(const te::layout::Properties& properties)
{
  //if somehow the item is invalid, we do nothing
  ScaleItem* view = dynamic_cast<ScaleItem*>(m_view);
  if (view == 0)
  {
    AbstractItemController::setProperties(properties);
    return;
  }

  te::layout::Properties propertiesCopy = properties;

  bool resultScaleAndUnitGap = syncScaleAndUnitGap(propertiesCopy); // if scale change, so gap unit value will change too

  syncScaleWidthUnitGap(propertiesCopy); // in some unit, ex.: km or m

  if (!resultScaleAndUnitGap) // if scale change and gap unit value recalculate, the scale width gap (mm) still the same
  {
    syncScaleWidthGap(propertiesCopy); // in mm
  }

  if (!syncBreaksByWidth(propertiesCopy))
  {
    propertiesCopy.removeProperty("width");
    if (propertiesCopy.getProperties().size() == 0)
    {
      return;
    }
  }

  checkByBreaks(propertiesCopy); // recalculate width by breaks

  syncScaleUnit(propertiesCopy); // check if change unit and update gap unit property

  //we finally set the properties into the model
  AbstractItemController::setProperties(propertiesCopy);
}

bool te::layout::ScaleController::syncScaleWidthUnitGap(te::layout::Properties& properties)
{
  bool result = false;
  // in some unit, ex.: km or m
  if (properties.contains("scale_in_unit_width_rect_gap"))
    return result;

  if (properties.contains("scale_width_rect_gap"))
  {
    // recalculate 'scale in unit width rect gap'
    Property newProperty = calculateScaleWidthInUnit(properties);
    properties.addProperty(newProperty);
    result = true;
  }
  return result;
}

bool te::layout::ScaleController::syncScaleWidthGap(te::layout::Properties& properties)
{
  bool result = false;
  // in mm
  if (properties.contains("scale_width_rect_gap"))
    return result;

  if (properties.contains("scale_in_unit_width_rect_gap"))
  {
    const Property& pScaleInUnit = properties.getProperty("scale_in_unit_width_rect_gap");
    const Property& pScaleInUnitFromModel = m_model->getProperty("scale_in_unit_width_rect_gap");

    double scaleInUnit = te::layout::Property::GetValueAs<double>(pScaleInUnit);
    double scaleInUnitFromModel = te::layout::Property::GetValueAs<double>(pScaleInUnitFromModel);
    if (scaleInUnit != scaleInUnitFromModel)
    {
      // recalculate 'scale width rect gap'
      Property newProperty = calculateScaleWidthInMM(properties);
      properties.addProperty(newProperty);
      result = true;
    }
  }
  return result;
}

bool te::layout::ScaleController::syncScaleUnit(te::layout::Properties& properties)
{
  bool result = false;
  // check if change unit and update gap unit property

  if (properties.contains("Unit"))
  {
    // recalculate 'scale in unit width rect gap'
    Property newProperty = calculateScaleUnit(properties);
    if (properties.contains("scale_in_unit_width_rect_gap"))
    {
      properties.updateProperty(newProperty);
    }
    else
    {
      properties.addProperty(newProperty);
    }
    result = true;
  }
  return result;
}

bool te::layout::ScaleController::syncScaleAndUnitGap(te::layout::Properties& properties)
{
  bool result = false;
  // if scale change, so gap unit value will change too
  if (properties.contains("scale_in_unit_width_rect_gap"))
    return result;

  if (properties.contains("scale"))
  {
    // recalculate 'scale in unit width rect gap'
    Property newProperty = calculateScaleWidthInUnit(properties);
    properties.addProperty(newProperty);
    result = true;
  }
  return result;
}

bool te::layout::ScaleController::syncBreaksByWidth(te::layout::Properties& properties)
{
  bool result = false;
    
  // in some unit, ex.: km or m
  if (properties.contains("number_of_breaks"))
    return result;

  if (properties.contains("width"))
  {
    const Property& pWidth = properties.getProperty("width");
    double width = te::layout::Property::GetValueAs<double>(pWidth);
    double biggerBreak = 0;
    int newNumBreaks = calculateNewNumBreaks(properties, biggerBreak);

    if (width < biggerBreak || newNumBreaks <= 0)
    {
      return result;
    }

    Property pNumberOfBreaks = getProperty("number_of_breaks");    
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    pNumberOfBreaks.setValue(newNumBreaks, dataType->getDataTypeInt());
    properties.addProperty(pNumberOfBreaks);
    result = true;
  }
  return result;
}

te::layout::Property te::layout::ScaleController::calculateScaleWidthInMM(const Properties& properties)
{
  Property prop;

  // If both are being updated, so no calculation will be made
  if (properties.contains("scale_width_rect_gap") && properties.contains("scale_in_unit_width_rect_gap"))
  {
    return prop;
  }

  const Property& pScaleInUnit = properties.getProperty("scale_in_unit_width_rect_gap");

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  const Property& pScale = m_model->getProperty("scale");

  double scale = te::layout::Property::GetValueAs<double>(pScale);
  double scaleInUnit = te::layout::Property::GetValueAs<double>(pScaleInUnit);

  std::string strUnit;
  double unit = getCurrentUnit(strUnit);
  double spacing = scale / 100.;

  double newGapX = (scaleInUnit * unit) / spacing;
  newGapX = newGapX * 10; // cm to mm
  
  prop.setName("scale_width_rect_gap");
  prop.setValue(newGapX, dataType->getDataTypeDouble());

  return prop;
}

te::layout::Property te::layout::ScaleController::calculateScaleWidthInUnit(const Properties& properties)
{
  Property prop;

  // If both are being updated, so no calculation will be made
  if (properties.contains("scale_width_rect_gap") && properties.contains("scale_in_unit_width_rect_gap"))
  {
    return prop;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  const Property& pScale = getProperty("scale", properties);
  const Property& pCurrentScaleWidth = getProperty("scale_width_rect_gap", properties);

  double scale = te::layout::Property::GetValueAs<double>(pScale);
  double gapX = te::layout::Property::GetValueAs<double>(pCurrentScaleWidth);

  std::string strUnit;
  double unit = getCurrentUnit(strUnit);
  double spacing = scale / 100.;
  double mmToCm = gapX / 10.;
  
  double valueDouble = (spacing * mmToCm) / unit;
  double value = valueDouble;

  const Property& pNewUnit = getProperty("Unit");
  std::string unitStr = "("+ pNewUnit.getOptionByCurrentChoice().toString() + ")";

  if (unitStr == "(m)" || unitStr == "m"){
    value = qRound(valueDouble);
    prop.setName("scale_in_unit_width_rect_gap");
    prop.setValue(value, dataType->getDataTypeDouble(), true, 0);
  }
  else{
    value = std::ceil(valueDouble * 2) / 2.0;
    prop.setName("scale_in_unit_width_rect_gap");
    prop.setValue(value, dataType->getDataTypeDouble(), true, 1);
  }

  return prop;
}

te::layout::Property te::layout::ScaleController::calculateScaleUnit(const Properties& properties)
{
  Property prop;
  if (properties.contains("Unit") == false)
  {
    return prop;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  const Property& pScaleInUnit = getProperty("scale_in_unit_width_rect_gap", properties);
  const Property& pNewUnit = properties.getProperty("Unit");
  const Property& pUnit = getProperty("Unit");

  double scaleInUnit = te::layout::Property::GetValueAs<double>(pScaleInUnit);

  std::string strUnit = pUnit.getOptionByCurrentChoice().toString();
  std::string strNewUnit = pNewUnit.getOptionByCurrentChoice().toString();

  double currentUnit = strToUnit(strUnit);
  double newUnit = strToUnit(strNewUnit);

  if (currentUnit == newUnit)
  {
    return prop;
  }

  double unit = std::max(currentUnit, newUnit);

  double scaleInUnitDouble = 0;

  if (currentUnit > newUnit)
  {
    scaleInUnitDouble = scaleInUnit * unit;
  }
  else
  {
    scaleInUnitDouble = scaleInUnit / unit;
  }
  
  const Property& pNewUnitString = getProperty("Unit");
  
  std::string unitStr = "(" + pNewUnit.getOptionByCurrentChoice().toString() + ")";

  if (unitStr == "(m)" || unitStr == "m"){
    scaleInUnit = qRound(scaleInUnitDouble);

    prop = pScaleInUnit;
    prop.setValue(scaleInUnit, dataType->getDataTypeDouble(), true, 0);
  }
  else{
    scaleInUnit = std::ceil(scaleInUnitDouble * 2) / 2.0;
    prop = pScaleInUnit;
    prop.setValue(scaleInUnit, dataType->getDataTypeDouble(), true, 1);
  }
  
  return prop;
}

double te::layout::ScaleController::strToUnit(std::string& strUnit)
{
  double unit = 1000.0;
  if (strUnit.compare("m") == 0)
  {
    unit = 1.0;
  }
  return unit;
}

void te::layout::ScaleController::checkByBreaks(Properties& properties)
{ 
  // recalculate width by breaks
  
  bool hasNewNumberOfBreaks = properties.contains("number_of_breaks");
  bool hasNewByBreaks = properties.contains("by_breaks");
  bool hasNewScaleWidth = properties.contains("scale_width_rect_gap");
  bool hasNewScaleInUnit = properties.contains("scale_in_unit_width_rect_gap");
  bool hasNewTextFont = properties.contains("font");

  if (hasNewNumberOfBreaks == false && hasNewByBreaks == false
      && hasNewScaleWidth == false && hasNewScaleInUnit == false
      && hasNewTextFont == false)
  {
    return;
  }

  bool hasNewWidth = properties.contains("width");

  if ((hasNewByBreaks && hasNewWidth) || hasNewWidth)
  {
    return;
  }

  const Property& pByBreaks = getProperty("by_breaks", properties);
  bool byBreaks = te::layout::Property::GetValueAs<bool>(pByBreaks);

  if (!byBreaks)
  {
    return;
  }

  const Property& pNumberOfBreaks = getProperty("number_of_breaks", properties);
  const Property& pScaleWidth = getProperty("scale_width_rect_gap", properties);
  const Property& pScaleInUnit = getProperty("scale_in_unit_width_rect_gap", properties);
  const Property& pTextFont = getProperty("font", properties);
  const Property& pUnit = getProperty("Unit", properties);
  const Property& pScale = getProperty("scale", properties);
  Property pCurrentWidth = getProperty("width");
  Property pResizable = getProperty("resizable");

  int numberOfBreaks = te::layout::Property::GetValueAs<int>(pNumberOfBreaks);
  double currentWidth = te::layout::Property::GetValueAs<double>(pCurrentWidth);
  double scaleWidth = te::layout::Property::GetValueAs<double>(pScaleWidth);
  double scaleInUnit = te::layout::Property::GetValueAs<double>(pScaleInUnit);
  Font font = te::layout::Property::GetValueAs<Font>(pTextFont);
  std::string strUnit = te::layout::Property::GetValueAs<std::string>(pUnit);
  double scale = te::layout::Property::GetValueAs<double>(pScale);

  double width = 0;
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  strUnit = "(" + strUnit + ")";

  width = getFullWidthByBreaks(numberOfBreaks, scaleWidth, scaleInUnit, font, strUnit, scale);
  if (width <= 0)
  {
    return;
  }

  if (width != currentWidth)
  {
    pCurrentWidth.setValue(width, dataType->getDataTypeDouble());
    properties.addProperty(pCurrentWidth);
  }
}

double te::layout::ScaleController::getFullWidthByBreaks(int numberOfBreaks, double scaleGapX, int scaleUnitGapX, Font font, std::string strUnit, double scale)
{
  double value = 0.;
  double width = 0;

  const Property& pDisplacement = getProperty("displacementBetweenScaleAndText");
  double displacementBetweenScaleAndText = te::layout::Property::GetValueAs<double>(pDisplacement);
  std::stringstream ss_value;
  ss_value.precision(15);

  if (scale == 0)
  {
    const Property& pScale = getProperty("scale");
    scale = te::layout::Property::GetValueAs<double>(pScale);
  }

  double initialGap = 0;
  double finalGap = getGap(initialGap, font, numberOfBreaks, scaleGapX, strUnit, scale);

  width = initialGap + finalGap + (2* displacementBetweenScaleAndText) + (numberOfBreaks * scaleGapX);

  return width;
}

void te::layout::ScaleController::calculateNewRectSize(te::layout::Properties& properties)
{
  if (properties.contains("scale_height_rect_gap") == true || properties.contains("font") == true || properties.contains("scale_type") == true
    || properties.contains("name") == true && properties.contains("id") == true)
  {

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    const Property& pWidth = getProperty("width");
    Property pCopyWidth(pWidth);


    const Property& pHeigth = getProperty("height");
    Property pCopyHeight(pHeigth);

    Property pWidthGap;
    if (properties.contains("scale_width_rect_gap") == true)
    {
      const Property& pWGap = properties.getProperty("scale_width_rect_gap");
      pWidthGap = pWGap;
    }
    else
    {
      const Property& pWGap = getProperty("scale_width_rect_gap");
      pWidthGap = pWGap;
    }


    Property pHeightGap;
    if (properties.contains("scale_height_rect_gap") == true)
    {
      const Property& pHGap = properties.getProperty("scale_height_rect_gap");
      pHeightGap = pHGap;
    }
    else
    {
      const Property& pHGap = getProperty("scale_height_rect_gap");
      pHeightGap = pHGap;

    }

    Property pScaleType;

    if (properties.contains("scale_type") == true)
    {
      const Property& pScaleT = properties.getProperty("scale_type");;
      pScaleType = pScaleT;
    }
    else
    {
      const Property& pScaleT = getProperty("scale_type");
      pScaleType = pScaleT;
    }

    EnumScaleType enumScale;

    const std::string& label = pScaleType.getOptionByCurrentChoice().toString();
    EnumType* currentScaleType = enumScale.getEnum(label);

    QRectF rect(0, 0, te::layout::Property::GetValueAs<double>(pCopyWidth), 10);

    QRectF gap(rect.bottomLeft().x(), rect.bottomLeft().y(), te::layout::Property::GetValueAs<double>(pWidthGap), te::layout::Property::GetValueAs<double>(pHeightGap));
    rect = rect.united(gap);

    if (currentScaleType)
    {
      if (currentScaleType != enumScale.getAlternatingScaleBarType())
      {
        QRectF doubleGap(gap.bottomLeft().x(), gap.bottomLeft().y(), te::layout::Property::GetValueAs<double>(pWidthGap), te::layout::Property::GetValueAs<double>(pHeightGap));
        rect = rect.united(doubleGap);
      }
    }

    QRectF space(rect.bottomLeft().x(), rect.bottomLeft().y(), te::layout::Property::GetValueAs<double>(pWidthGap), 4.0);

    QFont qFont;

    if (properties.contains("font") == true)
    {
      const Property& pFont = properties.getProperty("font");
      const Font& font = te::layout::Property::GetValueAs<Font>(pFont);
      qFont = ItemUtils::convertToQfont(font);
    }
    else
    {
      const Property& pFont = getProperty("font");
      const Font& font = te::layout::Property::GetValueAs<Font>(pFont);
      qFont = ItemUtils::convertToQfont(font);
    }

    double height = te::layout::Property::GetValueAs<double>(pWidth);
    double widthGap = te::layout::Property::GetValueAs<double>(pWidthGap);

    QPointF txtPoint(rect.bottomLeft().x(), rect.bottomLeft().y());

    QString txt("0");

    QPainterPath qpainter = ItemUtils::textToVector(txt, qFont, txtPoint, 0);

    double txtHeight = qpainter.boundingRect().height();
    rect = rect.united(qpainter.boundingRect());
    height = rect.height();

    pCopyHeight.setValue(height, dataType->getDataTypeDouble());
    properties.addProperty(pCopyHeight);
  }
}

int te::layout::ScaleController::calculateNewNumBreaks(const Properties& properties, double & biggerBreak)
{
  double initialGap = 0;

  const Property& pDisplacement = getProperty("displacementBetweenScaleAndText");
  const Property& pScaleWidth = getProperty("scale_width_rect_gap");
  const Property& pWidthInMapUnit = getProperty("scale_in_unit_width_rect_gap");
  const Property& pWidth = properties.getProperty("width");
  const Property& pTextFont = getProperty("font", properties);
  const Property& pUnit = getProperty("Unit", properties);
  const Property& pScale = getProperty("scale", properties);
  Property pNumberOfBreaks = getProperty("number_of_breaks", properties);

  double scaleWidth = te::layout::Property::GetValueAs<double>(pScaleWidth);
  double widthInMapUnit = te::layout::Property::GetValueAs<double>(pWidthInMapUnit);
  double displacementBetweenScaleAndText = te::layout::Property::GetValueAs<double>(pDisplacement);
  double width = te::layout::Property::GetValueAs<double>(pWidth);
  int numberOfBreaks = te::layout::Property::GetValueAs<int>(pNumberOfBreaks);
  const Font& font = te::layout::Property::GetValueAs<Font>(pTextFont);
  std::string strUnit = te::layout::Property::GetValueAs<std::string>(pUnit);
  double scale = te::layout::Property::GetValueAs<double>(pScale);

  QFont qFont = ItemUtils::convertToQfont(font);

  strUnit = "(" + strUnit + ")";

  double probableNumberOfBreaks = (int)(width / scaleWidth);

  std::stringstream ss_value;
  ss_value.precision(15);
  ss_value << (widthInMapUnit * probableNumberOfBreaks);
  std::string text = ss_value.str();
  std::string textAndUnit = text + " " + strUnit;;

  //get the size of the text considering only the number
  QPainterPath number = ItemUtils::textToVector(text.c_str(), qFont);

  //get the size of the text considering the number and the unit
  QPainterPath numberAndUnit = ItemUtils::textToVector(textAndUnit.c_str(), qFont);

  //here we calculate the width (for the last text and unit) that must exist after the last break.
  double finalGap = numberAndUnit.boundingRect().width() - (number.boundingRect().width() / 2.);

  double newWidth = width - (finalGap + displacementBetweenScaleAndText);
  int newBreaks = (int)(newWidth / scaleWidth); 
  biggerBreak = ((initialGap + finalGap) / width) + scaleWidth;

  return newBreaks;
}
