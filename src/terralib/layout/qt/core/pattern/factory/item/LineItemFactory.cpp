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
  \file terralib/layout/qt/core/pattern/factory/LineItemFactory.cpp

  \brief This is the concrete factory for line item.
*/

// TerraLib
#include "LineItemFactory.h"
#include "../../../../../core/enum/Enums.h"
#include "../../../../item/LineItem.h"

te::layout::AbstractItemView* te::layout::LineItemFactory::build(ItemFactoryParamsCreate params)
{  
  LineItem* view = new LineItem(params.getItemInputProxy());

  const Properties& props = params.getProperties();

  view->setProperties(props);
  return view;
}

te::layout::LineItemFactory::LineItemFactory() :
  ItemFactory(Enums::getInstance().getEnumObjectType()->getLineItem()->getName())
{

}

te::layout::LineItemFactory::~LineItemFactory()
{

}
