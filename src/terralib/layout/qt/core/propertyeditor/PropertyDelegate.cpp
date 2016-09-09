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
  \file PropertyDelegate.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PropertyDelegate.h"
#include "AbstractEditor.h"
#include "BuildEditor.h"

te::layout::PropertyDelegate::PropertyDelegate(QObject* parent) :
  QItemDelegate(parent)
{
  
}

te::layout::PropertyDelegate::~PropertyDelegate()
{

}

void te::layout::PropertyDelegate::setProperties(std::vector<Property> vprops)
{
  m_vprops = vprops;
}

QWidget * te::layout::PropertyDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  if (index.column() == 0)
  {
    return QItemDelegate::createEditor(parent, option, index);
  }
  return createFromFactory(parent, option, index);
}

void te::layout::PropertyDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
  AbstractEditor* abstractEditor = dynamic_cast<AbstractEditor*>(editor);
  if (abstractEditor)
  {
    abstractEditor->setEditorData(index);
  }
}

void te::layout::PropertyDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
  AbstractEditor* abstractEditor = dynamic_cast<AbstractEditor*>(editor);
  if (abstractEditor)
  {
    QVariant value = abstractEditor->getValue();
    model->setData(index, value);
  }
}

void te::layout::PropertyDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  editor->setGeometry(option.rect);
}

QWidget* te::layout::PropertyDelegate::createFromFactory(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QWidget* editor = 0;

  BuildEditor build;
  AbstractEditor* abstractEditor = build.buildEditor(m_vprops, index, parent);  
  if (abstractEditor)
  {
    editor = dynamic_cast<QWidget*>(abstractEditor);
  }

  return editor;
}

void te::layout::PropertyDelegate::onPropertiesChanged(std::vector<Property> vprops)
{

}
