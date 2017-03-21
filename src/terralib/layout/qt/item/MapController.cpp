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
#include "MapController.h"
#include "MapItem.h"
#include "../core/Grid.h"
#include "../core/Scene.h"
#include "../core/pattern/command/ChangePropertyCommand.h"
#include "../../core/ItemInputProxy.h"
#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/pattern/proxy/AbstractProxyProject.h"
#include "../../core/property/PlanarGridSettingsConfigProperties.h"
#include "../../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../../core/Value.h"

#include <terralib/core/translator/Translator.h>
#include <terralib/maptools/Utils.h>


// STL
#include <set>
#include <vector>
#include <map>

// Qt
#include <QGraphicsItem>

te::layout::MapController::MapController(AbstractItemModel* model, AbstractItemView* view)
  : AbstractItemController(model, view)
{

}

te::layout::MapController::~MapController()
{

}

void te::layout::MapController::recomposeExtent()
{
  const Property& pLayers = this->getProperty("layers");
  const Property& pSrid = this->getProperty("srid");

  const std::list<te::map::AbstractLayerPtr>& currentLayerList = te::layout::Property::GetValueAs< std::list<te::map::AbstractLayerPtr> >(pLayers);
  int srid = te::layout::Property::GetValueAs<int>(pSrid);

  if (currentLayerList.empty())
    return;

  te::gm::Envelope worldBox = te::map::GetExtent(currentLayerList, srid, false);


  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  Property pWorldBox;
  pWorldBox.setName("world_box");
  pWorldBox.setValue(worldBox, dataType->getDataTypeEnvelope());

  this->setProperty(pWorldBox);
}

void te::layout::MapController::zoom(const te::gm::Point& pointWorld, bool zoomIn)
{
  const Property& pWorldBox = this->getProperty("world_box");
  const te::gm::Envelope& worldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pWorldBox);

  te::gm::Envelope zoomedWorldBox = ItemUtils::calculateZoom(worldBox, 1.25, zoomIn, pointWorld);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  Property pWorldBoxZoomed;
  pWorldBoxZoomed.setName("world_box");
  pWorldBoxZoomed.setValue(zoomedWorldBox, dataType->getDataTypeEnvelope());

  this->setProperty(pWorldBoxZoomed);
}

void te::layout::MapController::zoom(const te::gm::Envelope& box)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pWorldBoxZoomed;
  pWorldBoxZoomed.setName("world_box");
  pWorldBoxZoomed.setValue(box, dataType->getDataTypeEnvelope());

  this->setProperty(pWorldBoxZoomed); 
}

void te::layout::MapController::pan(double dx, double dy)
{
  const Property& pWorldBox = this->getProperty("world_box");
  const te::gm::Envelope& worldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pWorldBox);

  te::gm::Envelope panWorldBox = ItemUtils::calculatePan(worldBox, dx, dy);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pWorldBoxPan;
  pWorldBoxPan.setName("world_box");
  pWorldBoxPan.setValue(panWorldBox, dataType->getDataTypeEnvelope());

  this->setProperty(pWorldBoxPan);
}

