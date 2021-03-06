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
\file MapLocationSettingsController.h

\brief

\ingroup layout
*/

// TerraLib
#include "MapLocationSettingsController.h"
#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../outside/MapSettingsModel.h"
#include "../core/BuildGraphicsOutside.h"
#include "../outside/MapLayerChoiceOutside.h"
#include "../item/MapItem.h"
#include "../../outside/MapLayerChoiceController.h"

# include <QGraphicsItem>

te::layout::MapLocationSettingsController::MapLocationSettingsController(Scene * scene, AbstractProxyProject * proxy, QWidget* parent, AbstractOutsideModel* o) :
AbstractOutsideController(o),
m_scene(scene),
m_proxy(proxy),
m_parent(parent)
{
  
}

te::layout::MapLocationSettingsController::~MapLocationSettingsController()
{

}

te::layout::Property te::layout::MapLocationSettingsController::getProperty(std::string name)
{
  Property prop;

  ItemUtils utils(m_scene);
  std::vector<MapItem*> mapList = utils.getMapItemList();

  if (mapList.size() == 0)
    return prop;

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
    else if (mapItem->parentItem() != 0 && mapItem->parentItem()->isSelected())
    {
      prop = mapItem->getProperty(name);
      break;
    }
  }

  return prop;
}

te::layout::MapLayerChoiceOutside*  te::layout::MapLocationSettingsController::getMapLayerChoice(){
  
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

  BuildGraphicsOutside build;
  
  QWidget* widget = build.createOutside(enumObj->getMapLayerChoice(), m_scene, m_parent, m_proxy);
  MapLayerChoiceOutside* controllerSettings = dynamic_cast<MapLayerChoiceOutside*>(widget);

  AbstractOutsideController* abstractControllerLayer = const_cast<AbstractOutsideController*>(controllerSettings->getController());
  MapLayerChoiceController * mapLayerChoiceController = dynamic_cast<MapLayerChoiceController*>(abstractControllerLayer);

  return controllerSettings;
}



QStringList te::layout::MapLocationSettingsController::getItemNames(QStringList list, const EnumType* type)
{
  QList<QGraphicsItem*> items = m_scene->items();
  foreach(QGraphicsItem* item, items)
  {
    if (item)
    {
      AbstractItemView* view = dynamic_cast<AbstractItemView*>(item);
      if (view)
      {
        const Property& prop_name = view->getProperty("name");

        std::string value = te::layout::Property::GetValueAs<std::string>(prop_name);
        QString txt = ItemUtils::convert2QString(value);

        const Properties& prop_type = view->getProperties();

        if (txt.compare("") != 0 && prop_type.getTypeObj() == type)
        {
          list.append(txt);
        }
      }
    }
  }

  return list;
}

