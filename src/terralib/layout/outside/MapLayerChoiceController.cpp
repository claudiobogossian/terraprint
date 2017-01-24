/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file PageSetupController.h
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapLayerChoiceController.h"

#include "../core/pattern/mvc/AbstractItemController.h"
#include "../qt/item/MapItem.h"
#include "../../layout/outside/MapLayerChoiceModel.h"

te::layout::MapLayerChoiceController::MapLayerChoiceController(Scene * scene, AbstractProxyProject * proxy, AbstractOutsideModel* o) :
  AbstractOutsideController(o),
  m_scene(scene),
  m_proxy(proxy)
{
  
}

te::layout::MapLayerChoiceController::~MapLayerChoiceController()
{

}

te::layout::Property te::layout::MapLayerChoiceController::getProperty(std::string name)
{
  Property prop;

  ItemUtils utils(m_scene);
  std::vector<MapItem*> vecMap = utils.getMapItemList();

  if (vecMap.empty() == true)
    return prop;

  //seraches for the selected MapItem
  for (size_t i = 0; i < vecMap.size(); ++i)
  {
    MapItem* mapItem = vecMap[i];
    if (mapItem->isSelected() == true)
    {
      prop = mapItem->getProperty(name);
      break;
    }
    else if(mapItem->parentItem() != 0 && mapItem->parentItem()->isSelected())
    {
      prop = mapItem->getProperty(name);
      break;
    }
  }

  return prop;
}

std::list<te::map::AbstractLayerPtr> te::layout::MapLayerChoiceController::getlistLayers(){
  
  return m_proxy->getAllLayers();
}

std::list<te::map::AbstractLayerPtr> te::layout::MapLayerChoiceController::searchLayers()
{
  std::list<te::map::AbstractLayerPtr> layers;

  std::string nameProp = "layers";
  Property prop = getProperty(nameProp);

  const std::list<te::map::AbstractLayerPtr>& currentLayers = te::layout::Property::GetValueAs< std::list<te::map::AbstractLayerPtr> > (prop);
  std::list<te::map::AbstractLayerPtr>::const_iterator itLayers = currentLayers.begin();
  while (itLayers != currentLayers.end())
  {
    layers.push_back(*itLayers);
    ++itLayers;
  }

  return layers;
}

std::list<te::map::AbstractLayerPtr> te::layout::MapLayerChoiceController::getSelectedlayers()
{
  std::list<te::map::AbstractLayerPtr> selectedLayers = searchLayers();
  
  return selectedLayers;
}
