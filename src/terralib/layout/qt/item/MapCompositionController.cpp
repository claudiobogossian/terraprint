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
#include "MapCompositionItem.h"

#include "../../core/property/GridSettingsConfigProperties.h"

#include <set>

te::layout::MapCompositionController::MapCompositionController(AbstractItemModel* model)
  : ItemGroupController(model)
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

  AbstractItemView* planarGridItem = mapCompositionItem->getPlanarGridItem();
  if (planarGridItem != 0)
  {
    Property pGridSettings = planarGridItem->getController()->getProperty("GridSettings");
    if (pGridSettings.isNull() == true)
    {
      return ItemGroupController::getProperties();
    }

    hideProperties(pGridSettings);

    pGridSettings.setLabel(TR_LAYOUT("Planar Grid Settings"));
    m_propertiesFullReference = te::layout::Properties(ItemGroupController::getProperties());    
    m_propertiesFullReference.addProperty(pGridSettings);
  }

  AbstractItemView* geodesicGridItem = mapCompositionItem->getGeodesicGridItem();
  if (planarGridItem != 0)
  {
    Property pGridSettings = geodesicGridItem->getController()->getProperty("GridSettings");
    if (pGridSettings.isNull() == true)
    {
      return ItemGroupController::getProperties();
    }

    hideProperties(pGridSettings);

    pGridSettings.setLabel(TR_LAYOUT("Geodesic Grid Settings"));
    m_propertiesFullReference.addProperty(pGridSettings);
  }

  return m_propertiesFullReference;
}

void te::layout::MapCompositionController::setProperties(const te::layout::Properties& properties)
{
  std::map<std::string, te::layout::Properties> mapProperties = groupPropertiesByParent(properties);

  const std::string& parentClass = this->getProperties().getTypeObj()->getName();

  std::map<std::string, te::layout::Properties>::iterator it = mapProperties.find(parentClass);

  if (it != mapProperties.end())
  {
    ItemGroupController::setProperties(it->second);
    return;
  }

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

  std::vector<Property> vecSubProperties = property.getSubProperty();
  for (size_t i = 0; i < vecSubProperties.size(); ++i)
  {
    Property subProperty = vecSubProperties[i];
    hideProperties(subProperty);
    property.completelyUpdateSubProperty(subProperty);
  }
}
