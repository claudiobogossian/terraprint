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
  \file GridSettingsController.h
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridSettingsController.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/enum/Enums.h"
#include "../../outside/GridSettingsModel.h"
#include "../core/ItemUtils.h"
#include "../core/Scene.h"

// Qt
#include <QGraphicsItem>

te::layout::GridSettingsController::GridSettingsController(Scene* scene, AbstractOutsideModel* o) :
  AbstractOutsideController(o),
  m_scene(scene)
{
  
}

te::layout::GridSettingsController::~GridSettingsController()
{

}

te::layout::Property te::layout::GridSettingsController::getProperty( std::string name, EnumType* enumType )
{
  Property prop;

  AbstractItemView* grid = searchGrid(enumType);
  if (grid)
  {
    prop = grid->getController()->getProperty(name);
  }

  return prop;
}

QStringList te::layout::GridSettingsController::getItemNames(QStringList list, const EnumType* type)
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

        std::string value = te::layout::Property::GetValueAs<std::string>(prop_name);
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

bool te::layout::GridSettingsController::containsGrid(EnumType* enumType)
{
  bool result = false;
  if (searchGrid(enumType))
  {
    result = true;
  }
  return result;
}

te::layout::AbstractItemView* te::layout::GridSettingsController::searchGrid(EnumType* enumType)
{
  QList<QGraphicsItem*> items = m_scene->items();
  foreach(QGraphicsItem* item, items)
  {
    AbstractItemView* absItem = dynamic_cast<AbstractItemView*>(item);
    if (absItem)
    {
      const Properties& properties = absItem->getProperties();
      if (properties.getTypeObj()->getName() == Enums::getInstance().getEnumObjectType()->getMapItem()->getName())
      {
        return absItem;
      }
    }
  }
  return 0;

  /*
  AbstractItemView* itemFound = 0;

  QList<QGraphicsItem*> items = m_scene->items();
  foreach(QGraphicsItem* item, items)
  {
    AbstractItemView* absItem = dynamic_cast<AbstractItemView*>(item);
    if (absItem)
    {
      if (absItem->getController()->getProperties().getTypeObj() == enumType)
      {
        if (item->isSelected())
        {
          itemFound = absItem;
          break;
        }
        else
        {
          if (item->parentItem())
          {
            if (item->parentItem()->isSelected())
            {
              itemFound = absItem;
              break;
            }
          }
        }
      }
    }
  }
  return itemFound;
  */
}

