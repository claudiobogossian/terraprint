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
  \file SpinBoxEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "SpinBoxEditor.h"
#include "../../../core/enum/Enums.h"

te::layout::SpinBoxEditor::SpinBoxEditor(const QModelIndex& index, QWidget* parent):
  QSpinBox(parent),
  AbstractEditor(index)
{
  EnumDataType* propertyData = Enums::getInstance().getEnumDataType();
  QVariant variant = index.data(propertyData->getDataTypeInt()->getId());
  if (variant.isValid() && !variant.isNull())
  {
    if (variant.canConvert(QVariant::Int))
    {
      int newValue = variant.toInt();
      setValue(newValue);
    }
  }
}

te::layout::SpinBoxEditor::~SpinBoxEditor()
{

}

void te::layout::SpinBoxEditor::setProperties(std::vector<Property> vprops)
{
  m_vprops = vprops;
}

void te::layout::SpinBoxEditor::setEditorData(const QModelIndex& index)
{
  EnumDataType* propertyData = Enums::getInstance().getEnumDataType();
  QVariant variant = index.data(propertyData->getDataTypeInt()->getId());
  if (variant.isValid() && !variant.isNull())
  {
    if (variant.canConvert(QVariant::Int))
    {
      int newValue = variant.toInt();
      setValue(newValue);
    }
  }
}

QVariant te::layout::SpinBoxEditor::getValue()
{
  return this->value();
}

