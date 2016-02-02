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
  \file PrintScene.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PrintPreviewDialog.h"

// STL
#include <sstream>

// Qt
#include <QPrinter>
#include <QPrintPreviewWidget>
#include <QToolBar>
#include <QAction>
#include <QSize>

#include <QPrintDialog>

te::layout::PrintPreviewDialog::PrintPreviewDialog(QPrinter * printer, QWidget * parent, Qt::WindowFlags flags) :
  QPrintPreviewDialog(printer, parent, flags)
{
  QSize dialogSize(640, 480);
  setMinimumSize(dialogSize);
  configureDialog();
}

te::layout::PrintPreviewDialog::~PrintPreviewDialog()
{

}

void te::layout::PrintPreviewDialog::configureDialog()
{
  invisibleAllButtons();
  visibleSomeButtons();
}

void te::layout::PrintPreviewDialog::disableAllButtons()
{
  QList<QToolBar *> toolbarlist = findChildren<QToolBar *>();
  foreach(QToolBar* tool, toolbarlist)
  {
    QList<QAction *> actionlist = tool->actions();
    foreach(QAction* currentAction, actionlist)
    {
      currentAction->setDisabled(true);
    }    
  }
}

void te::layout::PrintPreviewDialog::invisibleAllButtons()
{
  QList<QToolBar *> toolbarlist = findChildren<QToolBar *>();
  foreach(QToolBar* tool, toolbarlist)
  {
    QList<QAction *> actionlist = tool->actions();
    foreach(QAction* currentAction, actionlist)
    {
      currentAction->setVisible(false);
    }
  }
}

void te::layout::PrintPreviewDialog::enableSomeButtons()
{
  QList<QToolBar *> toolbarlist = findChildren<QToolBar *>();
  if (toolbarlist.isEmpty())
  {
    return;
  }

  if (toolbarlist.first()->actions().count() < 22)
  {
    return;
  }
  
  toolbarlist.first()->actions().at(0)->setEnabled(true);
  toolbarlist.first()->actions().at(1)->setEnabled(true);
  toolbarlist.first()->actions().at(2)->setEnabled(true); 
  toolbarlist.first()->actions().at(3)->setEnabled(true);
  toolbarlist.first()->actions().at(4)->setEnabled(true);
  toolbarlist.first()->actions().at(5)->setEnabled(true);
  toolbarlist.first()->actions().at(21)->setEnabled(true);
}

void te::layout::PrintPreviewDialog::visibleSomeButtons()
{
  QList<QToolBar *> toolbarlist = findChildren<QToolBar *>();
  if (toolbarlist.isEmpty())
  {
    return;
  }

  if (toolbarlist.first()->actions().count() < 22)
  {
    return;
  }

  toolbarlist.first()->actions().at(0)->setVisible(true);
  toolbarlist.first()->actions().at(1)->setVisible(true);
  toolbarlist.first()->actions().at(2)->setVisible(true);  
  toolbarlist.first()->actions().at(3)->setVisible(true);
  toolbarlist.first()->actions().at(4)->setVisible(true);
  toolbarlist.first()->actions().at(5)->setVisible(true);
  toolbarlist.first()->actions().at(21)->setVisible(true);
}
