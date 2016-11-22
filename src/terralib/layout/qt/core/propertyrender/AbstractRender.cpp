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
  \file AbstractRender.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AbstractRender.h"
#include <terralib/layout/core/property/Property.h>
#include <terralib/layout/qt/core/ItemUtils.h>

// Qt
#include <QVariant>

te::layout::AbstractRender::AbstractRender()
{

}

te::layout::AbstractRender::~AbstractRender()
{

}

QString te::layout::AbstractRender::propertyToQString(QVariant& value)
{
  te::layout::Property prop = qvariant_cast<te::layout::Property>(value);
  const te::dt::AbstractData* absData = prop.getValue();

  QString qValue;
  if (absData)
  {
    std::string svalue = absData->toString();
    qValue = QString(te::layout::ItemUtils::convert2QString(svalue));
  }
  return qValue;
}