void te::layout::MapController::addLayers(const std::list<te::map::AbstractLayerPtr>& newLayerList)
{
  const Property& pCurrentLayers = this->getProperty("layers");
  std::list<te::map::AbstractLayerPtr> currentLayersList = te::layout::Property::GetValueAs< std::list<te::map::AbstractLayerPtr> >(pCurrentLayers);
  std::list<te::map::AbstractLayerPtr>::const_iterator itNewList = newLayerList.begin();
  while (itNewList != newLayerList.end())
  {
    bool add = true;
    std::list<te::map::AbstractLayerPtr>::const_iterator itCurrentList = currentLayersList.begin();
    while (itCurrentList != currentLayersList.end())
    {
      if (itNewList->get()->getId() == itCurrentList->get()->getId())
      {
        add = false;
        break;
      }

      ++itCurrentList;
    }
    if (add == true)
    {
      currentLayersList.push_back(*itNewList);
    }

    ++itNewList;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  Property property;
  property.setName("layers");
  property.setValue(currentLayersList, dataType->getDataTypeLayerList());

  Properties beforeProps = getProperties();

  setProperty(property);
}

void te::layout::MapController::setProperty(const te::layout::Property& property)
{
  te::layout::Properties properties;
  properties.addProperty(property);

  setProperties(properties);
}

void te::layout::MapController::setProperties(const te::layout::Properties& properties)
{
  //we first copy the properties that are being set
  te::layout::Properties propertiesCopy(properties);

  //the first thing we need to do is adjust the size of the map (without the grids) and the size total size (with the grids)
  if (adjustMapSizeProperties(propertiesCopy) == true)
  {
  }

  //then we sync the properties related to the layers
  if (syncLayersProperties(propertiesCopy) == true)
  {
  }

  //then we sync the properties related to the SRID and Envelope
  if(syncSridAndEnvelope(propertiesCopy) == true)
  {
  }

  //now we sync the properties related to the size of the map
  if(syncMapSizeProperties(propertiesCopy) == true)
  {
  }

  //now we sync the properties related to the scale
  if (syncMapScaleProperties(propertiesCopy) == true)
  {
  }

  //now we sync the properties related to the planar grid initialization
  if (syncPlanarGridInitProperties(propertiesCopy) == true)
  {
  }

  //now we sync the properties related to the geodesic grid initialization
  if (syncGeodesicGridInitProperties(propertiesCopy) == true)
  {
  }

  //now we sync the properties related to the grid reference box
  if (syncGridReferenceProperties(propertiesCopy) == true)
  {
  }  

  //we finally set the properties into the model
  AbstractItemController::setProperties(propertiesCopy);
}

std::list<te::map::AbstractLayerPtr> te::layout::MapController::getIntersection(const std::list<te::map::AbstractLayerPtr>& listA, const std::list<te::map::AbstractLayerPtr>& listB) const
{
  //checks the intersection between the layers list in the model and the new layer list to be set
  std::list<te::map::AbstractLayerPtr> intersectionList;

  std::list<te::map::AbstractLayerPtr>::const_iterator itListA = listA.begin();
  while (itListA != listA.end())
  {
    std::list<te::map::AbstractLayerPtr>::const_iterator itListB = listB.begin();
    while (itListB != listB.end())
    {
      if (itListA->get()->getId() == itListB->get()->getId())
      {
        intersectionList.push_back(itListA->get());
        break;
      }
      ++itListB;
    }
    ++itListA;
  }

  return intersectionList;
}

te::layout::Property te::layout::MapController::syncLayersFromURIs(const Property& property)
{
  Property prop;

  std::list<te::map::AbstractLayerPtr> layerList;
  std::vector<std::string>  vString;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if (property.getName().compare("layers_uri") == 0)
  {
    vString = te::layout::Property::GetValueAs< std::vector<std::string> >(property);

    AbstractProxyProject* project = getAbstractProxyProject();
    if (!project)
    {
      return prop;
    }

    // search layers 
    for (std::vector<std::string>::iterator it = vString.begin(); it != vString.end(); ++it)
    {
      std::string uri = (*it);
      te::map::AbstractLayerPtr layer = project->getLayerFromURI(uri);
      if (layer)
      {
        layerList.push_back(layer);
      }
    }
    prop = m_model->getProperty("layers");
    prop.setValue(layerList, dataType->getDataTypeLayerList());
  }
  return prop;
}

te::layout::Property te::layout::MapController::syncURIsFromLayers(const Property& property)
{
  Property prop;
  std::vector<std::string>  vString;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if (property.getName().compare("layers") == 0)
  {
    const std::list<te::map::AbstractLayerPtr>& layerList = te::layout::Property::GetValueAs< std::list<te::map::AbstractLayerPtr> >(property);

    AbstractProxyProject* project = getAbstractProxyProject();
    if (!project)
    {
      return prop;
    }

    // search layer info (uri)
    for (std::list<te::map::AbstractLayerPtr>::const_iterator it = layerList.begin(); it != layerList.end(); ++it)
    {
      te::map::AbstractLayerPtr layer = (*it);
      std::string uri = project->getURIFromLayer(layer);
      vString.push_back(uri);
    }
    prop = m_model->getProperty("layers_uri");
    prop.setValue(vString, dataType->getDataTypeStringVector());
  }
  return prop;
}

bool te::layout::MapController::syncLayersProperties(Properties& properties)
{
  //if the property 'layers_uri' is set, we must try to initialize the 'layers' property based on it
  //we also check the opposite
  bool containsUri = properties.contains("layers_uri");
  bool containsLayers = properties.contains("layers");

  //we first check if only one of the properties related to the layers are set. If so, we sinchronize both values
  Property propertyToUpdate;
  if (containsUri == true && containsLayers == false)
  {
    const Property& pUri = properties.getProperty("layers_uri");
    propertyToUpdate = syncLayersFromURIs(pUri);
  }
  else if (containsUri == false && containsLayers == true)
  {
    const Property& pLayers = properties.getProperty("layers");
    propertyToUpdate = syncURIsFromLayers(pLayers);
  }

  //if we were able to sync the property, we add or update its value inside the Properties container
  if (propertyToUpdate.isNull() == true)
  {
    //there is nothing to update
    return false;
  }

  ItemUtils::addOrUpdateProperty(propertyToUpdate, properties);

  return true;
}

bool te::layout::MapController::syncSridAndEnvelope(Properties& properties)
{
  //if the new layer list does not have any intersection to the current layer list, we must also ensure that "srid" and "world_box" properties are initialized
  const Property& pCurrentLayerList = this->getProperty("layers");

  //if the list of layers has not been changed, there is nothing to do
  if (properties.contains("layers") == false)
  {
    return false;
  }

  const Property& pNewLayerList = properties.getProperty("layers");

  const std::list<te::map::AbstractLayerPtr>& currentLayerList = te::layout::Property::GetValueAs< std::list<te::map::AbstractLayerPtr> >(pCurrentLayerList);
  const std::list<te::map::AbstractLayerPtr>& newLayerList = te::layout::Property::GetValueAs< std::list<te::map::AbstractLayerPtr> >(pNewLayerList);

  //if there is an intersection, we do not try to initialize the "srid" and "world_box" properties
  std::list<te::map::AbstractLayerPtr> intersectionList = getIntersection(currentLayerList, newLayerList);
  if (intersectionList.empty() == false)
  {
    //there is nothing to update
    return false;
  }

  //if the list is entirely new, we must ensure that the "srid" and "world_box" properties will be initialized
  //if both values are set, we do nothing
  if(properties.contains("srid") == true && properties.contains("world_box") == true)
  {
    const Property& propSRID = properties.getProperty("srid");
    const Property& propBox = properties.getProperty("world_box");

    int numSRID = te::layout::Property::GetValueAs<int>(propSRID);
    const te::gm::Envelope& box = Property::GetValueAs<te::gm::Envelope >(propBox);

    //there is nothing to calculate because the current srid is planar
    int checkSRID = Utils::toPlanar(box, numSRID);
    if (checkSRID == numSRID)
    {
      return false;
    }
  }

  //if one of theme is not set, we calculate both values
  int srid = 0;
  te::gm::Envelope envelope;

  std::list<te::map::AbstractLayerPtr>::const_iterator it = newLayerList.begin();
  while (it != newLayerList.end())
  {
    te::map::AbstractLayerPtr layer = (*it);

    te::gm::Envelope currentEnvelope = layer->getExtent();

    int currentSrid = layer->getSRID();
    if (srid <= 0)
    {
      srid = Utils::toPlanar(currentEnvelope, currentSrid);
    }
    if (currentSrid != srid)
      currentEnvelope.transform(currentSrid, srid);

    envelope.Union(currentEnvelope);

    ++it;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  {
    Property property;
    property.setName("srid");
    property.setValue(srid, dataType->getDataTypeInt());
    ItemUtils::addOrUpdateProperty(property, properties);
  }

  {
    Property property;
    property.setName("world_box");
    property.setValue(envelope, dataType->getDataTypeEnvelope());
    ItemUtils::addOrUpdateProperty(property, properties);
  }

  return true;
}

bool te::layout::MapController::adjustMapSizeProperties(Properties& properties)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //here we sync all properties related to the map
  bool containsMapLocalBox = properties.contains("map_local_box");
  bool containsWidth = properties.contains("width");
  bool containsHeight = properties.contains("height");

  //if the map local box is being set, we just need to reset the overall width and height
  if (containsMapLocalBox == true)
  {
    const Property& pMapLocalBox = getProperty("map_local_box", properties);
    const te::gm::Envelope& mapLocalBox = Property::GetValueAs<te::gm::Envelope >(pMapLocalBox);
    double resetWidth = mapLocalBox.getWidth();
    double resetHeight = mapLocalBox.getHeight();

    Property pWidth;
    pWidth.setName("width");
    pWidth.setValue(resetWidth, dataType->getDataTypeDouble());
    ItemUtils::addOrUpdateProperty(pWidth, properties);

    Property pHeight;
    pHeight.setName("height");
    pHeight.setValue(resetHeight, dataType->getDataTypeDouble());
    ItemUtils::addOrUpdateProperty(pHeight, properties);

    return true;
  }

  //if there were no chances, we just reset the width and height of the item based of the size of the map
  if (containsMapLocalBox == false && containsWidth == false && containsHeight == false)
  {
    const Property& pMapLocalBox = getProperty("map_local_box", properties);
    const te::gm::Envelope& mapLocalBox = Property::GetValueAs<te::gm::Envelope >(pMapLocalBox);
    double resetWidth = mapLocalBox.getWidth();
    double resetHeight = mapLocalBox.getHeight();
    te::gm::Envelope resetMapLocalBox(0, 0, resetWidth, resetHeight);

    Property pWidth;
    pWidth.setName("width");
    pWidth.setValue(resetWidth, dataType->getDataTypeDouble());
    ItemUtils::addOrUpdateProperty(pWidth, properties);

    Property pHeight;
    pHeight.setName("height");
    pHeight.setValue(resetHeight, dataType->getDataTypeDouble());
    ItemUtils::addOrUpdateProperty(pHeight, properties);

    Property pNewMapLocalBox;
    pNewMapLocalBox.setName("map_local_box");
    pNewMapLocalBox.setValue(resetMapLocalBox, dataType->getDataTypeEnvelope());
    ItemUtils::addOrUpdateProperty(pNewMapLocalBox, properties);

    return true;
  }

  //If we got here, the item has been resized and we must recalcute the map local box
  const Property& pMapLocalBox = getProperty("map_local_box", properties);
  const te::gm::Envelope& mapLocalBox = Property::GetValueAs<te::gm::Envelope >(pMapLocalBox);

  //for the width
  double oldItemWidth = Property::GetValueAs<double>(getProperty("width"));
  double newItemWidth = Property::GetValueAs<double>(getProperty("width", properties));
    
  double oldMapWidth = mapLocalBox.getWidth();
  double diffMapWidth = oldItemWidth - oldMapWidth;
  double newMapWidth = newItemWidth - diffMapWidth;

  Property pNewWidth;
  pNewWidth.setName("width");
  pNewWidth.setValue(newMapWidth, dataType->getDataTypeDouble());
  ItemUtils::addOrUpdateProperty(pNewWidth, properties);

  //for the height
  double oldItemHeight = Property::GetValueAs<double>(getProperty("height"));
  double newItemHeight = Property::GetValueAs<double>(getProperty("height", properties));

  double oldMapHeight = mapLocalBox.getHeight();
  double diffMapHeight = oldItemHeight - oldMapHeight;
  double newMapHeight = newItemHeight - diffMapHeight;

  Property pNewHeight;
  pNewHeight.setName("height");
  pNewHeight.setValue(newMapHeight, dataType->getDataTypeDouble());
  ItemUtils::addOrUpdateProperty(pNewHeight, properties);
  
  //for the map local box
  te::gm::Envelope resetMapLocalBox(0, 0, newMapWidth, newMapHeight);
  Property pNewMapLocalBox;
  pNewMapLocalBox.setName("map_local_box");
  pNewMapLocalBox.setValue(resetMapLocalBox, dataType->getDataTypeEnvelope());
  ItemUtils::addOrUpdateProperty(pNewMapLocalBox, properties);

  return true;
}


bool te::layout::MapController::syncMapSizeProperties(Properties& properties)
{
  //here we sync all properties related to the map
  bool containsWorldBox = properties.contains("world_box");
  bool containsWidth = properties.contains("width");
  bool containsHeight = properties.contains("height");

  //if there were no chances, we dont need to do nothing
  if (containsWorldBox == false && containsWidth == false && containsHeight == false)
  {
    //there is nothing to update
    return false;
  }

  //if the size is not being set, we must consider the current size of the item
  const Property& pMapLocalBox = getProperty("map_local_box", properties);

  //if the world_box is not being set, we must consider the current world_box of the item
  const Property& pWorldBox = getProperty("world_box", properties);
  const Property& pSrid = getProperty("srid", properties);
  const Property& pScale = getProperty("scale", properties);
  const Property& pFixedScale = getProperty("fixed_scale", properties);

  //so we must ensure that the aspect of the world box is proportional to the width and height
  const te::gm::Envelope& mapLocalBox = Property::GetValueAs<te::gm::Envelope >(pMapLocalBox);
  double widthMM = mapLocalBox.getWidth();
  double heightMM = mapLocalBox.getHeight();
  te::gm::Envelope worldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pWorldBox);
  int srid = te::layout::Property::GetValueAs<int>(pSrid);
  bool fixedScale = te::layout::Property::GetValueAs<bool>(pFixedScale);
  double scale = te::layout::Property::GetValueAs<double>(pScale);

  if (worldBox.isValid() == false)
  {
    return false;
  }

  //if the scale is fixed, we must ensure that it will not be changed
  if (fixedScale == true)
  {
    //we first calculate the aspect
    ItemUtils::calculateAspectRatio(widthMM, heightMM, worldBox);

    //then we recalculate the box in order to keep the current scale
    worldBox = ItemUtils::calculateBoxFromScale(widthMM, heightMM, srid, worldBox, scale);
  }
  else
  {
    ItemUtils::calculateAspectRatio(widthMM, heightMM, worldBox);
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  Property pNewWorldBox(pWorldBox);
  pNewWorldBox.setValue(worldBox, dataType->getDataTypeEnvelope());

  ItemUtils::addOrUpdateProperty(pNewWorldBox, properties);

  return true;
}

bool te::layout::MapController::syncMapScaleProperties(Properties& properties)
{

  //here we sync all properties related to the map
  bool containsWorldBox = properties.contains("world_box");
  bool containsWidth = properties.contains("width");
  bool containsHeight = properties.contains("height");
  bool containsScale = properties.contains("scale");

  //if there were no chances, we dont need to do nothing
  if (containsWorldBox == false && containsWidth == false && containsHeight == false && containsScale == false)
  {
    //there is nothing to update
    return false;
  }

  //if the size is not being set, we must consider the current size of the item
  const Property& pWidth = getProperty("width", properties);
  const Property& pHeight = getProperty("height", properties);

  //if the world_box is not being set, we must consider the current world_box of the item
  const Property& pWorldBox = getProperty("world_box", properties);
  const Property& pSrid = getProperty("srid", properties);
  const Property& pScale = getProperty("scale", properties);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  double widthMM = te::layout::Property::GetValueAs<double>(pWidth);
  double heightMM = te::layout::Property::GetValueAs<double>(pHeight);
  te::gm::Envelope worldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pWorldBox);
  int srid = te::layout::Property::GetValueAs<int>(pSrid);

  if (worldBox.isValid() == false)
  {
    return false;
  }

  //if the scale is NOT being set, we must compute its value
  //(else) if the scale is being set, we must recompute the world_box
  if (containsScale == false)
  {
    //here we calculate the current scale
    double scale = ItemUtils::calculateScaleFromBox(widthMM, heightMM, srid, worldBox);

    Property pNewScale;
    pNewScale.setName("scale");
    pNewScale.setValue(scale, dataType->getDataTypeDouble());
    ItemUtils::addOrUpdateProperty(pNewScale, properties);
  }
  else
  {
    double scale = te::layout::Property::GetValueAs<double>(pScale);

    //here we calculate the world_box based on the scale
    te::gm::Envelope newWorld = ItemUtils::calculateBoxFromScale(widthMM, heightMM, srid, worldBox, scale);

    Property pNewWorldBox;
    pNewWorldBox.setName("world_box");
    pNewWorldBox.setValue(newWorld, dataType->getDataTypeEnvelope());
    ItemUtils::addOrUpdateProperty(pNewWorldBox, properties);
  }

  return true;
}

