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
  \file GridGeodesicModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridGeodesicModel.h"

#include "../core/enum/EnumTextFormatType.h"
#include "../core/pattern/singleton/Context.h"
#include "../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../core/property/SharedProperties.h"
#include "../core/property/Properties.h"
#include "../core/pattern/mvc/AbstractItemView.h"
#include "../core/pattern/mvc/AbstractItemController.h"
#include <terralib/common/StringUtils.h>
#include <terralib/common/UnitOfMeasure.h>
#include <terralib/srs/SpatialReferenceSystemManager.h>

// STL
#include <string>

te::layout::GridGeodesicModel::GridGeodesicModel() 
  : GridMapModel()
  , Observer()
  , m_gridPropertiesInitialized(false)
{
  m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getGridGeodesicItem());

  te::gm::Envelope geographicBox(0, 0, 10000, 10000);
  bool showDegreesText = true;
  bool showMinutesText = true;
  bool showSecondsText = true;
  bool showBouderIntersections = true;
  int secPresicion = 0;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  GeodesicGridSettingsConfigProperties settingsConfig;

  //adding properties
  // Reference Settings
  {
    Property property(0);
    property.setName("geographic_box");
    property.setLabel(TR_LAYOUT("Geographic Box"));
    property.setVisible(false);
    property.setValue(geographicBox, dataType->getDataTypeEnvelope());
    m_properties.addProperty(property);
  }

  Property prop_gridsettings = m_properties.getProperty("GridSettings");
  
  {
    Property property(0);
    property.setName(settingsConfig.getDegreesText());
    property.setLabel(TR_LAYOUT("Show Degrees Text"));
    property.setValue(showDegreesText, dataType->getDataTypeBool());
    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }
  {
    Property property(0);
    property.setName(settingsConfig.getMinutesText());
    property.setLabel(TR_LAYOUT("Show Minutes Text"));
    property.setValue(showMinutesText, dataType->getDataTypeBool());
    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }
  {
    Property property(0);
    property.setName(settingsConfig.getSecondsText());
    property.setLabel(TR_LAYOUT("Show Seconds Text"));
    property.setValue(showSecondsText, dataType->getDataTypeBool());
    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }
  {
    //seconds precision
    Property property(0);
    property.setName(settingsConfig.getSecondsPrecisionText());
    property.setLabel(TR_LAYOUT("Seconds Text Precision"));
    property.setValue(secPresicion, dataType->getDataTypeInt());
    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }
  {
    Property property(0);
    property.setName(settingsConfig.getBouderIntersections());
    property.setLabel(TR_LAYOUT("Show Bouder Intersection"));
    property.setValue(showBouderIntersections, dataType->getDataTypeBool());
    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }

  // Text Format
  {
    EnumTextFormatType textFormatType;
    EnumType* defaultTextFormat = textFormatType.getDefaultFormat();

    Property property(0);
    property.setName(settingsConfig.getTextFormat());
    property.setLabel(TR_LAYOUT("Text Format"));
    property.setComposeWidget(true);
    property.setValue(defaultTextFormat->getLabel(), dataType->getDataTypeStringList());

    for (int i = 0; i < textFormatType.size(); ++i)
    {
      EnumType* enumType = textFormatType.getEnum(i);

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);

      if (defaultTextFormat->getName() == enumType->getName())
      {
        property.setOptionChoice(v);
      }
    }

    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }
}

te::layout::GridGeodesicModel::~GridGeodesicModel()
{
}


void te::layout::GridGeodesicModel::update(const Subject* subject)
{
  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if(subjectModel == 0)
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
  te::gm::Envelope newGeographicBox = getWorldBoxInGeographic(newWorldBox, newSrid);
  std::string newItemObservable = pNewItemObserver.getValue().toString();
 

  //current values
  double currentWidth = pCurrentWidth.getValue().toDouble();
  double currentHeight = pCurrentHeight.getValue().toDouble();
  te::gm::Envelope currentGeographicBox = pCurrentGeographicBox.getValue().toEnvelope();
  std::string currentItemObservable = pCurrentItemObserver.getValue().toString();

  bool doUpdate = false;
  if(newWidth != currentWidth)
  {
    doUpdate = true;
  }
  else if(newHeight != currentHeight)
  {
    doUpdate = true;
  }
  else if(newGeographicBox.equals(currentGeographicBox) == false)
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
  

  if(doUpdate == true)
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
    if(newGeographicBox.equals(defaultGeographicBox) == false)
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

te::gm::Envelope te::layout::GridGeodesicModel::getWorldBoxInGeographic(const te::gm::Envelope& worldBox, int srid)
{
  te::gm::Envelope worldBoxGeographic = worldBox;

  //About units names (SI): terralib5\resources\json\uom.json 
  Utils utils(0, 0);
  te::common::UnitOfMeasurePtr unitPtr = utils.unitMeasure(srid);

  if(!unitPtr)
    return worldBoxGeographic;
  
  std::string unitPtrStr = unitPtr->getName(); 
  unitPtrStr = te::common::Convert2UCase(unitPtrStr);

  if(unitPtrStr.compare("DEGREE") != 0)
  {
    std::string proj4 = utils.proj4DescToGeodesic();

    // Get the id of the projection of destination 
    std::pair<std::string, unsigned int> projGeographic = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4); 

    // Remapping 
    worldBoxGeographic.transform(srid, projGeographic.second);
  }

  return worldBoxGeographic;
}
