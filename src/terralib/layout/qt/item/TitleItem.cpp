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
  \file TitleItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TitleItem.h"
#include "../../item/TitleModel.h"
#include "TitleController.h"

te::layout::TitleItem::TitleItem(te::layout::ItemInputProxy* itemInputProxy) 
  : TextGridItem(itemInputProxy)
{
}

te::layout::TitleItem::~TitleItem()
{
}

te::layout::AbstractItemModel* te::layout::TitleItem::createModel() const
{
  return new TitleModel();
}

te::layout::AbstractItemController* te::layout::TitleItem::createController() const
{
  AbstractItemModel* model = createModel();
  return new TitleController(model, (AbstractItemView*)this);
}
