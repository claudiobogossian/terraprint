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
  \file DialogItemToolbar.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "DialogItemToolbar.h"

// Qt
#include <QKeyEvent>
#include <QApplication>

te::layout::DialogItemToolbar::DialogItemToolbar(QWidget* parent) :
  QDialog(parent)
{
  setVisible(false);
  setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);
  Qt::WindowFlags flags = windowFlags();

  // Linux: for remove buttons in title bar
  flags &= ~Qt::WindowContextHelpButtonHint;
  flags &= ~Qt::WindowMinMaxButtonsHint;
  flags &= ~Qt::WindowCloseButtonHint;
  setWindowFlags(flags);
}

te::layout::DialogItemToolbar::~DialogItemToolbar()
{
 
}

void te::layout::DialogItemToolbar::keyPressEvent(QKeyEvent* keyEvent)
{
  // By default the "escape" key hides the window
  if (keyEvent->key() != Qt::Key_Escape)
  {
    QDialog::keyPressEvent(keyEvent);
  }

  if (parentWidget())
  {
    parentWidget();
    QKeyEvent keyPressEvent(QEvent::KeyPress, keyEvent->key(), keyEvent->modifiers(), keyEvent->text());
    QApplication::sendEvent(parentWidget(), &keyPressEvent);
    keyEvent->setAccepted(keyPressEvent.isAccepted());
  }
}