bool te::layout::MapController::syncPlanarGridInitProperties(Properties& properties)
{
  //if the new layer list does not have any intersection to the current layer list, we must also ensure that "srid" and "world_box" properties are initialized
  const Property& pCurrentLayerList = this->getProperty("layers");

  //if the list of layers has not been changed, there is nothing to do
  if (properties.contains("layers") == false)
  {
    // check if initial line has to be recalculate (Ex.: Zoom Out)
    if (hasToRecalculatePlanarInitialLine(properties))
    {
      recalculatePlanarInitialLine(properties);
    }
    return false;
  }

  const Property& pNewLayerList = properties.getProperty("layers");

  const std::list<te::map::AbstractLayerPtr>& currentLayerList = te::layout::Property::GetValueAs< std::list<te::map::AbstractLayerPtr> >(pCurrentLayerList);
  const std::list<te::map::AbstractLayerPtr>& newLayerList = te::layout::Property::GetValueAs< std::list<te::map::AbstractLayerPtr> >(pNewLayerList);

  //if there is an intersection, we do not try to initialize the "srid" and "world_box" properties
  std::list<te::map::AbstractLayerPtr> intersectionList = getIntersection(currentLayerList, newLayerList);
  if (intersectionList.empty() == false)
  {
    //there is nothing to update
    return false;
  }

  PlanarGridSettingsConfigProperties planarGridSettings;
  if (properties.contains(planarGridSettings.getGridSettings()))
  {
    return false;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pWorldBox = getProperty("world_box", properties);
  const Property& pSrid = getProperty("srid", properties);
  const Property& pWidth = getProperty("width", properties);
  const Property& pHeight = getProperty("height", properties);

  const te::gm::Envelope& worldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pWorldBox);
  int srid = te::layout::Property::GetValueAs<int>(pSrid);
  double width = te::layout::Property::GetValueAs<double>(pWidth);
  double height = te::layout::Property::GetValueAs<double>(pHeight);

  if (worldBox.isValid() == false)
  {
    return false;
  }

  if (width == 0 || height == 0)
  {
    return false;
  }

  double horizontalLineGap = 0;
  double verticalLineGap = 0;

  int planarSRID = Utils::toPlanar(worldBox, srid);

  // Optimized way to reproject a box, between source and destination projections.

  //if first prepare the world box, ensuring that it is a planar box. If it is not, we reproject it
  te::gm::Envelope planarBox = Utils::worldBoxTo(worldBox, srid, planarSRID);

  //then we calculate the initial coordinates and the gaps
  double horizontalLineInitial = this->getInitialCoord(planarBox.getLowerLeftY(), planarBox.getHeight(), horizontalLineGap);
  {
    Property property(0);
    property.setName(planarGridSettings.getHorizontalLineInitial());
    property.setValue(horizontalLineInitial, dataType->getDataTypeDouble());

    ItemUtils::addOrUpdateProperty(property, properties);
  }
  {
    Property property(0);
    property.setName(planarGridSettings.getHorizontalLineGap());
    property.setValue(horizontalLineGap, dataType->getDataTypeDouble());

    ItemUtils::addOrUpdateProperty(property, properties);
  }

  double verticalLineInitial = this->getInitialCoord(planarBox.getLowerLeftX(), planarBox.getWidth(), verticalLineGap);
  {
    Property property(0);
    property.setName(planarGridSettings.getVerticalLineInitial());
    property.setValue(verticalLineInitial, dataType->getDataTypeDouble());

    ItemUtils::addOrUpdateProperty(property, properties);
  }
  {
    Property property(0);
    property.setName(planarGridSettings.getVerticalLineGap());
    property.setValue(verticalLineGap, dataType->getDataTypeDouble());
    
    ItemUtils::addOrUpdateProperty(property, properties);
  }
  {
    Property property(0);
    property.setName(planarGridSettings.getPlanarSRID());
    property.setValue(planarSRID, dataType->getDataTypeInt()); // set or update the planar SRID

    ItemUtils::addOrUpdateProperty(property, properties);
  }

  return true;
}

