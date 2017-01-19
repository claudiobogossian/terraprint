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
  \file terralib/layout/qt/core/pattern/factory/PointItemFactory.cpp

  \brief This is the concrete factory for point item.
*/

// TerraLib
#include "PointItemFactory.h"
#include "../../../../../core/enum/Enums.h"
#include "../../../../../item/PointModel.h"
#include "../../../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../../item/PointItem.h"

te::layout::AbstractItemView* te::layout::PointItemFactory::build(ItemFactoryParamsCreate params)
{
  Properties      props = params.getProperties(); 

  PointModel *model = new PointModel();
  AbstractItemController* controller = new AbstractItemController(model, 0);
  PointItem* view = new PointItem(controller);
  controller->setView(view);

  if (props.getProperties().empty())
  {
    props = convertToProperties(params);
  }
  controller->setProperties(props);

  return view;
}

te::layout::PointItemFactory::PointItemFactory() :
  ItemFactory(Enums::getInstance().getEnumObjectType()->getPointItem()->getName())
{

}

te::layout::PointItemFactory::~PointItemFactory()
{

}
