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

