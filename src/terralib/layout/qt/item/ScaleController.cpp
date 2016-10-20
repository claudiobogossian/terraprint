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
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/pattern/proxy/AbstractProxyProject.h"
#include "../core/Value.h"
#include "../core/Scene.h"
#include "../../core/property/SharedProperties.h"
#include "../../core/enum/EnumScaleType.h"

// STL
#include <algorithm>
#include <cmath>

te::layout::ScaleController::ScaleController(te::layout::AbstractItemModel* model)
  : AbstractItemController(model)
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

    if (changeScaleWidthAfterConnection())
    {
      AbstractItemController::update(subject);
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
  double scaleNew = pScaleNew.getValue().toDouble();
  double scaleCurrent = pScaleCurrent.getValue().toDouble();

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

double te::layout::ScaleController::getGap(double& initialGap, Font font, int numberOfBreaks, double scaleWidthX, std::string strUnit)
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
    lastTextObject = getLastTextByBreaks(numberOfBreaks, font, scaleWidthX, strUnit);
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
  Font txtFont = pTextFont.getValue().toFont();
  QFont qFont = ItemUtils::convertToQfont(txtFont);

  std::string text = "0";

  std::string strUnit;
  double unit = getCurrentUnit(strUnit);
  QPainterPath unitTextObject = ItemUtils::textToVector(strUnit.c_str(), qFont, QPointF(0, 0));
  double unitGap = unitTextObject.boundingRect().width();

  const Property& pScale = getProperty("scale");
  const Property& pScaleGapX = getProperty("scale_width_rect_gap");

  double scale = pScale.getValue().toDouble();
  double gapX = pScaleGapX.getValue().toDouble();
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

QPainterPath te::layout::ScaleController::getLastTextByBreaks(int numberOfBreaks, Font font, double scaleWidthX, std::string strUnit)
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

  const Property& pScale = getProperty("scale");

  const Property& pScaleGapX = getProperty("scale_width_rect_gap");

  double scale = pScale.getValue().toDouble();
  double gapX = pScaleGapX.getValue().toDouble();

  if (scaleWidthX != 0)
  {
    gapX = scaleWidthX;
  }

  //convert millimeters to centimeters
  double mmToCm = gapX / 10.;
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

  calculateNewRectSize(propertiesCopy);

  const Property pScaleInUnit = propertiesCopy.getProperty("scale_in_unit_width_rect_gap");

  Property newProperty = checkScaleWidthAndUnit(propertiesCopy);
  if (!newProperty.isNull())
  {
    propertiesCopy.addProperty(newProperty);
  }

  Properties newProperties = checkByBreaks(propertiesCopy);


  const Property& pUnit = getProperty("Unit");

  std::string unit = pUnit.getValue().convertToString();

  Variant v = pScaleInUnit.getValue();

  Property prop;

  if (!v.isUsingPrecision() ){
    v.usePrecision(true);

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    if (unit == "(m)" || unit == "m"){
      v.setPrecision(0);
      prop.setName("scale_in_unit_width_rect_gap");
      prop.setValue(v, dataType->getDataTypeDouble(), true, 0);
    }
    else{
      v.setPrecision(1);
      prop.setName("scale_in_unit_width_rect_gap");
      prop.setValue(v, dataType->getDataTypeDouble(), true, 1);
    }
    newProperties.completelyUpdateProperty(prop);
    newProperties.addProperty(prop);
  }

  if (!newProperties.getProperties().empty())
  {
    std::vector<Property> props = newProperties.getProperties();
    for (std::vector<Property>::iterator it = props.begin(); it != props.end(); ++it)
    {
      newProperty = (*it);
      propertiesCopy.addProperty(newProperty);
    }
  }

  //we finally set the properties into the model
  AbstractItemController::setProperties(propertiesCopy);
}

te::layout::Property te::layout::ScaleController::checkScaleWidthAndUnit(const Properties& properties)
{
  const Property& pScaleWidth = properties.getProperty("scale_width_rect_gap");
  const Property& pScaleInUnit = properties.getProperty("scale_in_unit_width_rect_gap");
  const Property& pScaleInUnitFromModel = m_model->getProperty("scale_in_unit_width_rect_gap");
  const Property& pScale = m_model->getProperty("scale");
  const Property& pNewUnit = properties.getProperty("Unit");

  Property newProperty;

  // If both are being updated, so no calculation will be made
  if (!pScaleWidth.isNull() && !pScaleInUnit.isNull())
  {
    return newProperty;
  }

  double scaleInUnit = pScaleInUnit.getValue().toDouble();
  double scaleInUnitFromModel = pScaleInUnitFromModel.getValue().toDouble();
  double scale = pScale.getValue().toDouble();

  if (!pScaleInUnit.isNull())
  {
    if (scaleInUnit != scaleInUnitFromModel)
    {
      newProperty = calculateScaleWidthInMM(properties);
    }
  }
  else if(!pNewUnit.isNull())
  {
    newProperty = calculateScaleUnit(properties);
  }
  else if (!pScaleWidth.isNull())
  {
    newProperty = calculateScaleWidthInUnit(properties);
  }

  return newProperty;
}

