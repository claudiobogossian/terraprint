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
#include "MapCompositionController.h"
#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/property/SharedProperties.h"
#include "../../core/property/GridSettingsConfigProperties.h"
#include "MapCompositionItem.h"
#include "MapItem.h"



#include <set>

te::layout::MapCompositionController::MapCompositionController(AbstractItemModel* model, AbstractItemView* view)
  : ItemGroupController(model, view)
  , m_ignoreResize(false)
{

}

te::layout::MapCompositionController::~MapCompositionController()
{

}

const te::layout::Properties& te::layout::MapCompositionController::getProperties() const
{
  m_propertiesFullReference.clear();

  MapCompositionItem* mapCompositionItem = dynamic_cast<MapCompositionItem*>(m_view);
  if (mapCompositionItem == 0)
  {
    return ItemGroupController::getProperties();
  }

  //here, we decide which properties from the map and grid items should be hidden
  //what we do here is create (for each item) one new property that will contain all the properties that we want to hide
  //this is done because the properties returned MUST contain all the properties of the items in order to correctly show it in the dialogs or export to a saved map
  //in the other hand, when we receive a setProperties function call, we must "open" the new properties before passing them to the map and grid items

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  AbstractItemView* planarGridItem = mapCompositionItem->getPlanarGridItem();
  if (planarGridItem != 0)
  {
    const Properties& gridProperties = planarGridItem->getController()->getProperties();
    const std::vector<Property>& vecGridProperties = gridProperties.getProperties();

    Property pGridSettings = planarGridItem->getController()->getProperty("GridSettings");

    hideProperties(pGridSettings);

    pGridSettings.setLabel(TR_LAYOUT("Planar Grid Settings"));
    m_propertiesFullReference = te::layout::Properties(ItemGroupController::getProperties());    

    Property pGridProperties;
    pGridProperties.setName("PlanarGridProperties");
    pGridProperties.setValue(std::string(""), dataType->getDataTypeString());
    pGridProperties.setParent(pGridSettings.getParent());
    pGridProperties.setVisible(false);

    for (std::size_t i = 0; i < vecGridProperties.size(); ++i)
    {
      if (vecGridProperties[i].getName() == "GridSettings")
      {
        continue;
      }

      pGridProperties.addSubProperty(vecGridProperties[i]);
    }

    m_propertiesFullReference.addProperty(pGridSettings);
    m_propertiesFullReference.addProperty(pGridProperties);
  }

  AbstractItemView* geodesicGridItem = mapCompositionItem->getGeodesicGridItem();
  if (geodesicGridItem != 0)
  {
    const Properties& gridProperties = geodesicGridItem->getController()->getProperties();
    const std::vector<Property>& vecGridProperties = gridProperties.getProperties();

    Property pGridSettings = geodesicGridItem->getController()->getProperty("GridSettings");

    hideProperties(pGridSettings);

    pGridSettings.setLabel(TR_LAYOUT("Geodesic Grid Settings"));


    Property pGridProperties;
    pGridProperties.setName("GeodesicGridProperties");
    pGridProperties.setValue(std::string(""), dataType->getDataTypeString());
    pGridProperties.setParent(pGridSettings.getParent());
    pGridProperties.setVisible(false);

    for (std::size_t i = 0; i < vecGridProperties.size(); ++i)
    {
      if (vecGridProperties[i].getName() == "GridSettings")
      {
        continue;
      }

      pGridProperties.addSubProperty(vecGridProperties[i]);
    }

    m_propertiesFullReference.addProperty(pGridSettings);
    m_propertiesFullReference.addProperty(pGridProperties);
  }

  AbstractItemView* mapItem = mapCompositionItem->getMapItem();
  if (mapItem != 0)
  {
    const Properties& mapProperties = mapItem->getController()->getProperties();
    const std::vector<Property>& vecMapProperties = mapProperties.getProperties();

    const Property& pMapSettings = mapProperties.getProperty("mapSettings");

    Property pMapProperties;
    pMapProperties.setName("MapProperties");
    pMapProperties.setValue(std::string(""), dataType->getDataTypeString());
    pMapProperties.setParent(pMapSettings.getParent());
    pMapProperties.setVisible(false);
    
    for (std::size_t i = 0; i < vecMapProperties.size(); ++i)
    {
      if (vecMapProperties[i].getName() == "mapSettings")
      {
        continue;
      }

      pMapProperties.addSubProperty(vecMapProperties[i]);
    }

    m_propertiesFullReference.addProperty(pMapSettings);
    m_propertiesFullReference.addProperty(pMapProperties);
  }

  return m_propertiesFullReference;
}

