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
#include "GridPlanarController.h"

#include "../../core/pattern/mvc/AbstractItemModel.h"

#include "../../core/property/GridSettingsConfigProperties.h"
#include "../../core/property/PlanarGridSettingsConfigProperties.h"

te::layout::GridPlanarController::GridPlanarController(AbstractItemModel* model)
  : GridMapController(model)
{

}

te::layout::GridPlanarController::~GridPlanarController()
{

}

bool te::layout::GridPlanarController::syncItemAssociation(Properties& properties)
{
  bool result = GridMapController::syncItemAssociation(properties);
  if (result == false)
  {
    return false;
  }

  GridSettingsConfigProperties settingsConfig;
  const Property& pInitialX = properties.getProperty(settingsConfig.getInitialGridPointX());
  const Property& pInitialY = properties.getProperty(settingsConfig.getInitialGridPointY());
  const Property& pGapX = properties.getProperty(settingsConfig.getLneHrzGap());
  const Property& pGapY = properties.getProperty(settingsConfig.getLneVrtGap());

  //if the initial values are being set, we dont try to initialize the values
  if (pInitialX.isNull() == false && pInitialY.isNull() == false)
  {
    return result;
  }

  //if the gap values are being set, we dont try to initialize the values
  if (pGapX.isNull() == false && pGapY.isNull() == false)
  {
    return result;
  }

  //if none of them are being set, we initialize the planar grid
  Property pPlanarBox = properties.getProperty("planar_box");
  if (pPlanarBox.isNull() == true)
  {
    pPlanarBox = this->getProperty("planar_box");
  }

  double gapX = 0.;
  double gapY = 0.;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  const te::gm::Envelope& planarBox = pPlanarBox.getValue().toEnvelope();

  double distance = planarBox.getWidth();
  double initialX = getInitialCoord(planarBox.getLowerLeftX(), distance, gapX);
  {
    Property property(0);
    property.setName(settingsConfig.getInitialGridPointX());
    property.setValue(initialX, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  distance = planarBox.getHeight();
  double initialY = getInitialCoord(planarBox.getLowerLeftY(), distance, gapY);
  {
    Property property(0);
    property.setName(settingsConfig.getInitialGridPointY());
    property.setValue(initialY, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getLneHrzGap());
    property.setValue(gapX, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getLneVrtGap());
    property.setValue(gapY, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  return result;
}

double te::layout::GridPlanarController::getInitialCoord(double initialCoord, double distance, double& gap)
{
  if (distance <= 0)
  {
    gap = 0;
    return 0;
  }
  unsigned const int size = 25;
  int gaps[size] = { 1000, 1500, 2000, 2500, 5000, 7500, 10000, 12500, 15000, 20000, 25000, 50000, 100000, 125000, 150000, 175000, 200000, 250000, 500000, 750000, 1000000, 1250000, 1500000, 1750000, 2000000 };
  int numberOfIntervals = 5;
  gap = (int)(distance / numberOfIntervals);
  for (unsigned int i = 0; i < size; i++)
  {
    if (gap <= gaps[i])
    {
      if (i > 0)
      {
        gap = gaps[i - 1];
      }
      break;
    }
  }
  int interval = (int)(initialCoord / gap);
  return interval * gap;
}


