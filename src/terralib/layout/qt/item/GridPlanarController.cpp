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
#include "../../core/property/SharedProperties.h"
#include <terralib/common/StringUtils.h>
#include <terralib/common/UnitOfMeasure.h>
#include <terralib/maptools/Utils.h>
#include <terralib/srs/SpatialReferenceSystemManager.h>

te::layout::GridPlanarController::GridPlanarController(AbstractItemModel* model)
  : GridMapController(model)
  , m_gridPropertiesInitialized(false)
{

}

te::layout::GridPlanarController::~GridPlanarController()
{

}

void te::layout::GridPlanarController::update(const Subject* subject)
{
  if (m_model == subject)
  {
    GridMapController::update(subject);
    return;
  }

  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if (subjectModel == 0)
  {
    return;
  }

  SharedProperties sharedPropertiesName;

  //new properties
  const Property& pNewWidth = subjectModel->getProperty("width");
  const Property& pNewHeight = subjectModel->getProperty("height");
  const Property& pNewWorldBox = subjectModel->getProperty("world_box");
  const Property& pNewSrid = subjectModel->getProperty("srid");
  const Property& pNewScale = subjectModel->getProperty("scale");
  const Property& pNewItemObserver = subjectModel->getProperty(sharedPropertiesName.getItemObserver()); //associate / dissociate observer

  //current properties
  const Property& pCurrentWidth = this->getProperty("width");
  const Property& pCurrentHeight = this->getProperty("height");
  const Property& pCurrentPlanarBox = this->getProperty("planar_box");
  const Property& pCurrentItemObserver = this->getProperty(sharedPropertiesName.getItemObserver());

  //new values
  double newWidth = pNewWidth.getValue().toDouble();
  double newHeight = pNewHeight.getValue().toDouble();
  int newSrid = pNewSrid.getValue().toInt();
  const te::gm::Envelope& newWorldBox = pNewWorldBox.getValue().toEnvelope();
  te::gm::Envelope newPlanarBox = te::map::GetWorldBoxInPlanar(newWorldBox, newSrid);
  double newScale = pNewScale.getValue().toDouble();
  std::string newItemObservable = pNewItemObserver.getValue().toString();

  if (newScale == 0)
  {
    newScale = 1000;
  }

  //current values
  double currentWidth = pCurrentWidth.getValue().toDouble();
  double currentHeight = pCurrentHeight.getValue().toDouble();
  te::gm::Envelope currentPlanarBox = pCurrentPlanarBox.getValue().toEnvelope();
  const std::string currentItemObservable = pCurrentItemObserver.getValue().toString();

  bool doUpdate = false;
  if (newWidth != currentWidth)
  {
    doUpdate = true;
  }
  else if (newHeight != currentHeight)
  {
    doUpdate = true;
  }
  else if (newPlanarBox.equals(currentPlanarBox) == false)
  {
    doUpdate = true;
  }
  else if (!currentItemObservable.empty() && !newItemObservable.empty())
  {
    if (newItemObservable.compare(currentItemObservable) != 0)
    {
      doUpdate = true;
    }
  }
  else if ((newItemObservable.empty() && !currentItemObservable.empty())
    || (!newItemObservable.empty() && currentItemObservable.empty()))
  {
    doUpdate = true;
  }

  if (doUpdate == true)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    GridSettingsConfigProperties settingsConfig;

    Properties properties("");
    properties.addProperty(pNewWidth);
    properties.addProperty(pNewHeight);
    {
      Property property(0);
      property.setName("planar_box");
      property.setValue(newPlanarBox, dataType->getDataTypeEnvelope());
      properties.addProperty(property);
    }
    {
      Property property(0);
      property.setName("srid");
      property.setValue(newSrid, dataType->getDataTypeInt());
      properties.addProperty(property);
    }

    double gapX = 0.;
    double gapY = 0.;


    if (m_gridPropertiesInitialized == false && newPlanarBox.isValid() == true)
    {
      double distance = newPlanarBox.getWidth();
      double initialX = this->getInitialCoord(newPlanarBox.getLowerLeftX(), distance, gapX);
      {
        Property property(0);
        property.setName(settingsConfig.getInitialGridPointX());
        property.setValue(initialX, dataType->getDataTypeDouble());
        properties.addProperty(property);
      }

      distance = newPlanarBox.getHeight();
      double initialY = this->getInitialCoord(newPlanarBox.getLowerLeftY(), distance, gapY);
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
      m_gridPropertiesInitialized = true;
    }
    else if (newPlanarBox.isValid() == false)
    {

      m_gridPropertiesInitialized = false;

      te::gm::Envelope defaultPlanarBox(0, 0, 10000, 10000);
      newPlanarBox = defaultPlanarBox;
      {
        Property property(0);
        property.setName("planar_box");
        property.setValue(newPlanarBox, dataType->getDataTypeEnvelope());
        properties.updateProperty(property);
      }

    }
    else {

      const Property& pGridSettings = this->getProperty("GridSettings");
      const Property& pGapVrt = pGridSettings.containsSubProperty(settingsConfig.getLneVrtGap());
      const Property& pGapHrz = pGridSettings.containsSubProperty(settingsConfig.getLneHrzGap());

      const Property& pInitialX = pGridSettings.containsSubProperty(settingsConfig.getInitialGridPointX());
      const Property& pInitialY = pGridSettings.containsSubProperty(settingsConfig.getInitialGridPointY());

      double gapVrt = pGapVrt.getValue().toDouble();
      double gapHrz = pGapHrz.getValue().toDouble();

      double initialX = pInitialX.getValue().toDouble();
      double initialY = pInitialY.getValue().toDouble();

      double distance = newPlanarBox.getWidth();

      initialX = adjustInitialX(newPlanarBox, initialX, gapVrt);
      {
        Property property(0);
        property.setName(settingsConfig.getInitialGridPointX());
        property.setValue(initialX, dataType->getDataTypeDouble());
        properties.addProperty(property);
      }

      distance = newPlanarBox.getHeight();

      initialY = adjustInitialY(newPlanarBox, initialY, gapHrz);
      {
        Property property(0);
        property.setName(settingsConfig.getInitialGridPointY());
        property.setValue(initialY, dataType->getDataTypeDouble());
        properties.addProperty(property);
      }

    }


    te::gm::Envelope defaultPlanarBox(0, 0, 10000, 10000);
    if (newPlanarBox.equals(defaultPlanarBox) == false)
    {
      if (!newPlanarBox.isValid())
      {
        newPlanarBox = defaultPlanarBox;
        {
          Property property(0);
          property.setName("planar_box");
          property.setValue(newPlanarBox, dataType->getDataTypeEnvelope());
          properties.updateProperty(property);
        }
      }
    }

    setProperties(properties);
  }
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