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
  \file NorthSettingsController.h
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "../../qt/outside/NorthSettingsController.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../outside/NorthSettingsModel.h"
#include "../../qt/item/NorthItem.h"
#include "../../qt/core/Scene.h"

#include <QGraphicsItem>


te::layout::NorthSettingsController::NorthSettingsController(Scene* scene, AbstractOutsideModel* o) :
  AbstractOutsideController(o),
  m_scene(scene)
{
  
}

te::layout::NorthSettingsController::~NorthSettingsController()
{

}

te::layout::Property te::layout::NorthSettingsController::getNorthProperty(std::string name)
{
  Property prop;

  QList<QGraphicsItem*> items = m_scene->selectedItems();
  if (items.isEmpty())
    return prop;

  QGraphicsItem* item = items.first();
  NorthItem* northItem = dynamic_cast<NorthItem*>(item);
  if (northItem)
  {
    prop = northItem->getController()->getProperty(name);
  }
  return prop;
}