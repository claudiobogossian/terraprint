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
  \file MapSettingsEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapSettingsEditor.h"
#include "../../../../core/enum/Enums.h"
#include "../../../../core/property/Property.h"
#include "../../../outside/MapSettingsOutside.h"
#include "../../BuildGraphicsOutside.h"

// STL
#include <string>

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
#include <QColorDialog>

te::layout::MapSettingsEditor::MapSettingsEditor(const QModelIndex& index, QWidget* parent) :
  QWidget(parent),
  AbstractEditor(index, Enums::getInstance().getEnumDataType()->getDataTypeInt()),
  m_textLabel(0),
  m_button(0),
  m_mapSettings(0)
{
  createGroupBox();
  changeEditorData(index);
}

te::layout::MapSettingsEditor::~MapSettingsEditor()
{
  if (m_mapSettings)
  {
    m_mapSettings->setParent(0);
    delete m_mapSettings;
  }
}

QVariant te::layout::MapSettingsEditor::getValue()
{
  return m_textLabel->text();
}

void te::layout::MapSettingsEditor::changeEditorData(const QModelIndex& index)
{
  int propertyType = qMetaTypeId<te::layout::Property>();
  QVariant variant = index.data(propertyType);
  if (variant.isValid() && !variant.isNull())
  {
    m_property = qvariant_cast<te::layout::Property>(variant);
    std::string newValue = te::layout::Property::GetValueAs<std::string>(m_property);

    if (m_textLabel)
    {
      QString name;
      m_textLabel->setText(name);
    }
  }
}

void te::layout::MapSettingsEditor::createGroupBox()
{
  QHBoxLayout* hlayout = new QHBoxLayout(this);

  setupTreeViewEditorMargin(hlayout);
  m_textLabel = new QLabel("");

  m_textLabel->setFixedWidth(20);

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

bool te::layout::MapSettingsEditor::eventFilter(QObject * watched, QEvent * event)
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

void te::layout::MapSettingsEditor::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void te::layout::MapSettingsEditor::onButtonClicked(bool checked)
{
  emit showDlg();
  showSettings();
}

void te::layout::MapSettingsEditor::showSettings()
{
  EnumType* type = Enums::getInstance().getEnumObjectType()->getMapSettingsDialog();

  BuildGraphicsOutside build;
  if (!m_mapSettings)
  {
    QWidget* widget = build.createOutside(type, 0, (QWidget*) this->parent());
    m_mapSettings = dynamic_cast<MapSettingsOutside*>(widget);
  }

  m_mapSettings->load();
  m_mapSettings->show(); // modeless dialog
  m_mapSettings->raise(); // top of the parent widget's stack
}

void te::layout::MapSettingsEditor::setupTreeViewEditorMargin(QLayout* layout)
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

void te::layout::MapSettingsEditor::onUpdateProperty(Property prop)
{
  if (!prop.isNull())
  {
    // Emit our own signal.
    emit dataValueChanged(this, m_property);
  }
}
