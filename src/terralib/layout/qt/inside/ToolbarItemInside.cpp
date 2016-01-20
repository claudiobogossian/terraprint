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
  \file ToolbarItemInside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ToolbarItemInside.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/property/Properties.h"

#include <QToolBar>
#include <QToolButton>
#include <QPushButton>
#include <QAction>
#include <QToolBar>
#include <QWidget>

te::layout::ToolbarItemInside::ToolbarItemInside() :
    m_item(0)
  , m_toolbar(0)
{

}

te::layout::ToolbarItemInside::~ToolbarItemInside()
{
  if (!m_toolbar)
  {
    delete m_toolbar;
    m_toolbar = 0;
  }
}

te::layout::EnumType* te::layout::ToolbarItemInside::getItemType()
{
  EnumType* itemType = 0;
  if (!m_item)
    return itemType;

  itemType = m_item->getController()->getProperties().getTypeObj();
  return itemType;
}

te::layout::AbstractItemView* te::layout::ToolbarItemInside::getItem()
{
  return m_item;
}

void te::layout::ToolbarItemInside::setItem(AbstractItemView* item)
{
  m_item = item;
}

QToolBar* te::layout::ToolbarItemInside::getToolbar()
{
  return m_toolbar;
}

QToolButton* te::layout::ToolbarItemInside::createToolButton(const QString& text, const QString& tooltip, const QString& icon)
{
  QToolButton *btn = 0;

  if (!m_toolbar)
    return btn;

  btn = new QToolButton(m_toolbar);
  btn->setText(text);
  btn->setGeometry(0, 0, 10, 10);
  btn->setCheckable(true);
  btn->setToolTip(tooltip);

  if (!icon.isEmpty())
    btn->setIcon(QIcon::fromTheme(icon));

  return btn;
}

QPushButton* te::layout::ToolbarItemInside::createPushButton(const QString& text, const QString& tooltip, const QString& icon)
{
  QPushButton *btn = 0;

  if (!m_toolbar)
    return btn;

  btn = new QPushButton(m_toolbar);
  btn->setText(text);
  btn->setGeometry(0, 0, 10, 10);
  btn->setCheckable(true);
  btn->setToolTip(tooltip);

  btn->setIcon(QIcon::fromTheme(icon));

  return btn;
}

QAction* te::layout::ToolbarItemInside::createAction(const QString& text, const QString& objName, const QString& icon, const QString& tooltip, QWidget* parent)
{
  QAction *actionMenu = 0;

  if (!m_toolbar)
    return actionMenu;

  QWidget* parentToUse = parent;
  if (parentToUse == 0)
  {
    parentToUse = m_toolbar;
  }

  actionMenu = new QAction(text, parentToUse);
  actionMenu->setObjectName(objName);

  actionMenu->setIcon(QIcon::fromTheme(icon));
  actionMenu->setToolTip(tooltip);

  return actionMenu;
}

