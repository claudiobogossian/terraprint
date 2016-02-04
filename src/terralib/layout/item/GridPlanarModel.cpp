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
  \file GridPlanarModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridPlanarModel.h"

#include "../core/pattern/singleton/Context.h"
#include "../core/property/GridSettingsConfigProperties.h"
#include "../core/property/SharedProperties.h"
#include "../core/property/Properties.h"
#include "../core/pattern/mvc/AbstractItemView.h"
#include "../core/pattern/mvc/AbstractItemController.h"
#include "../core/property/PlanarGridSettingsConfigProperties.h"
#include "terralib/common/StringUtils.h"
#include "terralib/common/UnitOfMeasure.h"
#include "terralib/srs/SpatialReferenceSystemManager.h"
#include "terralib/maptools/Utils.h"

// STL
#include <string>

te::layout::GridPlanarModel::GridPlanarModel()
  : GridMapModel()
  , Observer()
{
  LayoutUnit unit(StyleKilometer);
  te::gm::Envelope planarBox(0, 0, 10000, 10000);

  m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getGridPlanarItem());

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  PlanarGridSettingsConfigProperties settingsConfig;

  Property prop_gridsettings = m_properties.getProperty("GridSettings");

  bool superscript = false;

  //adding properties
  /*{
    Property property(0);
    property.setName(settingsConfig.getUnit());
    property.setLabel(TR_LAYOUT("Unit"));
    property.setValue((int)unit, dataType->getDataTypeInt());
    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }*/

  {
    Property property(0);
    property.setName(settingsConfig.getUnit());
    property.setLabel(TR_LAYOUT("Unit"));
    property.setValue(std::string("km"), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(std::string("km"), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    v.setValue(std::string("m"), dataType->getDataTypeString());
    property.addOption(v);

    m_properties.addSubProperty(prop_gridsettings, property); // update gridsettings property
  }

  // Reference Settings
  {
    Property property(0);
    property.setName("planar_box");
    property.setLabel(TR_LAYOUT("Planar Box"));
    property.setVisible(false);
    property.setValue(planarBox, dataType->getDataTypeEnvelope());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getSuperscriptText());
    property.setLabel(TR_LAYOUT("Superscript"));
    property.setComposeWidget(true);
    property.setVisible(true); 
    property.setValue(superscript, dataType->getDataTypeBool());
    m_properties.completelyUpdateProperty(property);

  }
}

te::layout::GridPlanarModel::~GridPlanarModel()
{
}

void te::layout::GridPlanarModel::update(const Subject* subject)
{
  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if(subjectModel == 0)
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

  if(newScale == 0)
  {
    newScale = 1000;
  }

  //current values
  double currentWidth = pCurrentWidth.getValue().toDouble();
  double currentHeight = pCurrentHeight.getValue().toDouble();
  te::gm::Envelope currentPlanarBox = pCurrentPlanarBox.getValue().toEnvelope();
  const std::string currentItemObservable = pCurrentItemObserver.getValue().toString();

  bool doUpdate = false;
  if(newWidth != currentWidth)
  {
    doUpdate = true;
  }
  else if(newHeight != currentHeight)
  {
    doUpdate = true;
  }
  else if(newPlanarBox.equals(currentPlanarBox) == false)
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

    te::gm::Envelope defaultPlanarBox(0, 0, 10000, 10000);
    if(newPlanarBox.equals(defaultPlanarBox) == false)
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
