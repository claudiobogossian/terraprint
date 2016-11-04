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
  \file FontEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "FontEditor.h"
#include "../../../core/enum/Enums.h"
#include "../../../core/property/Property.h"
#include <terralib/layout/qt/core/ItemUtils.h>

//Qt
#include <QMetaType>
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
#include <QColor>

te::layout::FontEditor::FontEditor(const QModelIndex& index, QWidget* parent) :
  QWidget(parent),
  AbstractEditor(index, Enums::getInstance().getEnumDataType()->getDataTypeInt()),
  m_textLabel(0),
  m_button(0)
{
  createGroupBox();
  changeEditorData(index);
}

te::layout::FontEditor::~FontEditor()
{

}

QVariant te::layout::FontEditor::getValue()
{
  return m_textLabel->text();
}

void te::layout::FontEditor::changeEditorData(const QModelIndex& index)
{
  int propertyType = qRegisterMetaType<te::layout::Property>("te::layout::Property");
  QVariant variant = index.data(propertyType);
  if (variant.isValid() && !variant.isNull())
  {
    te::layout::Property prop = qvariant_cast<te::layout::Property>(variant);
    te::layout::Font newValue = te::layout::Property::GetValueAs<te::layout::Font>(prop);
    
    m_font = ItemUtils::convertToQfont(newValue);
    if (m_textLabel)
    {
      m_textLabel->setText(m_font.family());
    }
  }
}

void te::layout::FontEditor::createGroupBox()
{
  QHBoxLayout* hlayout = new QHBoxLayout(this);

  setupTreeViewEditorMargin(hlayout);
  m_textLabel = new QLabel("");

  QColor color(255, 255, 255);
  m_textLabel->setPalette(QPalette(color));

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

bool te::layout::FontEditor::eventFilter(QObject * watched, QEvent * event)
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

void te::layout::FontEditor::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void te::layout::FontEditor::onButtonClicked(bool checked)
{
  emit showDlg();
  getFont();
}

void te::layout::FontEditor::getFont()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, m_font, this);
  if (ok)
  {
    // font is set to the font the user selected  }
    m_font = font;
  }
  else 
  {
    // the user canceled the dialog; font is set to the initial
    return;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property prop;
  prop.setName("");
  //prop.setValue(newFont, dataType->getDataTypeFont());

  //Emit our own signal.
  emit dataValueChanged(this, prop);
}

void te::layout::FontEditor::setupTreeViewEditorMargin(QLayout* layout)
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

