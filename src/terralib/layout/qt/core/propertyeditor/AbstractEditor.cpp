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
  \file AbstractEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AbstractEditor.h"


te::layout::AbstractEditor::AbstractEditor(const QModelIndex& index, EnumType* type)
{
  m_dataType = type; // type
}

te::layout::AbstractEditor::~AbstractEditor()
{

}

void te::layout::AbstractEditor::setProperties(const std::vector<Property>& vprops)
{
  m_vprops = vprops;
}

void te::layout::AbstractEditor::setEditorData(const QModelIndex& index)
{
  changeEditorData(index);
}

te::layout::EnumType* te::layout::AbstractEditor::getType()
{
  return m_dataType;
}
