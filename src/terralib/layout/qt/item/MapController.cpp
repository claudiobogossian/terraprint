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
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "terralib/qt/widgets/canvas/MapDisplay.h"
#include "terralib/maptools/MapDisplay.h"
#include "terralib/dataaccess/datasource/DataSourceInfoManager.h"
#include "../core/Value.h"
#include "../core/Scene.h"
#include "../../core/pattern/proxy/AbstractProxyProject.h"

// STL
#include <set>
#include <vector>
#include <map>

te::layout::MapController::MapController(AbstractItemModel* model)
  : AbstractItemController(model)
  , m_zoom(0)
  , m_ignoreExtentChangedEvent(false)
{

}

te::layout::MapController::~MapController()
{

}

void te::layout::MapController::addLayers(const std::list<te::map::AbstractLayerPtr>& layerList)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  Property property;
  property.setName("layers");
  property.setValue(layerList, dataType->getDataTypeLayerList());

  setProperty(property);
}

void te::layout::MapController::extentChanged(const te::gm::Envelope& envelope, double scale, int srid)
{
  if(m_ignoreExtentChangedEvent == true)
  {
    return;
  }

  Properties properties = getExtentChangedProperties(envelope, scale, srid);
  if(properties.getProperties().empty() == false)
  {
    AbstractItemController::setProperties(properties);
  }
}

void te::layout::MapController::setZoom( const int& zoom )
{
  m_zoom = zoom;
}

void te::layout::MapController::setProperty(const te::layout::Property& property)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    AbstractItemController::setProperty(property);
    return;
  }

  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();
  bool wasSync = false;

  if(isMapDisplayProperty(property) == true)
  {
    if(syncMapDisplayProperty(property) == true)
    {
      wasSync = true;
    }
  }

  Property prop;
  // Sync Layers and URIs
  if (property.getName().compare("layers_uri") == 0 || property.getName().compare("layers") == 0)
  {
    prop = syncLayersAndURIs(property);
    syncMapDisplayProperty(prop);
  }

  if(wasSync == true)
  {
    Properties extentChangedProperties = getExtentChangedProperties(mapDisplay->getExtent(), mapDisplay->getScale(), mapDisplay->getSRID());
    if (property.getName() == "layers")
    {
      extentChangedProperties.addProperty(property);
    }

    if (!prop.isNull())
    {
      extentChangedProperties.addProperty(prop);
    }

    AbstractItemController::setProperties(extentChangedProperties);

    view->doRefresh();
  }
  else
  {
    if (prop.isNull())
    {
      AbstractItemController::setProperty(property);
    }
    else
    {
      Properties changedProperties;
      changedProperties.addProperty(property);
      changedProperties.addProperty(prop);
      AbstractItemController::setProperties(changedProperties);
    }
  }
}

void te::layout::MapController::setProperties(const te::layout::Properties& properties)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    AbstractItemController::setProperties(properties);
    return;
  }

  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();
  bool wasSync = false;

  Properties propertiesCopy;
  const std::vector<Property>& vecProperties = properties.getProperties();
  for(size_t i = 0; i < vecProperties.size(); ++i)
  {
    const Property& property = vecProperties[i];
    if(isMapDisplayProperty(property) == true)
    {
      if(syncMapDisplayProperty(property) == true)
      {
        wasSync = true;

        if (property.getName() == "layers")
        {
          propertiesCopy.addProperty(property);
        }
      }
      else
      {
        propertiesCopy.addProperty(property);
      }
    }
    else
    {
      // Sync Layers and URIs
      if (property.getName().compare("layers_uri") == 0
        || property.getName().compare("layers") == 0)
      {
        Property prop = syncLayersAndURIs(property);
        if (!prop.isNull())
        {
          propertiesCopy.addProperty(prop);
          syncMapDisplayProperty(prop);
        }
      }
      propertiesCopy.addProperty(property);
    }
  }

  if(wasSync == true)
  {
    Properties extentChangedProperties = getExtentChangedProperties(mapDisplay->getExtent(), mapDisplay->getScale(), mapDisplay->getSRID());
    for(size_t j = 0; j < extentChangedProperties.getProperties().size(); ++j)
    {
      propertiesCopy.addProperty(extentChangedProperties.getProperties()[j]);
    }
  }

  AbstractItemController::setProperties(propertiesCopy);

  if(wasSync == true)
  {
    view->doRefresh();
  }
}