bool te::layout::MapController::syncGeodesicGridInitProperties(Properties& properties)
{
  //if the new layer list does not have any intersection to the current layer list, we must also ensure that "srid" and "world_box" properties are initialized
  const Property& pCurrentLayerList = this->getProperty("layers");

  //if the list of layers has not been changed, there is nothing to do
  if (properties.contains("layers") == false)
  {
    // check if initial line has to be recalculate (Ex.: Zoom Out)
    if (hasToRecalculateGeodesicInitialLine(properties))
    {
      recalculateGeodesicInitialLine(properties);
    }
    return false;
  }

  const Property& pNewLayerList = properties.getProperty("layers");

  const std::list<te::map::AbstractLayerPtr>& currentLayerList = te::layout::Property::GetValueAs< std::list<te::map::AbstractLayerPtr> >(pCurrentLayerList);
  const std::list<te::map::AbstractLayerPtr>& newLayerList = te::layout::Property::GetValueAs< std::list<te::map::AbstractLayerPtr> >(pNewLayerList);

  //if there is an intersection, we do not try to initialize the "srid" and "world_box" properties
  std::list<te::map::AbstractLayerPtr> intersectionList = getIntersection(currentLayerList, newLayerList);
  if (intersectionList.empty() == false)
  {
    //there is nothing to update
    return false;
  }

  GeodesicGridSettingsConfigProperties geodesicGridSettings;
  if (properties.contains(geodesicGridSettings.getGridSettings()))
  {
    return false;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pWorldBox = getProperty("world_box", properties);
  const Property& pSrid = getProperty("srid", properties);
  const Property& pWidth = getProperty("width", properties);
  const Property& pHeight = getProperty("height", properties);

  const te::gm::Envelope& worldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pWorldBox);
  int srid = te::layout::Property::GetValueAs<int>(pSrid);
  double width = te::layout::Property::GetValueAs<double>(pWidth);
  double height = te::layout::Property::GetValueAs<double>(pHeight);

  if (worldBox.isValid() == false)
  {
    return false;
  }

  if (width == 0 || height == 0)
  {
    return false;
  }

  int planarSRID = Utils::toPlanar(worldBox, srid);
  int geodesicSRID = Utils::toGeographic(worldBox, srid);
   
  // Optimized way to reproject a box, between source and destination projections.

  //if first prepare the world box, ensuring that it is a geographic box. If it is not, we reproject it
  te::gm::Envelope geographicBox = Utils::worldBoxTo(worldBox, srid, geodesicSRID);
  
  //then we calculate the initial coordinates and the gaps
  double horizontalLineGap = geographicBox.getHeight() / 4.;
  double horizontalLineInitial = geographicBox.getLowerLeftY();
  
  double verticalLineGap = geographicBox.getWidth() / 4.;
  double verticalLineInitial = geographicBox.getLowerLeftX();

  {
    Property property(0);
    property.setName(geodesicGridSettings.getHorizontalLineInitial());
    property.setValue(horizontalLineInitial, dataType->getDataTypeDouble());

    ItemUtils::addOrUpdateProperty(property, properties);
  }
  {
    Property property(0);
    property.setName(geodesicGridSettings.getHorizontalLineGap());
    property.setValue(horizontalLineGap, dataType->getDataTypeDouble());

    ItemUtils::addOrUpdateProperty(property, properties);
  }

  {
    Property property(0);
    property.setName(geodesicGridSettings.getVerticalLineInitial());
    property.setValue(verticalLineInitial, dataType->getDataTypeDouble());

    ItemUtils::addOrUpdateProperty(property, properties);
  }
  {
    Property property(0);
    property.setName(geodesicGridSettings.getVerticalLineGap());
    property.setValue(verticalLineGap, dataType->getDataTypeDouble());

    ItemUtils::addOrUpdateProperty(property, properties);
  }
  {
    Property property(0);
    property.setName(geodesicGridSettings.getPlanarSRID());
    property.setValue(planarSRID, dataType->getDataTypeInt()); // set or update the planar SRID

    ItemUtils::addOrUpdateProperty(property, properties);
  }
  {
    Property property(0);
    property.setName(geodesicGridSettings.getGeodesicSRID());
    property.setValue(geodesicSRID, dataType->getDataTypeInt()); // set or update the geodesic SRID

    ItemUtils::addOrUpdateProperty(property, properties);
  }

  return true;
}

