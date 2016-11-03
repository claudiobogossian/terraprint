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
  \file ColorEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ColorEditor.h"
#include "../../../core/enum/Enums.h"
#include "../../../core/property/Property.h"

// Qt
#include <QString>
#include <QVariant>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QEvent>
#include <QKeyEvent>
#include <QStyleOption>
#include <QPainter>
#include <QStyle>
#include <QApplication>
#include <QFontDialog>
#include <QColorDialog>

te::layout::ColorEditor::ColorEditor(const QModelIndex& index, QWidget* parent) :
  QWidget(parent),
  AbstractEditor(index, Enums::getInstance().getEnumDataType()->getDataTypeInt()),
  m_textLabel(0),
  m_button(0)
{
  createGroupBox();
  changeEditorData(index);
}

te::layout::ColorEditor::~ColorEditor()
{

}

QVariant te::layout::ColorEditor::getValue()
{
  return m_textLabel->text();
}

void te::layout::ColorEditor::changeEditorData(const QModelIndex& index)
{
  EnumDataType* propertyData = Enums::getInstance().getEnumDataType();
  QVariant variant = index.data(propertyData->getDataTypeFont()->getId());
  if (variant.isValid() && !variant.isNull())
  {
    m_color = qvariant_cast<QColor>(variant); // cast to custom type
    if (m_textLabel)
    {
      m_textLabel->setText(m_color.name());
    }
  }
}

void te::layout::ColorEditor::createGroupBox()
{
  QHBoxLayout* hlayout = new QHBoxLayout(this);

  setupTreeViewEditorMargin(hlayout);
  m_textLabel = new QLabel("");

  QColor color(255, 255, 255);
  m_textLabel->setPalette(QPalette(color));
  m_textLabel->setAutoFillBackground(true);

  m_button = new QToolButton(this);
  m_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
  m_button->setFixedWidth(20);
  m_button->setText("...");
  m_button->installEventFilter(this);

  connect(m_button, SIGNAL(clicked(bool)), this, SLOT(onButtonClicked(bool)));
  
  setFocusProxy(m_button);
  setFocusPolicy(m_button->focusPolicy());

  hlayout->setSpacing(0);
  hlayout->addWidget(m_textLabel);

  QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Ignored);
  hlayout->addItem(spacer);

  hlayout->addWidget(m_button);

  setLayout(hlayout);
}

bool te::layout::ColorEditor::eventFilter(QObject * watched, QEvent * event)
{
  if (watched == m_button)
  {
    if (event->type() == QEvent::KeyRelease)
    {
      QKeyEvent* kEvent = dynamic_cast<QKeyEvent*>(event);
      if (kEvent)
      {
        if (kEvent->key() == Qt::Key_Return)
        {
          event->ignore();
          return true;
        }
      }
    }
  }
  return QWidget::eventFilter(watched, event);
}

void te::layout::ColorEditor::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void te::layout::ColorEditor::onButtonClicked(bool checked)
{
  emit showDlg();
  getColor();
}

void te::layout::ColorEditor::getColor()
{
  QColor color = configColor(m_textLabel);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property prop;
  prop.setName("");
  //prop.setValue(newFont, dataType->getDataTypeFont());

  //Emit our own signal.
  emit dataValueChanged(this, prop);
}

void te::layout::ColorEditor::setupTreeViewEditorMargin(QLayout* layout)
{
  int decorationMargin = 4;

  if(QApplication::layoutDirection() == Qt::LeftToRight)
  {
    layout->setContentsMargins(decorationMargin, 0, 0, 0);
  }
  else
  {
    layout->setContentsMargins(0, 0, decorationMargin, 0);
  }
}

QColor te::layout::ColorEditor::configColor(QWidget* widget)
{
  QPalette ptt(widget->palette());
  QBrush brush = ptt.brush(widget->backgroundRole());
  
  QColor color = QColorDialog::getColor(brush.color(), this, tr("Select Color"));

  if (color.isValid())
  {
    QPalette paltt(widget->palette());
    paltt.setColor(widget->backgroundRole(), color);
    widget->setPalette(paltt);
    widget->setAutoFillBackground(true);
  }
  
  return color;
}