bool te::layout::MapController::isMapDisplayProperty(const te::layout::Property& property)
{
  std::set<std::string> setMapDisplayProperty;
  setMapDisplayProperty.insert("world_box");
  setMapDisplayProperty.insert("scale");
  setMapDisplayProperty.insert("srid");
  setMapDisplayProperty.insert("layers");

  if(setMapDisplayProperty.find(property.getName()) != setMapDisplayProperty.end())
  {
    return true;
  }

  return false;
}

bool te::layout::MapController::syncMapDisplayProperty(const te::layout::Property& property)
{
  const Property& prop_layers = this->getProperty("layers");
  const std::list<te::map::AbstractLayerPtr>& currentLayerList = prop_layers.getValue().toLayerList();
  
  if (property.getName() == "layers")
  {
    const std::list<te::map::AbstractLayerPtr>& newLayerList = property.getValue().toLayerList();

    if (syncLayersToItem(newLayerList) == true)
    {
      return true;
    }
  }
  else if (property.getName() == "scale" && !currentLayerList.empty())
  {
    double newScale = property.getValue().toDouble();
    if(syncScaleToItem(newScale) == true)
    {
      return true;
    }
  }
  else if (property.getName() == "world_box" && !currentLayerList.empty())
  {
    te::gm::Envelope newExtent = property.getValue().toEnvelope();
    if(syncExtentToItem(newExtent) == true)
    {
      return true;
    }
  }
  else if (property.getName() == "srid" && !currentLayerList.empty())
  {
    int newSRID = property.getValue().toInt();
    if(syncSridToItem(newSRID) == true)
    {
      return true;
    }
  }

  return false;
}

te::layout::Properties te::layout::MapController::getExtentChangedProperties(const te::gm::Envelope& envelope, double scale, int srid)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pWorldBox = getProperty("world_box");
  const Property& pScale = getProperty("scale");
  const Property& pSrid = getProperty("srid");

  te::gm::Envelope currentEnvelope = pWorldBox.getValue().toEnvelope();
  double currentScale = pScale.getValue().toDouble();
  int currentSrid =  pSrid.getValue().toInt();

  Properties properties("");
  if(envelope.equals(currentEnvelope) == false)
  {
    Property property;
    property.setName("world_box");
    property.setValue(envelope, dataType->getDataTypeEnvelope());
    properties.addProperty(property);
  }

  if(scale != currentScale)
  {
    Property property;
    property.setName("scale");
    scale = scale * (m_zoom / 100.);
    property.setValue(scale, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  if(srid != currentSrid)
  {
    Property property;
    property.setName("srid");
    property.setValue(srid, dataType->getDataTypeInt());
    properties.addProperty(property);
  }

  return properties;
}

bool te::layout::MapController::syncScaleToItem(double scale)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    return false;
  }
  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();

  scale = scale / (m_zoom / 100.); //adjusting scale to the zoom factor

  double currentScale = mapDisplay->getScale();
  if(currentScale != scale)
  {
    m_ignoreExtentChangedEvent = true;
    mapDisplay->setScale(scale);
    m_ignoreExtentChangedEvent = false;

    return true;
  }

  return false;
}

bool te::layout::MapController::syncExtentToItem(const te::gm::Envelope& extent)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    return false;
  }

  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();

  const te::gm::Envelope& currentExtent = mapDisplay->getExtent();

  if(extent.equals(currentExtent) == false)
  {
    te::gm::Envelope extentCopy = extent;

    m_ignoreExtentChangedEvent = true;
    mapDisplay->setExtent(extentCopy, false);
    m_ignoreExtentChangedEvent = false;

    return true;
  }

  return false;
}

bool te::layout::MapController::syncSridToItem(int srid)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    return false;
  }

  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();

  int currentSrid = mapDisplay->getSRID();

  if(srid != currentSrid)
  {
    m_ignoreExtentChangedEvent = true;
    mapDisplay->setSRID(srid, false);
    m_ignoreExtentChangedEvent = false;

    return true;
  }

  return false;
}

