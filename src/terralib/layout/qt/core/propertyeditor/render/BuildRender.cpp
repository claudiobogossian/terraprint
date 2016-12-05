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
  \file BuildRender.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BuildRender.h"
#include "../../../../core/enum/Enums.h"
#include "../../pattern/factory/propertyeditor/render/RenderFactory.h"
#include "../../pattern/factory/propertyeditor/render/RenderFactoryParamsCreate.h"
#include "../../../../core/property/Property.h"

// STL
#include <string>

// Qt
#include <QMetaType>
#include <QModelIndex>
#include <QVariant>

te::layout::BuildRender::BuildRender()
{
  
}

te::layout::BuildRender::~BuildRender()
{
 
}

te::layout::AbstractRender* te::layout::BuildRender::buildRender(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  RenderFactoryParamsCreate params;

  int propertyType = qMetaTypeId<te::layout::Property>();
  QVariant variant = index.data(propertyType);

  te::layout::Property prop = qvariant_cast<te::layout::Property>(variant);

  te::common::ParameterizedAbstractFactory<AbstractRender, std::string, RenderFactoryParamsCreate>::dictionary_type& d = te::common::ParameterizedAbstractFactory<AbstractRender, std::string, RenderFactoryParamsCreate>::getDictionary();

  std::string renderName = prop.getType()->getName();
  RenderFactory* fact = dynamic_cast<RenderFactory*>(d.find(renderName));
  
  AbstractRender* render = 0;
  if (fact)
  {
    render = te::layout::RenderFactory::make(renderName, params);
  }
  return render;
}