bool te::layout::MapController::syncGridReferenceProperties(Properties& properties)
{
  MapItem* mapItem = dynamic_cast<MapItem*>(m_view);
  if (mapItem == 0)
  {
    return false;
  }

  Properties fullProperties = this->getProperties();

  mergeProperties(properties, fullProperties);

  const Property& pMapLocalBox = getProperty("map_local_box", fullProperties);
  const te::gm::Envelope& mapLocalBox = Property::GetValueAs<te::gm::Envelope >(pMapLocalBox);
  double mapWidth = mapLocalBox.getWidth();
  double mapHeight = mapLocalBox.getHeight();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pWidth;
  pWidth.setName("width");
  pWidth.setValue(mapWidth, dataType->getDataTypeDouble());
  ItemUtils::addOrUpdateProperty(pWidth, fullProperties);

  Property pHeight;
  pHeight.setName("height");
  pHeight.setValue(mapHeight, dataType->getDataTypeDouble());
  ItemUtils::addOrUpdateProperty(pHeight, fullProperties);

  te::layout::Grid* planarGrid = mapItem->getPlanarGrid();
  planarGrid->initialize(fullProperties);

  te::layout::Grid* geodesicGrid = mapItem->getGeodesicGrid();
  geodesicGrid->initialize(fullProperties);

  QPointF planarOriginPoint = planarGrid->getOrigin();
  QPointF planarFinalPoint = planarGrid->getFinal();
  QSizeF planarSize = planarGrid->getSize();

  QPointF geodesicOriginPoint = geodesicGrid->getOrigin();
  QPointF geodesicFinalPoint = geodesicGrid->getFinal();
  QSizeF geodesicSize = geodesicGrid->getSize();

  QSizeF mapSize(mapWidth, mapHeight);

  QPointF gridOriginPoint(qMax(planarOriginPoint.x(), geodesicOriginPoint.x()), qMax(planarOriginPoint.y(), geodesicOriginPoint.y()));
  QPointF gridFinalPoint(qMax(planarFinalPoint.x(), geodesicFinalPoint.x()), qMax(planarFinalPoint.y(), geodesicFinalPoint.y()));

  // Calculate the entire bounding rect of the item (map plus grid)
  te::gm::Envelope outsideBoundingBox = calculateOutsideBoundingBox(gridOriginPoint, gridFinalPoint, mapSize);

  // new map bounding rect
  te::gm::Envelope newMapLocalBox(gridOriginPoint.x(), gridOriginPoint.y(), gridOriginPoint.x() + mapWidth, gridOriginPoint.y() + mapHeight);

  Property pNewWidth;
  pNewWidth.setName("width");
  pNewWidth.setValue(outsideBoundingBox.getWidth(), dataType->getDataTypeDouble());
  ItemUtils::addOrUpdateProperty(pNewWidth, properties);

  Property pNewHeight;
  pNewHeight.setName("height");
  pNewHeight.setValue(outsideBoundingBox.getHeight(), dataType->getDataTypeDouble());
  ItemUtils::addOrUpdateProperty(pNewHeight, properties);

  Property pNewMapLocalBox;
  pNewMapLocalBox.setName("map_local_box");
  pNewMapLocalBox.setValue(newMapLocalBox, dataType->getDataTypeEnvelope());
  ItemUtils::addOrUpdateProperty(pNewMapLocalBox, properties);

  return true;
}

