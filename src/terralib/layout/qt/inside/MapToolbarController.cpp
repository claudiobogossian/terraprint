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
  \file ToolbarItemController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapToolbarController.h"
#include "MapToolbarInside.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/property/Properties.h"
#include "MapToolbarInside.h"
#include "../../../layout/core/enum/Enums.h"
#include "../../../layout/core/enum/EnumModeType.h"
#include "../item/MapItem.h"

// Qt
#include <QToolButton>
#include <QAction>

te::layout::MapToolbarController::MapToolbarController() :
  m_toolbarItem(0)
{
  
}

te::layout::MapToolbarController::~MapToolbarController()
{

}

void te::layout::MapToolbarController::setMapToolbarInside(MapToolbarInside* toolbarInside)
{
  m_toolbarItem = toolbarInside;
}

void te::layout::MapToolbarController::onPanToolClicked(bool checked)
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeMapTool(type->getModeMapPan());
}

void te::layout::MapToolbarController::onZoomInClicked(bool checked)
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeMapTool(type->getModeMapZoomIn());
}

void te::layout::MapToolbarController::onZoomOutClicked(bool checked)
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeMapTool(type->getModeMapZoomOut());
}

void te::layout::MapToolbarController::onRecomposeClicked(bool checked)
{
  EnumModeType* type = Enums::getInstance().getEnumModeType();
  changeMapTool(type->getModeMapRecompose());
}

void te::layout::MapToolbarController::onSystematicScaleClicked(bool checked)
{
  // do nothing
}

void te::layout::MapToolbarController::onTextGridAsObjectClicked(bool checked)
{
  // do nothing
}

void te::layout::MapToolbarController::onTextMapAsObjectClicked(bool checked)
{
  // do nothing
}

void te::layout::MapToolbarController::onLegendAsObjectClicked(bool checked)
{
  // do nothing
}

void te::layout::MapToolbarController::changeMapTool(EnumType* tool)
{
  if (m_toolbarItem)
  {
    AbstractItemView* item = m_toolbarItem->getItem();
    if (item)
    {
      MapItem* mapItem = dynamic_cast<MapItem*>(item);
      if (mapItem)
      {
        mapItem->changeCurrentTool(tool);
      }
    }
  }
}