void te::layout::MapCompositionController::setProperties(const te::layout::Properties& properties)
{
  //here we must "open" the new properties before passing them to the map and grid items
  Properties allProperties = expandHiddenProperties(properties);
  std::map<std::string, te::layout::Properties> mapProperties = groupPropertiesByParent(allProperties);

  syncItemSize(mapProperties);

  const std::string& parentClass = this->getModel()->getType()->getName();

  std::vector<AbstractItemView*> vecAvailableViews;
  MapCompositionItem* mapCompositionItem = dynamic_cast<MapCompositionItem*>(m_view);
  if (mapCompositionItem != 0)
  {
    AbstractItemView* planarGridItem = mapCompositionItem->getPlanarGridItem();
    if (planarGridItem != 0)
    {
      vecAvailableViews.push_back(planarGridItem);
    }

    AbstractItemView* geodesicGridItem = mapCompositionItem->getGeodesicGridItem();
    if (geodesicGridItem != 0)
    {
      vecAvailableViews.push_back(geodesicGridItem);
    }

    AbstractItemView* mapItem = mapCompositionItem->getMapItem();
    if (mapItem != 0)
    {
      vecAvailableViews.push_back(mapItem);
    }
  }

  for (size_t i = 0; i < vecAvailableViews.size(); ++i)
  {
    AbstractItemView* currentView = vecAvailableViews[i];
    const std::string& parentClass = currentView->getController()->getProperties().getTypeObj()->getName();

    std::map<std::string, te::layout::Properties>::iterator it = mapProperties.find(parentClass);

    if (it != mapProperties.end())
    {
      currentView->getController()->setProperties(it->second);
    }
  }

  std::map<std::string, te::layout::Properties>::iterator it = mapProperties.find(parentClass);

  if (it != mapProperties.end())
  {
    te::layout::Properties& mapCompositionProperties = it->second;
    ItemGroupController::setProperties(mapCompositionProperties);
  }
}

void te::layout::MapCompositionController::update(const Subject* subject)
{
  ItemGroupController::update(subject);

  if (subject != m_model)
  {
    m_view->prepareGeometryChange();
  }
}

std::map<std::string, te::layout::Properties> te::layout::MapCompositionController::groupPropertiesByParent(const te::layout::Properties& properties)
{
  std::map<std::string, te::layout::Properties> mapProperties;

  const std::vector<Property>& vecProperty = properties.getProperties();
  for (size_t i = 0; i < vecProperty.size(); ++i)
  {
    const Property& property = vecProperty[i];
    const std::string& parentClass = property.getParent();

    std::map<std::string, te::layout::Properties>::iterator it = mapProperties.find(parentClass);
    if (it != mapProperties.end())
    {
      it->second.addProperty(property);
    }
    else
    {
      te::layout::Properties properties;
      properties.addProperty(property);

      mapProperties[parentClass] = properties;
    }
  }

  return mapProperties;
}

te::layout::Properties te::layout::MapCompositionController::expandHiddenProperties(const Properties& properties)
{
  const std::vector<Property>& vecProperties = properties.getProperties();

  te::layout::Properties expandedProperties;
  
  for (std::size_t i = 0; i < vecProperties.size(); ++i)
  {
    const Property& property = vecProperties[i];
    if(property.getName() == "PlanarGridProperties")
    { 
      const std::vector<Property>& vecSubProperties = property.getSubProperty();
      for (std::size_t j = 0; j < vecSubProperties.size(); ++j)
      {
        expandedProperties.addProperty(vecSubProperties[j]);
      }
    }
    else if (property.getName() == "GeodesicGridProperties")
    {
      const std::vector<Property>& vecSubProperties = property.getSubProperty();
      for (std::size_t j = 0; j < vecSubProperties.size(); ++j)
      {
        expandedProperties.addProperty(vecSubProperties[j]);
      }
    }
    else if (property.getName() == "MapProperties")
    {
      const std::vector<Property>& vecSubProperties = property.getSubProperty();
      for (std::size_t j = 0; j < vecSubProperties.size(); ++j)
      {
        expandedProperties.addProperty(vecSubProperties[j]);
      }
    }
    else
    {
      expandedProperties.addProperty(property);
    }
  }

  return expandedProperties;
}