te::layout::AbstractProxyProject* te::layout::MapController::getAbstractProxyProject()
{
  AbstractProxyProject* project = 0;
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if (!view)
  {
    return project;
  }

  ItemInputProxy* itemInputProxy = m_view->getItemInputProxy();
  if (itemInputProxy == 0)
  {
    return project;
  }

  Value<AbstractProxyProject* >* value = itemInputProxy->getContextValues<AbstractProxyProject *>("proxy_project");
  if (value)
  {
    project = value->get();
  }
  return project;
}


void te::layout::MapController::validateItem()
{
  this->getWarningManager()->clearWarnings();

  const Property& pLayers = this->getProperty("layers");
  std::list<te::map::AbstractLayerPtr> currentLayerList;
  currentLayerList = te::layout::Property::GetValueAs< std::list<te::map::AbstractLayerPtr> >(pLayers);

  if (currentLayerList.empty())
    return;

  const Property& pSrid = this->getProperty("srid");
  int srid = te::layout::Property::GetValueAs<int>(pSrid);

  if (srid <= 0)
  {
    m_warningManager->addWarning(TE_TR("Map Item Without Projection"));
  }


  for (std::list<te::map::AbstractLayerPtr>::iterator it = currentLayerList.begin(); it != currentLayerList.end(); ++it)
  {

    te::map::AbstractLayerPtr layer = (*it);
    int layerSrid = layer->getSRID();

    if (layerSrid <= 0)
    {
      std::string warningMSG = TE_TR("Layer ") + layer->getTitle() + TE_TR(" Without Projection");
      m_warningManager->addWarning(warningMSG);
    }
  }
}

void te::layout::MapController::sceneHasChanged(Scene* scene)
{
  if (scene == 0)
  {
    return;
  }

  //as the item has been added to the scene, we must check if the layer list is initialized (the initialization needs a valid scene)
  const Property& pLayers = this->getProperty("layers");
  const Property& pLayersUri = this->getProperty("layers_uri");

  std::list<te::map::AbstractLayerPtr> currentLayerList = te::layout::Property::GetValueAs< std::list<te::map::AbstractLayerPtr> >(pLayers);
  std::vector<std::string> vecUri = te::layout::Property::GetValueAs< std::vector<std::string> >(pLayersUri);

  if (vecUri.empty() == false && vecUri.size() != currentLayerList.size())
  {
    //if we must initialize it, we just set again the URI list
    //as we are just synchronizing a value, we dont need to generate an undo/redo command
    m_view->setUndoEnabled(false);
    setProperty(pLayersUri);
    m_view->setUndoEnabled(true);
  }
}

