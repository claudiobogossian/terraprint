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
  \file DoubleSpinBoxEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "DoubleSpinBoxEditor.h"
#include "../../../core/enum/Enums.h"

//Qt
#include <QMetaType>

te::layout::DoubleSpinBoxEditor::DoubleSpinBoxEditor(const QModelIndex& index, QWidget* parent) :
  QDoubleSpinBox(parent),
  AbstractEditor(index, Enums::getInstance().getEnumDataType()->getDataTypeDouble())
{
  changeEditorData(index);
  // connect signal / slot
  connect(this, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));
}

te::layout::DoubleSpinBoxEditor::~DoubleSpinBoxEditor()
{

}

QVariant te::layout::DoubleSpinBoxEditor::getValue()
{
  return this->value();
}

void te::layout::DoubleSpinBoxEditor::changeEditorData(const QModelIndex& index)
{
  int propertyType = qRegisterMetaType<te::layout::Property>("te::layout::Property");
  QVariant variant = index.data(propertyType);
  if (variant.isValid() && !variant.isNull())
  {
    m_property = qvariant_cast<te::layout::Property>(variant);
    double newValue = te::layout::Property::GetValueAs<double>(m_property);
    setValue(newValue);
  }
}

void te::layout::DoubleSpinBoxEditor::onValueChanged(double d)
{
  double oldValue = te::layout::Property::GetValueAs<double>(m_property);
  if (oldValue == d)
    return;

  EnumType* type = getType();
  m_property.setValue(d, type);

  emit dataValueChanged(this, m_property);
}
