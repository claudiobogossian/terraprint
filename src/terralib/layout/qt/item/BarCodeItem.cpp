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
  \file BarCodeItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BarCodeItem.h"
#include "BarCodeController.h"
#include "../../item/BarCodeModel.h"


// Qt
#include <QStyleOptionGraphicsItem>

te::layout::BarCodeItem::BarCodeItem(te::layout::ItemInputProxy* itemInputProxy)
  : TextItem(itemInputProxy)
{
 
}

te::layout::BarCodeItem::~BarCodeItem()
{

}

te::layout::AbstractItemModel* te::layout::BarCodeItem::createModel() const
{
  return new BarCodeModel();
}

te::layout::AbstractItemController* te::layout::BarCodeItem::createController() const
{
  AbstractItemModel* model = createModel();
  return new BarCodeController(model, (AbstractItemView*)this);
}

void te::layout::BarCodeItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /* = 0 */ )
{
  TextItem::paint(painter, option, widget);
}
