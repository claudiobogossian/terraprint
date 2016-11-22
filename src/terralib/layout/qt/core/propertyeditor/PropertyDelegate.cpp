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
#include "BuildPaintCustomData.h"

// Qt
#include <QApplication>
#include <QPainter>
#include <QDebug>

te::layout::PropertyDelegate::PropertyDelegate(QObject* parent) :
  QStyledItemDelegate(parent),
  m_currentEditor(0),
  m_currentEditorRow(-1),
  m_currentEditorColumn(-1)
{
  m_paintCustomDataType = new BuildPaintCustomData();
}

te::layout::PropertyDelegate::~PropertyDelegate()
{
  if (m_paintCustomDataType)
  {
    delete m_paintCustomDataType;
    m_paintCustomDataType = 0;
  }
}

void te::layout::PropertyDelegate::setProperties(const std::vector<Property>& vprops)
{
  m_vprops = vprops;
}

QWidget * te::layout::PropertyDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  if (index.column() == 0)
  {
    m_currentEditor =  QStyledItemDelegate::createEditor(parent, option, index);
  }
  else
  {
    m_currentEditor = createFromFactory(parent, option, index);
    if (m_currentEditor != 0)
    {
      m_currentEditorRow = index.row();
      m_currentEditorColumn = index.column();
    }
  }
  return m_currentEditor;
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
    int role = abstractEditor->getType()->getId(); // important because this delegate is using custom data type(role) and obvious not exist in Qt default implementation

    if (index.column() == 1)
    {
      QVariant modelData = model->data(index, role);
      QVariant indexData = index.data(1);
      if (modelData == indexData) 
        return;
    }
    model->setData(index, value, role);
  }
}

void te::layout::PropertyDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  editor->setGeometry(option.rect);
}

void te::layout::PropertyDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  if (index.column() == 1)
  {
    if (m_paintCustomDataType->paintCustomData(painter, option, index))
    {
      return;
    }
  }
  QStyledItemDelegate::paint(painter, option, index);
}

QWidget* te::layout::PropertyDelegate::createFromFactory(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QWidget* editor = 0;

  BuildEditor build;
  AbstractEditor* abstractEditor = build.buildEditor(m_vprops, index, parent);  
  if (abstractEditor)
  {
    editor = dynamic_cast<QWidget*>(abstractEditor);
    if (editor)
    {
      // connect signal / slot
      connect(editor, SIGNAL(dataValueChanged(QWidget*, Property)), this, SLOT(onDataValueChanged(QWidget*, Property)));
    }
  }
  return editor;
}

void te::layout::PropertyDelegate::onDataValueChanged(QWidget* widget, const Property& prop)
{
  if (widget == m_currentEditor)
  {
    // Signal from QItemDelegate
    commitData(widget); // call setEditorData()
    emit dataEditorChanged(prop, m_currentEditorRow, m_currentEditorColumn);
  }
}

