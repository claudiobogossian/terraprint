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
  \file BoolCheckBoxEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BoolCheckBoxEditor.h"
#include "../../../core/enum/Enums.h"

te::layout::BoolCheckBoxEditor::BoolCheckBoxEditor(const QModelIndex& index, QWidget* parent) :
  QCheckBox(parent),
  AbstractEditor(index, Enums::getInstance().getEnumDataType()->getDataTypeBool())
{
  changeEditorData(index);
}

te::layout::BoolCheckBoxEditor::~BoolCheckBoxEditor()
{

}

QVariant te::layout::BoolCheckBoxEditor::getValue()
{
  return this->text();
}

void te::layout::BoolCheckBoxEditor::changeEditorData(const QModelIndex& index)
{
  EnumDataType* propertyData = Enums::getInstance().getEnumDataType();
  QVariant variant = index.data(propertyData->getDataTypeBool()->getId());
  if (variant.isValid() && !variant.isNull())
  {
    te::layout::Property prop = qvariant_cast<te::layout::Property>(variant);
    bool newValue = te::layout::Property::GetValueAs<bool>(prop);
    setChecked(newValue);

    QString txt = variant.toString();
    setText(txt);
  }
}

