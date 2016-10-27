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
#include "GridGeodesicController.h"

#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/property/GridSettingsConfigProperties.h"
#include "../../core/property/SharedProperties.h"
#include "../../core/Utils.h"

#include <terralib/common/StringUtils.h>
#include <terralib/common/UnitOfMeasure.h>
#include <terralib/srs/SpatialReferenceSystemManager.h>

te::layout::GridGeodesicController::GridGeodesicController(AbstractItemModel* model)
  : GridMapController(model)
  , m_gridPropertiesInitialized(false)
{

}

te::layout::GridGeodesicController::~GridGeodesicController()
{

}

void te::layout::GridGeodesicController::update(const Subject* subject)
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
  GridSettingsConfigProperties settingsGridConfig;

  //new properties
  const Property& pNewWidth = subjectModel->getProperty("width");
  const Property& pNewHeight = subjectModel->getProperty("height");
  const Property& pNewWorldBox = subjectModel->getProperty("world_box");
  const Property& pNewSrid = subjectModel->getProperty("srid");
  const Property& pNewItemObserver = subjectModel->getProperty(sharedPropertiesName.getItemObserver()); //associate / dissociate observer

  const Property& pNewVerticalGap = subjectModel->getProperty(settingsGridConfig.getLneVrtGap());
  const Property& pNewHorizontalGap = subjectModel->getProperty(settingsGridConfig.getLneHrzGap());

  //current properties
  const Property& pCurrentWidth = this->getProperty("width");
  const Property& pCurrentHeight = this->getProperty("height");
  const Property& pCurrentGeographicBox = this->getProperty("geographic_box");
  const Property& pCurrentItemObserver = this->getProperty(sharedPropertiesName.getItemObserver());

  const Property& pCurrentVerticalGap = this->getProperty(settingsGridConfig.getLneVrtGap());
  const Property& pCurrentHorizontalGap = this->getProperty(settingsGridConfig.getLneHrzGap());

  //new values
  double newWidth = pNewWidth.getValue().toDouble();
  double newHeight = pNewHeight.getValue().toDouble();
  int newSrid = pNewSrid.getValue().toInt();
  const te::gm::Envelope& newWorldBox = pNewWorldBox.getValue().toEnvelope();
  if (newWorldBox.isValid() == false)
  {
    Properties properties("");
    properties.addProperty(pNewWidth);
    properties.addProperty(pNewHeight);
    setProperties(properties);

    return;
  }

  te::gm::Envelope newGeographicBox = getWorldBoxInGeographic(newWorldBox, newSrid);
  std::string newItemObservable = pNewItemObserver.getValue().toString();


  //current values
  double currentWidth = pCurrentWidth.getValue().toDouble();
  double currentHeight = pCurrentHeight.getValue().toDouble();
  te::gm::Envelope currentGeographicBox = pCurrentGeographicBox.getValue().toEnvelope();
  std::string currentItemObservable = pCurrentItemObserver.getValue().toString();

  bool doUpdate = false;
  if (newWidth != currentWidth)
  {
    doUpdate = true;
  }
  else if (newHeight != currentHeight)
  {
    doUpdate = true;
  }
  else if (newGeographicBox.equals(currentGeographicBox) == false)
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
      property.setName("geographic_box");
      property.setValue(newGeographicBox, dataType->getDataTypeEnvelope());
      properties.addProperty(property);
    }

    te::gm::Envelope defaultGeographicBox(0, 0, 10000, 10000);
    if (newGeographicBox.equals(defaultGeographicBox) == false)
    {
      if (!newGeographicBox.isValid())
      {
        newGeographicBox = defaultGeographicBox;
        {
          Property property(0);
          property.setName("geographic_box");
          property.setValue(newGeographicBox, dataType->getDataTypeEnvelope());
          properties.updateProperty(property);
        }
      }

      if (m_gridPropertiesInitialized == false && newGeographicBox.isValid() == true)
      {
        {
          double horizontalGap = newGeographicBox.getWidth() / 4.;
          Property property(0);
          property.setName(settingsConfig.getLneHrzGap());
          property.setValue(horizontalGap, dataType->getDataTypeDouble());
          properties.addProperty(property);
        }
        {
          double verticalGap = newGeographicBox.getHeight() / 4.;
          Property property(0);
          property.setName(settingsConfig.getLneVrtGap());
          property.setValue(verticalGap, dataType->getDataTypeDouble());
          properties.addProperty(property);
        }
        {
          Property property(0);
          property.setName(settingsConfig.getInitialGridPointX());
          property.setValue(newGeographicBox.getLowerLeftX(), dataType->getDataTypeDouble());
          properties.addProperty(property);
        }
        {
          Property property(0);
          property.setName(settingsConfig.getInitialGridPointY());
          property.setValue(newGeographicBox.getLowerLeftY(), dataType->getDataTypeDouble());
          properties.addProperty(property);
        }

        m_gridPropertiesInitialized = true;
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

        double distance = newGeographicBox.getWidth();

        initialX = adjustInitialX(newGeographicBox, initialX, gapVrt);
        {
          Property property(0);
          property.setName(settingsConfig.getInitialGridPointX());
          property.setValue(initialX, dataType->getDataTypeDouble());
          properties.addProperty(property);
        }

        distance = newGeographicBox.getHeight();

        initialY = adjustInitialY(newGeographicBox, initialY, gapHrz);
        {
          Property property(0);
          property.setName(settingsConfig.getInitialGridPointY());
          property.setValue(initialY, dataType->getDataTypeDouble());
          properties.addProperty(property);
        }

      }
      //te::gm::Envelope defaultPlanarBox(0, 0, 10000, 10000);
      if (newGeographicBox.equals(defaultGeographicBox) == false)
      {
        if (!newGeographicBox.isValid())
        {
          newGeographicBox = defaultGeographicBox;
          {
            Property property(0);
            property.setName("planar_box");
            property.setValue(newGeographicBox, dataType->getDataTypeEnvelope());
            properties.updateProperty(property);
          }
        }
      }
    }

    setProperties(properties);
  }
}

te::gm::Envelope te::layout::GridGeodesicController::getWorldBoxInGeographic(const te::gm::Envelope& worldBox, int srid)
{
  te::gm::Envelope worldBoxGeographic = worldBox;

  //About units names (SI): terralib5\resources\json\uom.json 
  te::layout::Utils utils(0);
  te::common::UnitOfMeasurePtr unitPtr = utils.unitMeasure(srid);

  if (!unitPtr)
    return worldBoxGeographic;

  std::string unitPtrStr = unitPtr->getName();
  unitPtrStr = te::common::Convert2UCase(unitPtrStr);

  if (unitPtrStr.compare("DEGREE") != 0)
  {
    std::string proj4 = utils.proj4DescToGeodesic();

    // Get the id of the projection of destination 
    std::pair<std::string, unsigned int> projGeographic = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4);

    // Remapping 
    worldBoxGeographic.transform(srid, projGeographic.second);
  }

  return worldBoxGeographic;
}
