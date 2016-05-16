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
  \file SVGDialogController.h
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "SVGDialogController.h"
#include "SVGDialogModel.h"
#include "../qt/item/MapItem.h"
#include "../qt/core/ItemUtils.h"

te::layout::SVGDialogController::SVGDialogController(Scene * scene, AbstractProxyProject * proxy, AbstractOutsideModel* o) :
  AbstractOutsideController(o),
  m_scene(scene),
  m_proxy(proxy)
{
  
}

te::layout::SVGDialogController::~SVGDialogController()
{

}

/*
te::layout::Property te::layout::SVGDialogController::getProperty(std::string name)
{
  SVGDialogModel* outsideModel = 0;
  Property prop;

  if (m_model)
  {
    AbstractOutsideModel* model = dynamic_cast<AbstractOutsideModel*>(m_model);
    outsideModel = dynamic_cast<SVGDialogModel*>(model);
  }

  if (!outsideModel)
    return prop;

  prop = outsideModel->containsOutsideSubProperty(name);

  return prop;
}*/


te::layout::Property te::layout::SVGDialogController::getProperty(std::string name)
{
  Property prop;

  ItemUtils utils(m_scene);
  AbstractItemView* view = utils.getSelectedItem();

  prop = view->getController()->getProperty(name);

  /*
  if (vecMap.empty() == true)
    return prop;

  //seraches for the selected MapItem
  for (size_t i = 0; i < vecMap.size(); ++i)
  {
    MapItem* mapItem = vecMap[i];

    if (mapItem->isSelected() == true)
    {
      prop = mapItem->getController()->getProperty(name);
      break;
    }
    else if (mapItem->parentItem() != 0 && mapItem->parentItem()->isSelected())
    {
      prop = mapItem->getController()->getProperty(name);
      break;
    }
  }
  */
  return prop;
}