te::layout::Property te::layout::ScaleController::calculateScaleWidthInMM(const Properties& properties)
{
  Property prop;

  const Property& pScaleWidth = properties.getProperty("scale_width_rect_gap");
  const Property& pScaleInUnit = properties.getProperty("scale_in_unit_width_rect_gap");

  // If both are being updated, so no calculation will be made
  if (!pScaleWidth.isNull() && !pScaleInUnit.isNull())
  {
    return prop;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  const Property& pScale = m_model->getProperty("scale");

  double scale = pScale.getValue().toDouble();
  double scaleInUnit = pScaleInUnit.getValue().toDouble();

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

  const Property& pScaleWidth = properties.getProperty("scale_width_rect_gap");
  const Property& pScaleInUnit = properties.getProperty("scale_in_unit_width_rect_gap");

  // If both are being updated, so no calculation will be made
  if (!pScaleWidth.isNull() && !pScaleInUnit.isNull())
  {
    return prop;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  const Property& pScale = m_model->getProperty("scale");
  const Property& pCurrentScaleWidth = properties.getProperty("scale_width_rect_gap");

  double scale = pScale.getValue().toDouble();
  double gapX = pCurrentScaleWidth.getValue().toDouble();

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

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pScaleInUnit = m_model->getProperty("scale_in_unit_width_rect_gap");
  const Property& pNewUnit = properties.getProperty("Unit");
  const Property& pUnit = m_model->getProperty("Unit");

  if (pNewUnit.isNull())
  {
    return prop;
  }

  double scaleInUnit = pScaleInUnit.getValue().toDouble();

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

bool te::layout::ScaleController::changeScaleWidthAfterConnection()
{
  bool change = false;

  SharedProperties sharedProps;
  const Property& pConnection = getProperty(sharedProps.getItemObserver());
  if (pConnection.isNull())
  {
    return change;
  }

  const Property& pScaleWidth = getProperty("scale_width_rect_gap");
  if (pScaleWidth.isNull()){
    return change;
  }

  const Property& pScaleInUnit = getProperty("scale_in_unit_width_rect_gap");
  if (pScaleInUnit.isNull()){
    return change;
  }

  const Property& pScale = getProperty("scale");
  if (pScale.isNull()){
    return change;
  }

  double scale = pScale.getValue().toDouble();
  double scaleInUnit = pScaleInUnit.getValue().toDouble();
  double gapX = pScaleWidth.getValue().toDouble();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  std::string strUnit;
  double unit = getCurrentUnit(strUnit);
  double spacing = scale / 100.;
  double mmToCm = gapX / 10.;

  double valueDouble = (spacing * mmToCm) / unit;
  double value = valueDouble;

  const Property& pNewUnit = getProperty("Unit");
  std::string unitStr = "(" + pNewUnit.getOptionByCurrentChoice().toString() + ")";

  if (unitStr == "(m)" || unitStr == "m"){
    value = qRound(valueDouble);
  }
  else{
    value = std::ceil(valueDouble * 2) / 2.0;
  }


  if ((value != scaleInUnit) && value > 0)
  {
    Property prop;

    if (unitStr == "(m)" || unitStr == "m"){
      prop.setName("scale_in_unit_width_rect_gap");
      prop.setValue(value, dataType->getDataTypeDouble(), true, 0);
    }
    else{
      prop.setName("scale_in_unit_width_rect_gap");
      prop.setValue(value, dataType->getDataTypeDouble(), true, 1);
    }


    setProperty(prop);
    change = true;
  }
  return change;
}

te::layout::Properties te::layout::ScaleController::checkByBreaks(const Properties& properties)
{
  Properties props;
  
  const Property& pNewNumberOfBreaks = properties.getProperty("number_of_breaks");
  const Property& pNumberOfBreaks = getProperty("number_of_breaks");
  const Property& pNewByBreaks = properties.getProperty("by_breaks");
  const Property& pByBreaks = getProperty("by_breaks");
  const Property& pNewScaleWidth = properties.getProperty("scale_width_rect_gap");
  const Property& pScaleWidth = getProperty("scale_width_rect_gap");
  const Property& pNewScaleInUnit = properties.getProperty("scale_in_unit_width_rect_gap");
  const Property& pScaleInUnit = getProperty("scale_in_unit_width_rect_gap");
  const Property& pNewTextFont = properties.getProperty("font");
  const Property& pTextFont = getProperty("font");
  const Property& pNewUnit = properties.getProperty("Unit");
  const Property& pUnit = getProperty("Unit");
  Property pCurrentWidth = getProperty("width");
  Property pResizable = getProperty("resizable");

  if (pNewNumberOfBreaks.isNull() && pNewByBreaks.isNull()
      && pNewScaleWidth.isNull() && pNewScaleInUnit.isNull()
      && pNewTextFont.isNull())
  {
    return props;
  }

  int numberOfBreaks = pNumberOfBreaks.getValue().toInt();
  bool byBreaks = pByBreaks.getValue().toBool();
  double currentWidth = pCurrentWidth.getValue().toDouble();
  double scaleWidth = pScaleWidth.getValue().toDouble();
  double scaleInUnit = pScaleInUnit.getValue().toDouble();
  Font font = pTextFont.getValue().toFont();
  std::string strUnit = pUnit.getValue().toString();

  bool resizable = pResizable.getValue().toBool();
  double width = 0;

  if (!pNewNumberOfBreaks.isNull())
  {
    numberOfBreaks = pNewNumberOfBreaks.getValue().toInt();
  }
  if (!pNewByBreaks.isNull())
  {
    byBreaks = pNewByBreaks.getValue().toBool();
  }
  if (!pNewScaleWidth.isNull())
  {
    scaleWidth = pNewScaleWidth.getValue().toDouble();
  }
  if (!pNewScaleInUnit.isNull())
  {
    scaleInUnit = pNewScaleInUnit.getValue().toDouble();
  }
  if (!pNewTextFont.isNull())
  {
    font = pNewTextFont.getValue().toFont();
  }
  if (!pNewUnit.isNull())
  {
    strUnit = pNewUnit.getValue().toString();
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  resizable = !byBreaks;
  pResizable.setValue(resizable, dataType->getDataTypeBool());
  props.addProperty(pResizable);

  if (!byBreaks)
  {
    return props;
  }
  
  strUnit = "(" + strUnit + ")";

  width = getFullWidthByBreaks(numberOfBreaks, scaleWidth, scaleInUnit, font, strUnit);
  if (width <= 0)
  {
    return props;
  }

  if (width != currentWidth)
  {
    pCurrentWidth.setValue(width, dataType->getDataTypeDouble());
    props.addProperty(pCurrentWidth);
  }

  return props;
}

double te::layout::ScaleController::getFullWidthByBreaks(int numberOfBreaks, double scaleGapX, int scaleUnitGapX, Font font, std::string strUnit)
{
  //if somehow the item is invalid, we do nothing
  ScaleItem* view = dynamic_cast<ScaleItem*>(m_view);
  if (view == 0)
  {
    return 0;
  }

  if (!view->scene())
    return 0;

  Scene* sc = dynamic_cast<Scene*>(view->scene());
  ItemUtils utils = sc->getItemUtils();
  
  double value = 0.;
  double width = 0;

  double displacementBetweenScaleAndText = 2.;
  std::stringstream ss_value;
  ss_value.precision(15);

  double initialGap = 0;
  double gap = getGap(initialGap, font, numberOfBreaks, scaleGapX, strUnit);
  width += initialGap;

  for (int i = 0; i < numberOfBreaks; ++i)
  {
    ss_value << value;
    const std::string& text = ss_value.str();
    if (value == 0)
    {
      QRectF textRect = utils.getMinimumTextBoundary(font.getFamily(), font.getPointSize(), text);

      double firstTextWidth = textRect.width();
      width += displacementBetweenScaleAndText + textRect.width();
    }

    value += scaleUnitGapX;
    width += scaleGapX;
  }

  width += gap;

  return width;
}


void te::layout::ScaleController::calculateNewRectSize(te::layout::Properties& properties)
{
  if (properties.contains("scale_height_rect_gap") == true || properties.contains("font") == true || properties.contains("scale_type")
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
    EnumType* currentScaleType = enumScale.searchLabel(label);

    QRectF rect(0, 0, pCopyWidth.getValue().toDouble(), 10);

    QRectF gap(rect.bottomLeft().x(), rect.bottomLeft().y(), pWidthGap.getValue().toDouble(), pHeightGap.getValue().toDouble());
    rect = rect.united(gap);

    if (currentScaleType != enumScale.getAlternatingScaleBarType())
    {
      QRectF doubleGap(gap.bottomLeft().x(), gap.bottomLeft().y(), pWidthGap.getValue().toDouble(), pHeightGap.getValue().toDouble());
      rect = rect.united(doubleGap);
    }


    QRectF space(rect.bottomLeft().x(), rect.bottomLeft().y(), pWidthGap.getValue().toDouble(), 4.0);

    QFont qFont;

    if (properties.contains("font") == true)
    {
      const Property& pFont = properties.getProperty("font");
      Font font = pFont.getValue().toFont();
      qFont = ItemUtils::convertToQfont(font);
    }
    else
    {
      const Property& pFont = getProperty("font");
      Font font = pFont.getValue().toFont();
      qFont = ItemUtils::convertToQfont(font);
    }

    double height = pWidth.getValue().toDouble();
    double widthGap = pWidthGap.getValue().toDouble();

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