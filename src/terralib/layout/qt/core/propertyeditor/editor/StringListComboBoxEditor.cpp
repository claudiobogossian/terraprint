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
  \file StringListComboBoxEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "StringListComboBoxEditor.h"
#include "../../../../core/enum/Enums.h"
#include "../../../../qt/core/ItemUtils.h"

//Qt
#include <QMetaType>
#include <QStringList>

te::layout::StringListComboBoxEditor::StringListComboBoxEditor(const QModelIndex& index, QWidget* parent) :
  QComboBox(parent),
  AbstractEditor(index, Enums::getInstance().getEnumDataType()->getDataTypeStringList())
{
  changeEditorData(index);
}

te::layout::StringListComboBoxEditor::~StringListComboBoxEditor()
{

}

QVariant te::layout::StringListComboBoxEditor::getValue()
{
  return this->currentText();
}

void te::layout::StringListComboBoxEditor::changeEditorData(const QModelIndex& index)
{
  int propertyType = qMetaTypeId<te::layout::Property>();
  QVariant variant = index.data(propertyType);
  if (variant.isValid() && !variant.isNull())
  {
    m_property = qvariant_cast<te::layout::Property>(variant);
    std::string newValue = te::layout::Property::GetValueAs<std::string>(m_property);

    std::vector<te::layout::Variant> options = m_property.getOptionChoices();
    addComboOptions(options);
  }
}

void te::layout::StringListComboBoxEditor::addComboOptions(const std::vector<te::layout::Variant>& options)
{
  clear();

  QStringList list;
  for (std::vector<Variant>::const_iterator it = options.begin(); it != options.end(); ++it)
  {
    std::string value = (*it).toString();
    QString qValue = te::layout::ItemUtils::convert2QString(value);
    list.append(qValue);
  }
  addItems(list);

  int index = -1;
  QVariant variant;

  //When the value is not a QString
  QString value = variant.toString();
  variant.setValue(value);

  index = findData(variant, Qt::DisplayRole);
  if (index == -1)
  {
    index = findText(value);
    if (index != -1)
    {
      setCurrentIndex(index);
    }
  }
  else
  {
    setCurrentIndex(index);
  }
}
