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
  \file terralib/layout/qt/core/pattern/factory/MapCompositionItemFactory.cpp

  \brief This is the concrete factory for item group item.
*/

// TerraLib
#include "MapCompositionItemFactory.h"
#include "../../../../item/MapCompositionController.h"
#include "../../../../../core/enum/Enums.h"
#include "../../../../../item/MapCompositionModel.h"
#include "../../../../item/MapCompositionItem.h"

te::layout::AbstractItemView* te::layout::MapCompositionItemFactory::build(ItemFactoryParamsCreate params)
{
  Properties props = params.getProperties(); 
  const Property& propName = props.getProperty("name");
  std::string name = te::layout::Property::GetValueAs<std::string>(propName);

  MapCompositionModel* model = new MapCompositionModel();
<<<<<<< HEAD
  MapCompositionController* controller = new MapCompositionController(model, 0);
  MapCompositionItem* view = new MapCompositionItem(controller, params.getName());
=======
  MapCompositionController* controller = new MapCompositionController(model);
  MapCompositionItem* view = new MapCompositionItem(controller, name);
>>>>>>> Creation of items so that they receive all as properties at once just after their creation, thus reducing unnecessary overheads;
  controller->setView(view);

  props.setTypeObj(model->getType());
  std::string parentName = model->getType()->getName();
  props.reparentProperties(parentName);

  controller->setProperties(props);

  return view;
}

te::layout::MapCompositionItemFactory::MapCompositionItemFactory() :
  ItemFactory(Enums::getInstance().getEnumObjectType()->getMapCompositionItem()->getName())
{

}

te::layout::MapCompositionItemFactory::~MapCompositionItemFactory()
{

}

