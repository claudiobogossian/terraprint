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
  te::layout::Properties properties;
  properties.addProperty(property);

  setProperties(properties);
}

void te::layout::MapController::setProperties(const te::layout::Properties& properties)
{
  //if somehow the item is invalid, we do nothing
  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if (view == 0)
  {
    AbstractItemController::setProperties(properties);
    return;
  }

  te::layout::Properties propertiesCopy = properties;

  //if the property 'layers_uri' is set, we must try to initialize the 'layers' property based on it
  const Property& pUri = propertiesCopy.getProperty("layers_uri");
  const Property& pLayers = propertiesCopy.getProperty("layers");
  if (pUri.isNull() == false || pLayers.isNull() == false)
  {
    Property propertyToUpdate = syncLayersAndURIs(pUri);
    if (propertyToUpdate.isNull() == true)
    {
      propertyToUpdate = syncLayersAndURIs(pLayers);
    }
    if (propertyToUpdate.isNull() == false)
    {
      if (propertiesCopy.contains(propertyToUpdate.getName()) == true)
      {
        propertiesCopy.updateProperty(propertyToUpdate);
      }
      else
      {
        propertiesCopy.addProperty(propertyToUpdate);
      }
    }
  }

  //now we apply all the map related properties to the mapDisplay inside the view
  bool wasSync = syncMapDisplayProperties(propertiesCopy.getProperties());  
  if (wasSync == true)
  {
    te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();

    Properties extentChangedProperties = getExtentChangedProperties(mapDisplay->getExtent(), mapDisplay->getScale(), mapDisplay->getSRID());
    for (size_t j = 0; j < extentChangedProperties.getProperties().size(); ++j)
    {
      const Property& pChanged = extentChangedProperties.getProperties()[j];

      if (propertiesCopy.contains(pChanged.getName()) == true)
      {
        propertiesCopy.updateProperty(pChanged);
      }
      else
      {
        propertiesCopy.addProperty(pChanged);
      }
    }
  }

  //we finally set the properties into the model
  AbstractItemController::setProperties(propertiesCopy);
  if (wasSync == true)
  {
    //view->doRefresh();
  }


  /*
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
  }*/
}

bool te::layout::MapController::isMapDisplayProperty(const te::layout::Property& property) const
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

bool te::layout::MapController::syncMapDisplayProperties(const std::vector<te::layout::Property>& vecProperties)
{
  std::map<std::string, Property> mapProperties;
  std::map<std::string, Property>::iterator itProperties;

  for (size_t i = 0; i < vecProperties.size(); ++i)
  {
    const Property& property = vecProperties[i];
    mapProperties[property.getName()] = property;
  }

  std::list<te::map::AbstractLayerPtr> layerList;
  int srid = -1;
  te::gm::Envelope envelope;
  double scale = 0.;
  double width = 0.;
  double height = 0.;

  //we get the layer list
  itProperties = mapProperties.find("layers");
  if (itProperties != mapProperties.end())
  {
    layerList = itProperties->second.getValue().toLayerList();
  }

  //we get the srid
  itProperties = mapProperties.find("srid");
  if (itProperties != mapProperties.end())
  {
    srid = itProperties->second.getValue().toInt();
  }

  //we get the envelope
  itProperties = mapProperties.find("world_box");
  if (itProperties != mapProperties.end())
  {
    envelope = itProperties->second.getValue().toEnvelope();
  }

  //we get the scale
  itProperties = mapProperties.find("scale");
  if (itProperties != mapProperties.end())
  {
    scale = itProperties->second.getValue().toDouble();
  }

  //we get the width
  itProperties = mapProperties.find("width");
  if (itProperties != mapProperties.end())
  {
    width = itProperties->second.getValue().toDouble();
  }

  //we get the height
  itProperties = mapProperties.find("height");
  if (itProperties != mapProperties.end())
  {
    height = itProperties->second.getValue().toDouble();
  }

  //then we try to compute any uninitialized properties based on the layer list.
  //if the layerList is empty, we are not able to initialize any other property
  if (layerList.empty() == true)
  {
    return false;
  }

  if (layerList.empty() == false)
  {
    if (srid == -1)
    {
      srid = layerList.begin()->get()->getSRID();
    }
    if (envelope.isValid() == false)
    {
      const Property& property = this->getProperty("layers");
      const std::list<te::map::AbstractLayerPtr>& currentLayerList = property.getValue().toLayerList();

      if (currentLayerList != layerList)
      {
        //checks the intersection between the layers list in the model and the new layer list to be set
        std::list<te::map::AbstractLayerPtr> intersectionList = getIntersection(currentLayerList, layerList);

        //If the old layer list has no intersection to the new, we need to also define the srid e the envelope
        if (intersectionList.empty() == true)
        {
          std::list<te::map::AbstractLayerPtr>::const_iterator it = layerList.begin();
          while (it != layerList.end())
          {
            te::map::AbstractLayerPtr layer = (*it);

            te::gm::Envelope currentEnvelope = layer->getExtent();

            int currentSrid = layer->getSRID();
            if (srid <= 0)
              srid = currentSrid;
            if (currentSrid != srid)
              currentEnvelope.transform(currentSrid, srid);

            envelope.Union(currentEnvelope);

            ++it;
          }
        }
      }
    }
  }

  MapItem* view = dynamic_cast<MapItem*>(m_view);
  if (view == 0)
  {
    return false;
  }

  int widthPixels = 0;
  int heightPixels = 0;

  bool sizeChanged = false;
  if (width != 0 && height != 0 && view->scene() != 0)
  {
    Scene* myScene = dynamic_cast<Scene*>(view->scene());
    if (myScene != 0)
    {
      Utils utils = myScene->getUtils();

      QRectF boxMM = view->boundingRect();

      te::gm::Envelope box(0, 0, boxMM.width(), boxMM.height());
      box = utils.viewportBox(box);

      widthPixels = qRound(box.getWidth());
      heightPixels = qRound(box.getHeight());
    }
  }

  te::qt::widgets::MapDisplay* mapDisplay = view->getMapDisplay();

  //this refresh need to be done in order to correctly initialize the mapDisplay. We should review this later
  m_ignoreExtentChangedEvent = true;
  //mapDisplay->changeData(te::map::AbstractLayerPtr());

  if (widthPixels != 0 && heightPixels != 0)
  {
    QSize currentSize = mapDisplay->size();
    QSize newSize(widthPixels, heightPixels);
    
    if (currentSize != newSize)
    {
      mapDisplay->resize(newSize);
      sizeChanged = true;
    }
  }

  mapDisplay->setLayerList(layerList);
  if (srid > 0)
  {
    mapDisplay->setSRID(srid, false);
  }
  
  if (envelope.isValid() == true)
  {
    mapDisplay->setExtent(envelope, false);
  }
  
  if (scale > 0.)
  {
    scale = scale / (m_zoom / 100.); //adjusting scale to the zoom factor
    mapDisplay->setScale(scale);    
  }

  double currentScale = mapDisplay->getScale() / (m_zoom / 100.);

  if (sizeChanged == false)
  {
    view->doRefresh();
  }

  m_ignoreExtentChangedEvent = false;

  return true;
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
      mapDisplay->changeData(te::map::AbstractLayerPtr());
      mapDisplay->setLayerList(layerList);
      mapDisplay->setSRID(srid, false);
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

