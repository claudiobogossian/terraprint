/*  Copyright (C) 2001-2016 National Institute For Space Research (INPE) - Brazil.

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
\file MapSettingsController.h

\brief

\ingroup layout
*/

// TerraLib
#include "MapSettingsController.h"
#include "../core/pattern/mvc/AbstractOutsideModel.h"
#include "MapSettingsModel.h"
#include "../qt/core/BuildGraphicsOutside.h"
#include "../qt/outside/MapLayerChoiceOutside.h"
#include "QGraphicsItem"
#include "../qt/item/MapItem.h"
#include "../../layout/outside/MapLayerChoiceController.h"

te::layout::MapSettingsController::MapSettingsController(Scene * scene, AbstractProxyProject * proxy, QWidget* parent, AbstractOutsideModel* o) :
AbstractOutsideController(o),
m_scene(scene),
m_proxy(proxy),
m_parent(parent)
{
  
}

te::layout::MapSettingsController::~MapSettingsController()
{
 /* if (m_scene != 0)
  {
    delete m_scene;
    m_scene = 0;
  }

  if (m_proxy != 0)
  {
    delete m_proxy;
    m_proxy = 0;
  }
  */
}

te::layout::Property te::layout::MapSettingsController::getProperty(std::string name)
{
  Property prop;


  QList<QGraphicsItem*> items = m_scene->selectedItems();
  if (items.isEmpty())
    return prop;

  QGraphicsItem* item = items.first();
  MapItem* mapItem = dynamic_cast<MapItem*>(item);
  if (mapItem)
  {
    prop = mapItem->getController()->getProperty(name);
  }
  return prop;

}

te::layout::MapLayerChoiceOutside*  te::layout::MapSettingsController::getMapLayerChoice(){
  
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

  BuildGraphicsOutside build;
  
  QWidget* widget = build.createOutside(enumObj->getMapLayerChoice(), m_scene, m_parent, m_proxy);
  MapLayerChoiceOutside* controllerSettings = dynamic_cast<MapLayerChoiceOutside*>(widget);

  AbstractOutsideController* abstractControllerLayer = const_cast<AbstractOutsideController*>(controllerSettings->getController());
  MapLayerChoiceController * mapLayerChoiceController = dynamic_cast<MapLayerChoiceController*>(abstractControllerLayer);

  return controllerSettings;
}


