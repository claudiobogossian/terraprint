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
  \file terralib/layout/qt/core/pattern/factory/editor/EditorFactoryParamsCreate.cpp

  \brief Parameters to create a new tool. 
*/

// TerraLib
#include "EditorFactoryParamsCreate.h"

te::layout::EditorFactoryParamsCreate::EditorFactoryParamsCreate(const QModelIndex& index, std::vector<Property> vprops, QWidget* parent) :
  m_index(index),
  m_vprops(vprops),
  m_parent(parent)
{

}

te::layout::EditorFactoryParamsCreate::EditorFactoryParamsCreate(const EditorFactoryParamsCreate& rhs)
{
  this->operator=(rhs);
}

te::layout::EditorFactoryParamsCreate::~EditorFactoryParamsCreate()
{

}

te::common::AbstractParameters* te::layout::EditorFactoryParamsCreate::clone() const
{
  return new EditorFactoryParamsCreate(*this);
}

void te::layout::EditorFactoryParamsCreate::reset() throw(te::common::Exception)
{

}

QModelIndex te::layout::EditorFactoryParamsCreate::getModelIndex()
{
  return m_index;
}

QWidget* te::layout::EditorFactoryParamsCreate::getParent()
{
  return m_parent;
}

std::vector<te::layout::Property> te::layout::EditorFactoryParamsCreate::getProperties()
{
  return m_vprops;
}

