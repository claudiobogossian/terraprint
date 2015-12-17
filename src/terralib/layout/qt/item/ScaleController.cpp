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

// STL
#include <algorithm>

te::layout::ScaleController::ScaleController(te::layout::AbstractItemModel* model)
  : AbstractItemController(model)
{
}

te::layout::ScaleController::~ScaleController()
{
}

void te::layout::ScaleController::update(const Subject* subject)
{
  ScaleItem* scaleItem = dynamic_cast<ScaleItem*>(this->getView());
  if (scaleItem == 0)
  {
    return;
  }

  if (changeScaleWidthAfterConnection())
  {
    return;
  }

  scaleItem->refreshScaleProperties();
}

double te::layout::ScaleController::getUnit(std::string& strUnit)
{
  double unit = 1000.0;
  strUnit = "(" + getProperty("Unit").getOptionByCurrentChoice().toString() + ")";

  if (strUnit == "(m)")
  {
    unit = 1.0;
  }
  return unit;
}

double te::layout::ScaleController::getGap(double& initialGap)
{
  ScaleItem* scaleItem = dynamic_cast<ScaleItem*>(this->getView());
  if (scaleItem == 0)
  {
    return 0;
  }

  const Property& pTextFont = getProperty("font");
  Font txtFont = pTextFont.getValue().toFont();
  QFont qFont = ItemUtils::convertToQfont(txtFont);

  std::string text = "0";
  QPainterPath firstTextObject = ItemUtils::textToVector(text.c_str(), qFont, QPointF(0, 0));
  initialGap = firstTextObject.boundingRect().width() / 2;

  std::string strUnit;
  double unit = getUnit(strUnit);
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
    ss_value << value;

    text = ss_value.str();
  }
  QPainterPath lastTextObject = ItemUtils::textToVector(text.c_str(), qFont, QPointF(0, 0));
  double finalGap = lastTextObject.boundingRect().width() / 2;
  gap = finalGap + unitGap + 2.5;
  return gap;
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

  Property newProperty = checkScaleWidthAndUnit(propertiesCopy);
  if (!newProperty.isNull())
  {
    propertiesCopy.addProperty(newProperty);
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
  double unit = getUnit(strUnit);
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
  double scaleInUnit = pScaleInUnit.getValue().toDouble();
  double gapX = pCurrentScaleWidth.getValue().toDouble();

  std::string strUnit;
  double unit = getUnit(strUnit);
  double spacing = scale / 100.;
  double mmToCm = gapX / 10.;
  
  double value = (spacing * mmToCm) / unit;

  prop.setName("scale_in_unit_width_rect_gap");
  prop.setValue(value, dataType->getDataTypeDouble());

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
  if (currentUnit > newUnit)
  {
    scaleInUnit = scaleInUnit * unit;
  }
  else
  {
    scaleInUnit = scaleInUnit / unit;
  }

  prop = pScaleInUnit;
  prop.setValue(scaleInUnit, dataType->getDataTypeDouble());

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
  const Property& pScaleInUnit = getProperty("scale_in_unit_width_rect_gap");
  const Property& pScale = getProperty("scale");

  double scale = pScale.getValue().toDouble();
  double scaleInUnit = pScaleInUnit.getValue().toDouble();
  double gapX = pScaleWidth.getValue().toDouble();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  std::string strUnit;
  double unit = getUnit(strUnit);
  double spacing = scale / 100.;
  double mmToCm = gapX / 10.;

  double value = (spacing * mmToCm) / unit;
  if (value != scaleInUnit)
  {
    Property prop;
    prop.setName("scale_in_unit_width_rect_gap");
    prop.setValue(value, dataType->getDataTypeDouble());
    setProperty(prop);
    change = true;
  }
  return change;
}

