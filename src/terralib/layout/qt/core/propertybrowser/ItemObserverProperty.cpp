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
    \file ItemObserverProperty.cpp

    \brief

    \ingroup layout
*/

// TerraLib
#include "ItemObserverProperty.h"

#include <QComboBox>
#include <QFocusEvent>
#include <QHBoxLayout>

te::layout::ItemObserverProperty::ItemObserverProperty(QWidget *parent)
    : QWidget(parent)
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0);
  layout->setSpacing(0);

  m_cbNames = new QComboBox(this);
  m_cbNames->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
  
  layout->addWidget(m_cbNames);

  setFocusProxy(m_cbNames);
  setFocusPolicy(Qt::StrongFocus);
  setAttribute(Qt::WA_InputMethodEnabled);

  connect(m_cbNames, SIGNAL(currentIndexChanged(const QString &)),
    this, SIGNAL(currentNameChanged(const QString &)));
}

te::layout::ItemObserverProperty::~ItemObserverProperty()
{

}

void te::layout::ItemObserverProperty::setCurrentName(const QString &name)
{
  if (m_cbNames->count() == 0 && name.compare("") != 0)
    m_cbNames->addItem(name);
  
  int index = m_cbNames->findText(name);
  if (index != -1)
    m_cbNames->setCurrentIndex(index);
}

QString te::layout::ItemObserverProperty::currentName() const
{
  QString name = m_cbNames->currentText();
  return name;
}

void te::layout::ItemObserverProperty::setNameList(const QStringList & names)
{
  QString currentName = m_cbNames->currentText();

  m_cbNames->clear();

  QStringList newNames;
  newNames.append(""); //first position
  newNames.append(names);
  
  m_cbNames->addItems(newNames);
  
  if(currentName.compare("") != 0)
  {
    setCurrentName(currentName);
  }
}

void te::layout::ItemObserverProperty::focusInEvent(QFocusEvent *e)
{
  m_cbNames->event(e);
  QWidget::focusInEvent(e);
}

void te::layout::ItemObserverProperty::focusOutEvent(QFocusEvent *e)
{
  m_cbNames->event(e);
  QWidget::focusOutEvent(e);
}

void te::layout::ItemObserverProperty::keyPressEvent(QKeyEvent *e)
{
  m_cbNames->event(e);
}

void te::layout::ItemObserverProperty::keyReleaseEvent(QKeyEvent *e)
{
  m_cbNames->event(e);
}