void te::layout::MapCompositionController::hideProperties(Property& property) const
{
  GridSettingsConfigProperties settingsConfig;

  std::set<std::string> setVisible;
  setVisible.insert("GridSettings");
  setVisible.insert(settingsConfig.getInitialGridPointX());
  setVisible.insert(settingsConfig.getInitialGridPointY());
  setVisible.insert(settingsConfig.getLneHrzGap());
  setVisible.insert(settingsConfig.getLneVrtGap());
  setVisible.insert(settingsConfig.getStyle());
  setVisible.insert(settingsConfig.getLineStyle());
  setVisible.insert(settingsConfig.getVisible());

  if (setVisible.find(property.getName()) == setVisible.end())
  {
    property.setVisible(false);
  }

  const std::vector<Property>& vecSubProperties = property.getSubProperty();
  for (size_t i = 0; i < vecSubProperties.size(); ++i)
  {
    Property subProperty(vecSubProperties[i]); //copies the property
    hideProperties(subProperty);
    property.completelyUpdateSubProperty(subProperty);
  }
}


void te::layout::MapCompositionController::syncItemSize(std::map<std::string, te::layout::Properties>& mapProperties)
{
  if (m_ignoreResize)
  {
    return;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  MapCompositionItem* mapCompositionItem = dynamic_cast<MapCompositionItem*>(m_view);
  AbstractItemView* mapItem = mapCompositionItem->getMapItem();
  if (mapItem == 0)
  {
    return;
  }

  std::string mapItemClassName = mapItem->getController()->getProperties().getTypeObj()->getName();


  const std::string& parentClass = this->getModel()->getType()->getName();
  
  std::map<std::string, te::layout::Properties>::iterator itMapComposition = mapProperties.find(parentClass);
  std::map<std::string, te::layout::Properties>::iterator itMap = mapProperties.find(mapItemClassName);

  //if the size of the map composition item is being defined and the size of the map is not, we must resize the internal items
  if (itMapComposition == mapProperties.end())
  {
    return;
  }

  const te::layout::Properties& mapCompositionItemProperties = itMapComposition->second;
  te::layout::Properties mapItemProperties;
  if (itMap != mapProperties.end())
  {
    mapItemProperties = itMap->second;

    if (mapItemProperties.contains("width") || mapItemProperties.contains("height"))
    {
      return;
    }
  }

  if (mapCompositionItemProperties.contains("width"))
  {
    double newMapCompositionWidth = Property::GetValueAs<double>(mapCompositionItemProperties.getProperty("width"));
    double currentMapCompositionWidth = Property::GetValueAs<double>(getProperty("width"));
    double currentMapWidth = Property::GetValueAs<double>(mapItem->getController()->getProperty("width"));
    double diffWidth = currentMapCompositionWidth - currentMapWidth;
    double newMapWidth = newMapCompositionWidth - diffWidth;

    Property pNewWidth;
    pNewWidth.setName("width");
    pNewWidth.setValue(newMapWidth, dataType->getDataTypeDouble());
    pNewWidth.setParent(mapItemClassName);

    mapItemProperties.addProperty(pNewWidth);
  }

  if (mapCompositionItemProperties.contains("height"))
  {
    double newMapCompositionHeight = Property::GetValueAs<double>(mapCompositionItemProperties.getProperty("height"));
    double currentMapCompositionHeight = Property::GetValueAs<double>(getProperty("height"));
    double currentMapHeight = Property::GetValueAs<double>(mapItem->getController()->getProperty("height"));
    double diffHeight = currentMapCompositionHeight - currentMapHeight;
    double newMapHeight = newMapCompositionHeight - diffHeight;

    Property pNewHeight;
    pNewHeight.setName("height");
    pNewHeight.setValue(newMapHeight, dataType->getDataTypeDouble());
    pNewHeight.setParent(mapItemClassName);

    mapItemProperties.addProperty(pNewHeight);
  }

  if (mapItemProperties.getProperties().empty() == false)
  {
    mapProperties[mapItemClassName] = mapItemProperties;
  }
}

void te::layout::MapCompositionController::updateBoundingRect(QRectF rect)
{
  m_ignoreResize = true;
  ItemGroupController::updateBoundingRect(rect);
  m_ignoreResize = false;
}