void te::layout::MapController::changedPropertyLayerURIFromDropEvent(const Properties& beforeProps)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  std::vector<QGraphicsItem*> commandItems;
  std::vector<Properties> commandOld;
  std::vector<Properties> commandNew;
  
  Properties afterProps = getProperties();
  QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(m_view);
  if (item)
  {
    commandItems.push_back(item);
  }
  commandOld.push_back(beforeProps);
  commandNew.push_back(afterProps);

  QUndoCommand* command = new ChangePropertyCommand(commandItems, commandOld, commandNew);
  if (item)
  {
    Scene* scene = dynamic_cast<Scene*>(item->scene());
    if (scene)
    {
      scene->addUndoStack(command);
    }    
  }
}

double te::layout::MapController::getInitialCoord(double initialCoord, double distance, double& gap) const
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

void te::layout::MapController::recalculatePlanarInitialLine(Properties& properties)
{
  /* check if initial line has to be recalculate (Ex.: Zoom Out) */

  PlanarGridSettingsConfigProperties planarGridSettings;
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pWorldBox = getProperty("world_box", properties);
  const te::gm::Envelope& worldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pWorldBox);
  if (!worldBox.isValid())
  {
    return;
  }

  const Property& pSrid = m_model->getProperty("srid");
  int srid = te::layout::Property::GetValueAs<int>(pSrid);

  const Property& pHorizontalLineGap = m_model->getProperty(planarGridSettings.getHorizontalLineGap());
  double horizontalLineGap = te::layout::Property::GetValueAs<double>(pHorizontalLineGap);

  const Property& pVerticalLineGap = m_model->getProperty(planarGridSettings.getVerticalLineGap());
  double verticalLineGap = te::layout::Property::GetValueAs<double>(pVerticalLineGap);

  const Property& pPlanarSRID = m_model->getProperty(planarGridSettings.getPlanarSRID());
  int planarSRID = te::layout::Property::GetValueAs<int>(pPlanarSRID);

  te::gm::Envelope planarBox = Utils::worldBoxTo(worldBox, srid, planarSRID);

  const std::string& nHorizontalLineInitial = planarGridSettings.getHorizontalLineInitial();
  const std::string& nVerticalLineInitial = planarGridSettings.getVerticalLineInitial();
  recalculateInitialLine(properties, planarBox, horizontalLineGap, verticalLineGap, nHorizontalLineInitial, nVerticalLineInitial);
}

void te::layout::MapController::recalculateGeodesicInitialLine(Properties& properties)
{
  /* check if initial line has to be recalculate (Ex.: Zoom Out) */

  GeodesicGridSettingsConfigProperties geodesicGridSettings;
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pWorldBox = getProperty("world_box", properties);
  const te::gm::Envelope& worldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pWorldBox);
  if (!worldBox.isValid())
  {
    return;
  }
  
  const Property& pSrid = m_model->getProperty("srid");
  int srid = te::layout::Property::GetValueAs<int>(pSrid);

  const Property& pHorizontalLineGap = m_model->getProperty(geodesicGridSettings.getHorizontalLineGap());
  double horizontalLineGap = te::layout::Property::GetValueAs<double>(pHorizontalLineGap);

  const Property& pVerticalLineGap = m_model->getProperty(geodesicGridSettings.getVerticalLineGap());
  double verticalLineGap = te::layout::Property::GetValueAs<double>(pVerticalLineGap);

  const Property& pGeodesicSRID = m_model->getProperty(geodesicGridSettings.getGeodesicSRID());
  int geodesicSRID = te::layout::Property::GetValueAs<int>(pGeodesicSRID);

  te::gm::Envelope geographicBox = Utils::worldBoxTo(worldBox, srid, geodesicSRID);
    
  const std::string& nHorizontalLineInitial = geodesicGridSettings.getHorizontalLineInitial();
  const std::string& nVerticalLineInitial = geodesicGridSettings.getVerticalLineInitial();
  recalculateInitialLine(properties, geographicBox, horizontalLineGap, verticalLineGap, nHorizontalLineInitial, nVerticalLineInitial);
}