bool te::layout::MapController::syncLayersToItem(const std::list<te::map::AbstractLayerPtr>& layerList)
{
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if(view == 0)
  {
    return false;
  }

  //we read the current layer list from the model because mapDisplay does not have the getLayers function
  const Property& property = this->getProperty("layers");
  const std::list<te::map::AbstractLayerPtr>& currentLayerList = property.getValue().toLayerList();

  if(currentLayerList != layerList)
  {
    te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();

   //checks the intersection between the layers list in the model and the new layer list to be set
    std::list<te::map::AbstractLayerPtr> intersectionList;

    std::list<te::map::AbstractLayerPtr>::const_iterator itCurrent = currentLayerList.begin();
    while (itCurrent != currentLayerList.end())
    {
      std::list<te::map::AbstractLayerPtr>::const_iterator itNew = layerList.begin();
      while (itNew != layerList.end())
      {
        if (itCurrent->get()->getId() == itNew->get()->getId())
        {
          intersectionList.push_back(itCurrent->get());
          break;
        }
        ++itNew;
      }
      ++itCurrent;
    }

    //If the old layer list has no intersection to the new, we need to also define the srid e the envelope
    if(intersectionList.empty() == true)
    {
      te::gm::Envelope envelope;
      int srid = -1;

      std::list<te::map::AbstractLayerPtr>::const_iterator it = layerList.begin();
      while(it != layerList.end())
      {
        te::map::AbstractLayerPtr layer = (*it);

        te::gm::Envelope currentEnvelope = layer->getExtent();

        int currentSrid = layer->getSRID();
        if(srid <= 0)
          srid = currentSrid;
        if(currentSrid != srid)
          currentEnvelope.transform(currentSrid, srid);

        envelope.Union(currentEnvelope);

        ++it;
      }

      //this refresh need to be done in order to correctly initialize the mapDisplay. We should review this later
      m_ignoreExtentChangedEvent = true;
      mapDisplay->setLayerList(layerList);
      mapDisplay->setSRID(srid, false);
      mapDisplay->refresh();
      mapDisplay->setExtent(envelope, false);
      m_ignoreExtentChangedEvent = false;
    }
    else
    {
      //If there is an intersection, we just update the list and keep all other properties enchanged
      m_ignoreExtentChangedEvent = true;
      mapDisplay->setLayerList(layerList);
      m_ignoreExtentChangedEvent = false;
    }

    return true;
  }

  return false;
}

te::layout::Property te::layout::MapController::syncLayersAndURIs(const Property& property)
{
  Property prop;
  prop = syncURIsFromLayers(property);
  if (prop.isNull())
  {
    prop = syncLayersFromURIs(property);
  }
  return prop;
}

te::layout::Property te::layout::MapController::syncLayersFromURIs(const Property& property)
{
  Property prop;
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if (view == 0)
  {
    return prop;
  }

  Scene* scene = dynamic_cast<Scene*>(view->scene());
  if (!scene)
  {
    return prop;
  }

  std::list<te::map::AbstractLayerPtr> layerList;
  std::vector<std::string>  vString;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if (property.getName().compare("layers_uri") == 0)
  {
    vString = property.getValue().toStringVector();

    AbstractProxyProject* project;
    std::list<te::map::AbstractLayerPtr> allLayerList;
    Value<AbstractProxyProject* >* value = scene->getContextValues<AbstractProxyProject *>("proxy_project");
    if (value)
    {
      project = value->get();
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

  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if (view == 0)
  {
    return prop;
  }

  std::list<te::map::AbstractLayerPtr> layerList;
  std::vector<std::string>  vString;
  std::string uriInfo = "URI";

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if (property.getName().compare("layers") == 0)
  {
    layerList = property.getValue().toLayerList();

    // search layer info (uri)
    for (std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin(); it != layerList.end(); ++it)
    {
      const std::string& id = it->get()->getDataSourceId();

      te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(id);
      const std::map<std::string, std::string>& connInfo = info->getConnInfo();

      std::string uri = "";

      for (std::map<std::string, std::string>::const_iterator it = connInfo.begin(); it != connInfo.end(); ++it)
      {
        std::string nameURI = it->first;
        if (nameURI.compare(uriInfo) == 0)
        {
          uri = it->second;
        }
        vString.push_back(uri);
      }
    }
    prop = m_model->getProperty("layers_uri");
    prop.setValue(vString, dataType->getDataTypeStringVector());
  }
  return prop;
}

