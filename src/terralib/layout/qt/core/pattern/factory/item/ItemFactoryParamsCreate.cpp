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
  \file terralib/layout/qt/core/pattern/factory/ItemFactoryParamsCreate.cpp

  \brief Parameters to create a new item object (MVC graphic Object). 
*/

// TerraLib
#include "ItemFactoryParamsCreate.h"

te::layout::ItemFactoryParamsCreate::ItemFactoryParamsCreate(const ItemFactoryParamsCreate& rhs)
{
  this->operator=(rhs);
}

te::layout::ItemFactoryParamsCreate::ItemFactoryParamsCreate(const Properties& props):
  m_props(props)
{

}

te::layout::ItemFactoryParamsCreate::~ItemFactoryParamsCreate()
{

}

te::common::AbstractParameters* te::layout::ItemFactoryParamsCreate::clone() const
{
  return new ItemFactoryParamsCreate(*this);
}

void te::layout::ItemFactoryParamsCreate::reset() throw(te::common::Exception)
{
  m_props.clear();
}

te::layout::Properties te::layout::ItemFactoryParamsCreate::getProperties() const
{
  return m_props;
}

