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
  \file ScaleSettingsController.h
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ScaleSettingsController.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../outside/ScaleSettingsModel.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../core/ItemUtils.h"
#include "../core/Scene.h"
#include "../item/ScaleItem.h"

// Qt
#include <QGraphicsItem>

te::layout::ScaleSettingsController::ScaleSettingsController(Scene* scene, AbstractOutsideModel* o) :
  AbstractOutsideController(o),
  m_scene(scene)
{
  
}

te::layout::ScaleSettingsController::~ScaleSettingsController()
{

}

te::layout::Property te::layout::ScaleSettingsController::getScaleProperty(std::string name)
{
  Property prop;

  QList<QGraphicsItem*> items = m_scene->selectedItems();
  if (items.isEmpty())
    return prop;

  QGraphicsItem* item = items.first();
  ScaleItem* scaleItem = dynamic_cast<ScaleItem*>(item);
  if (scaleItem)
  {
    prop = scaleItem->getController()->getProperty(name);
  }
  return prop;
}

QStringList te::layout::ScaleSettingsController::getItemNames(QStringList list, const EnumType* type)
{
  QList<QGraphicsItem*> items = m_scene->items();
  foreach(QGraphicsItem* item, items)
  {
    if (item)
    {
      AbstractItemView* view = dynamic_cast<AbstractItemView*>(item);
      if (view)
      {
        const Property& prop_name = view->getController()->getProperty("name");

        std::string value = prop_name.getValue().toString();
        QString txt = ItemUtils::convert2QString(value);

        const Properties& prop_type = view->getController()->getProperties();

        if (txt.compare("") != 0 && prop_type.getTypeObj() == type)
        {
          list.append(txt);
        }
      }
    }
  }

  return list;
}

