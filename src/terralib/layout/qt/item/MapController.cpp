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
#include "../../core/pattern/proxy/AbstractProxyProject.h"
#include "../core/Value.h"
#include "../core/Scene.h"
#include "../core/pattern/command/ChangePropertyCommand.h"

#include "terralib/maptools/Utils.h"

// STL
#include <set>
#include <vector>
#include <map>

// Qt
#include <QGraphicsItem>

te::layout::MapController::MapController(AbstractItemModel* model)
  : AbstractItemController(model)
{

}

te::layout::MapController::~MapController()
{

}

void te::layout::MapController::recomposeExtent()
{
  const Property& pLayers = this->getProperty("layers");
  const Property& pSrid = this->getProperty("srid");

  const std::list<te::map::AbstractLayerPtr>& currentLayerList = pLayers.getValue().toLayerList();
  int srid = pSrid.getValue().toInt();

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
  const te::gm::Envelope& worldBox = pWorldBox.getValue().toEnvelope();

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
  const te::gm::Envelope& worldBox = pWorldBox.getValue().toEnvelope();

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
  std::list<te::map::AbstractLayerPtr> currentLayersList = pCurrentLayers.getValue().toLayerList();
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

  changedPropertyLayerURIFromDropEvent(beforeProps); // add change property command to undo/redo 
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
  te::layout::Properties propertiesCopy = properties;

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
    vString = property.getValue().toStringVector();

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
  std::list<te::map::AbstractLayerPtr> layerList;
  std::vector<std::string>  vString;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if (property.getName().compare("layers") == 0)
  {
    layerList = property.getValue().toLayerList();

    AbstractProxyProject* project = getAbstractProxyProject();
    if (!project)
    {
      return prop;
    }

    // search layer info (uri)
    for (std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin(); it != layerList.end(); ++it)
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
  const Property& pUri = properties.getProperty("layers_uri");
  const Property& pLayers = properties.getProperty("layers");

  //we first check if only one of the properties related to the layers are set. If so, we sinchronize both values
  Property propertyToUpdate;
  if (pUri.isNull() == false && pLayers.isNull() == true)
  {
    propertyToUpdate = syncLayersFromURIs(pUri);
  }
  else if (pUri.isNull() == true && pLayers.isNull() == false)
  {
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
  const Property& pNewLayerList = properties.getProperty("layers");
  const std::list<te::map::AbstractLayerPtr>& currentLayerList = pCurrentLayerList.getValue().toLayerList();
  const std::list<te::map::AbstractLayerPtr>& newLayerList = pNewLayerList.getValue().toLayerList();

  //if the list of layers has not been changed, there is nothing to do
  if (pNewLayerList.isNull() == true)
  {
    return false;
  }

  //if there is an intersection, we do not try to initialize the "srid" and "world_box" properties
  std::list<te::map::AbstractLayerPtr> intersectionList = getIntersection(currentLayerList, newLayerList);
  if (intersectionList.empty() == false)
  {
    //there is nothing to update
    return false;
  }

  //if the list is entirely new, we must ensure that the "srid" and "world_box" properties will be initialized
  //if both values are set, we do nothing  
  const Property& pSrid = properties.getProperty("srid");
  const Property& pWorldBox = properties.getProperty("world_box");
  if (pSrid.isNull() == false && pWorldBox.isNull() == false)
  {
    //there is nothing to update
    return false;
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
      srid = currentSrid;
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


bool te::layout::MapController::syncMapSizeProperties(Properties& properties)
{
  //here we sync all properties related to the map
  Property pWorldBox = properties.getProperty("world_box");
  Property pSrid = properties.getProperty("srid");
  Property pWidth = properties.getProperty("width");
  Property pHeight = properties.getProperty("height");
  Property pScale = properties.getProperty("scale");
  Property pFixedScale = properties.getProperty("fixed_scale");

  //if there were no chances, we dont need to do nothing
  if (pWorldBox.isNull() && pWidth.isNull() && pHeight.isNull())
  {
    //there is nothing to update
    return false;
  }

  //if the size is not being set, we must consider the current size of the item
  if (pWidth.isNull() == true)
  {
    pWidth = this->getProperty("width");
  }
  if (pHeight.isNull() == true)
  {
    pHeight = this->getProperty("height");
  }

  //if the world_box is not being set, we must consider the current world_box of the item
  if (pWorldBox.isNull() == true)
  {
    pWorldBox = this->getProperty("world_box");
  }
  if (pSrid.isNull() == true)
  {
    pSrid = this->getProperty("srid");
  }
  if (pFixedScale.isNull() == true)
  {
    pFixedScale = this->getProperty("fixed_scale");
  }
  if (pScale.isNull() == true)
  {
    pScale = this->getProperty("scale");
  }

  //so we must ensure that the aspect of the world box is proportional to the width and height
  double widthMM = pWidth.getValue().toDouble();
  double heightMM = pHeight.getValue().toDouble();
  te::gm::Envelope worldBox = pWorldBox.getValue().toEnvelope();
  int srid = pSrid.getValue().toInt();
  bool fixedScale = pFixedScale.getValue().toBool();
  double scale = pScale.getValue().toDouble();

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
  pWorldBox.setValue(worldBox, dataType->getDataTypeEnvelope());

  ItemUtils::addOrUpdateProperty(pWorldBox, properties);

  return true;
}

bool te::layout::MapController::syncMapScaleProperties(Properties& properties)
{
  //here we sync all properties related to the map
  Property pWorldBox = properties.getProperty("world_box");
  Property pSrid = properties.getProperty("srid");
  Property pScale = properties.getProperty("scale");
  Property pWidth = properties.getProperty("width");
  Property pHeight = properties.getProperty("height");

  //if there were no chances, we dont need to do nothing
  if (pWorldBox.isNull() && pWidth.isNull() && pHeight.isNull() && pScale.isNull())
  {
    //there is nothing to update
    return false;
  }

  //if the size is not being set, we must consider the current size of the item
  if (pWidth.isNull() == true)
  {
    pWidth = this->getProperty("width");
  }
  if (pHeight.isNull() == true)
  {
    pHeight = this->getProperty("height");
  }

  //if the world_box is not being set, we must consider the current world_box of the item
  if (pWorldBox.isNull() == true)
  {
    pWorldBox = this->getProperty("world_box");
  }
  if (pSrid.isNull() == true)
  {
    pSrid = this->getProperty("srid");
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  double widthMM = pWidth.getValue().toDouble();
  double heightMM = pHeight.getValue().toDouble();
  te::gm::Envelope worldBox = pWorldBox.getValue().toEnvelope();
  int srid = pSrid.getValue().toInt();

  //if the scale is NOT being set, we must compute its value
  //(else) if the scale is being set, we must recompute the world_box
  if (pScale.isNull() == true)
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
    double scale = pScale.getValue().toDouble();

    //here we calculate the world_box based on the scale
    te::gm::Envelope newWorld = ItemUtils::calculateBoxFromScale(widthMM, heightMM, srid, worldBox, scale);

    Property pNewWorldBox;
    pNewWorldBox.setName("world_box");
    pNewWorldBox.setValue(newWorld, dataType->getDataTypeEnvelope());
    ItemUtils::addOrUpdateProperty(pNewWorldBox, properties);
  }

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

  Scene* scene = dynamic_cast<Scene*>(view->scene());
  if (!scene)
  {
    return project;
  }

  Value<AbstractProxyProject* >* value = scene->getContextValues<AbstractProxyProject *>("proxy_project");
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
  currentLayerList = pLayers.getValue().toLayerList();

  if (currentLayerList.empty())
    return;

  const Property& pSrid = this->getProperty("srid");
  int srid = pSrid.getValue().toInt();

  if (srid <= 0)
  {
    m_warningManager->addWarning(TR_LAYOUT("Map Item Without Projection"));
  }


  for (std::list<te::map::AbstractLayerPtr>::iterator it = currentLayerList.begin(); it != currentLayerList.end(); ++it)
  {

    te::map::AbstractLayerPtr layer = (*it);
    int layerSrid = layer->getSRID();

    if (layerSrid <= 0)
    {
      std::string warningMSG = TR_LAYOUT("Layer ") + layer->getTitle() + TR_LAYOUT(" Without Projection");
      m_warningManager->addWarning(warningMSG);
    }
  }
}

void te::layout::MapController::changedPropertyLayerURIFromDropEvent(const Properties& beforeProps)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  Property prop;
  prop = m_model->getProperty("layers_uri");

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