void te::layout::MapController::recalculateInitialLine(Properties& properties, const te::gm::Envelope& worldBox,
  double horizontalLineGap, double verticalLineGap,
  const std::string& nHorizontalLineInitial, const std::string& nVerticalLineInitial)
{
  if (!worldBox.isValid())
  {
    return;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  const Property& pSrid = m_model->getProperty("srid");
  int srid = te::layout::Property::GetValueAs<int>(pSrid);

  // get initial coordenates
  const Property& pHorizontalLineInitial = m_model->getProperty(nHorizontalLineInitial);
  double horizontalLineInitial = te::layout::Property::GetValueAs<double>(pHorizontalLineInitial);

  const Property& pVerticalLineInitial = m_model->getProperty(nVerticalLineInitial);
  double verticalLineInitial = te::layout::Property::GetValueAs<double>(pVerticalLineInitial);

  //then we calculate the initial coordinates
  horizontalLineInitial = adjustHorizontalLineInitial(worldBox, horizontalLineInitial, horizontalLineGap);
  {
    Property property(0);
    property.setName(nHorizontalLineInitial);
    property.setValue(horizontalLineInitial, dataType->getDataTypeDouble());

    ItemUtils::addOrUpdateProperty(property, properties);
  }

  verticalLineInitial = adjustHorizontalLineInitial(worldBox, verticalLineInitial, verticalLineGap);
  {
    Property property(0);
    property.setName(nVerticalLineInitial);
    property.setValue(verticalLineInitial, dataType->getDataTypeDouble());

    ItemUtils::addOrUpdateProperty(property, properties);
  }
}

double te::layout::MapController::adjustHorizontalLineInitial(const te::gm::Envelope& worldBox, double initialX, double gapX)
{
  double lowerLeftX = worldBox.getLowerLeftX();
  double distance = initialX - lowerLeftX;
  double gapCount = floor(distance / gapX);
  double newInitialX = initialX - (gapCount * gapX);

  return newInitialX;
}

double te::layout::MapController::adjustVerticalLineInitial(const te::gm::Envelope& worldBox, double initialY, double gapY)
{
  double loweLeftY = worldBox.getLowerLeftY();
  double distance = initialY - loweLeftY;
  double gapCount = floor(distance / gapY);
  double newInitialY = initialY - (gapCount * gapY);

  return newInitialY;
}

bool te::layout::MapController::hasToRecalculatePlanarInitialLine(const Properties& properties)
{
  // check if initial line has to be recalculate (Ex.: Zoom Out)
  const Property& pWorldBox = getProperty("world_box", properties);
  const te::gm::Envelope& worldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pWorldBox);
  if (!worldBox.isValid())
  {
    return false;
  }

  PlanarGridSettingsConfigProperties planarGridSettings;

  if (properties.contains(planarGridSettings.getHorizontalLineInitial())
    || properties.contains(planarGridSettings.getVerticalLineInitial()))
  {
    return false;
  }

  const Property& pSrid = m_model->getProperty("srid");
  int srid = te::layout::Property::GetValueAs<int>(pSrid);

  const Property& pHorizontalLineGap = m_model->getProperty(planarGridSettings.getHorizontalLineGap());
  double horizontalLineGap = te::layout::Property::GetValueAs<double>(pHorizontalLineGap);

  const Property& pVerticalLineGap = m_model->getProperty(planarGridSettings.getVerticalLineGap());
  double verticalLineGap = te::layout::Property::GetValueAs<double>(pVerticalLineGap);

  const Property& pHorizontalLineInitial = m_model->getProperty(planarGridSettings.getHorizontalLineInitial());
  double horizontalLineInitial = te::layout::Property::GetValueAs<double>(pHorizontalLineInitial);

  const Property& pVerticalLineInitial = m_model->getProperty(planarGridSettings.getVerticalLineInitial());
  double verticalLineInitial = te::layout::Property::GetValueAs<double>(pVerticalLineInitial);

  const Property& pPlanarSRID = m_model->getProperty(planarGridSettings.getPlanarSRID());
  int planarSRID = te::layout::Property::GetValueAs<int>(pPlanarSRID);

  //if first prepare the world box, ensuring that it is a planar box. If it is not, we reproject it
  te::gm::Envelope planarBox = Utils::worldBoxTo(worldBox, srid, planarSRID);

  if (horizontalDistanceBiggerThanGap(planarBox, horizontalLineInitial, horizontalLineGap)
    || verticalDistanceBiggerThanGap(planarBox, verticalLineInitial, verticalLineGap))
  {
    return true;
  }

  return false;

}
bool te::layout::MapController::hasToRecalculateGeodesicInitialLine(const Properties& properties)
{
  // check if initial line has to be recalculate (Ex.: Zoom Out)
  const Property& pWorldBox = getProperty("world_box", properties);
  const te::gm::Envelope& worldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pWorldBox);
  if (!worldBox.isValid())
  {
    return false;
  }

  GeodesicGridSettingsConfigProperties geodesicGridSettings;

  if (properties.contains(geodesicGridSettings.getHorizontalLineInitial())
    || properties.contains(geodesicGridSettings.getVerticalLineInitial()))
  {
    return false;
  }

  const Property& pSrid = m_model->getProperty("srid");
  int srid = te::layout::Property::GetValueAs<int>(pSrid);

  const Property& pHorizontalLineGap = m_model->getProperty(geodesicGridSettings.getHorizontalLineGap());
  double horizontalLineGap = te::layout::Property::GetValueAs<double>(pHorizontalLineGap);

  const Property& pVerticalLineGap = m_model->getProperty(geodesicGridSettings.getVerticalLineGap());
  double verticalLineGap = te::layout::Property::GetValueAs<double>(pVerticalLineGap);

  const Property& pHorizontalLineInitial = m_model->getProperty(geodesicGridSettings.getHorizontalLineInitial());
  double horizontalLineInitial = te::layout::Property::GetValueAs<double>(pHorizontalLineInitial);

  const Property& pVerticalLineInitial = m_model->getProperty(geodesicGridSettings.getVerticalLineInitial());
  double verticalLineInitial = te::layout::Property::GetValueAs<double>(pVerticalLineInitial);

  const Property& pGeodesicSRID = m_model->getProperty(geodesicGridSettings.getGeodesicSRID());
  int geodesicSRID = te::layout::Property::GetValueAs<int>(pGeodesicSRID);

  te::gm::Envelope geographicBox = Utils::worldBoxTo(worldBox, srid, geodesicSRID);

  if (horizontalDistanceBiggerThanGap(geographicBox, horizontalLineInitial, horizontalLineGap)
    || verticalDistanceBiggerThanGap(geographicBox, verticalLineInitial, verticalLineGap))
  {
    return true;
  }

  return false;
}

bool te::layout::MapController::horizontalDistanceBiggerThanGap(const te::gm::Envelope& worldBox, double initialX, double gapX)
{
  bool result = false;

  double lowerLeftX = worldBox.getLowerLeftX();
  double distance = initialX - lowerLeftX;

  if (distance > gapX)
  {
    result = true;
  }

  return result;
}

bool te::layout::MapController::verticalDistanceBiggerThanGap(const te::gm::Envelope& worldBox, double initialY, double gapY)
{
  bool result = false;

  double loweLeftY = worldBox.getLowerLeftY();
  double distance = initialY - loweLeftY;

  if (distance > gapY)
  {
    result = true;
  }

  return result;
}

te::gm::Envelope te::layout::MapController::calculateOutsideBoundingBox(const QPointF& originGrid, const QPointF& finalGrid, const QSizeF& mapSize)
{
  double width = mapSize.width();
  double height = mapSize.height();

  if (finalGrid.x() > 0. || finalGrid.y() > 0.)
  {
    width += (originGrid.x() + originGrid.x());
    height += (originGrid.y() + originGrid.y());
  }

  te::gm::Envelope newBoundingBox(0, 0, width, height);
  return newBoundingBox;
}
