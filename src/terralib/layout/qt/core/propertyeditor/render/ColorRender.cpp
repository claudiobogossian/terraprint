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
  \file ColorRender.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ColorRender.h"
#include "../../../../core/property/Property.h"
#include "../../../../qt/core/ItemUtils.h"

// Qt
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>

te::layout::ColorRender::ColorRender()
{

}

te::layout::ColorRender::~ColorRender()
{

}

void te::layout::ColorRender::render(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  int propertyType = qMetaTypeId<te::layout::Property>();
  QVariant variant = index.data(propertyType);

  if (variant.isValid())
  {
    QString value = propertyToQString(variant);
    painter->drawText(option.rect, Qt::AlignLeft, value);
  }
}

