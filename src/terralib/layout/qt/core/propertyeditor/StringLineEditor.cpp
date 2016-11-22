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
  \file StringLineEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "StringLineEditor.h"
#include "../../../core/enum/Enums.h"
#include <terralib/layout/qt/core/ItemUtils.h>

// Qt
#include <QMetaType>

te::layout::StringLineEditor::StringLineEditor(const QModelIndex& index, QWidget* parent) :
  QLineEdit(parent),
  AbstractEditor(index, Enums::getInstance().getEnumDataType()->getDataTypeString())
{
  changeEditorData(index);
}

te::layout::StringLineEditor::~StringLineEditor()
{

}

QVariant te::layout::StringLineEditor::getValue()
{
  return this->text();
}

void te::layout::StringLineEditor::changeEditorData(const QModelIndex& index)
{
  int propertyType = qRegisterMetaType<te::layout::Property>("te::layout::Property");
  QVariant variant = index.data(propertyType);
  if (variant.isValid() && !variant.isNull())
  {
    m_property = qvariant_cast<te::layout::Property>(variant);
    std::string newValue = te::layout::Property::GetValueAs<std::string>(m_property);

    QString qValue = ItemUtils::convert2QString(newValue);
    setText(qValue);
  }
}

