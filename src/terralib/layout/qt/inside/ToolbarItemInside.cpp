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

QToolButton* te::layout::ToolbarItemInside::createToolButton(std::string text, std::string tooltip, std::string icon)
{
  QToolButton *btn = 0;

  if (!m_toolbar)
    return btn;

  btn = new QToolButton(m_toolbar);
  btn->setText(text.c_str());
  btn->setGeometry(0, 0, 10, 10);
  btn->setCheckable(true);
  btn->setToolTip(tooltip.c_str());

  if (!icon.empty())
    btn->setIcon(QIcon::fromTheme(icon.c_str()));

  return btn;
}

QPushButton* te::layout::ToolbarItemInside::createPushButton(std::string text, std::string tooltip, std::string icon)
{
  QPushButton *btn = 0;

  if (!m_toolbar)
    return btn;

  btn = new QPushButton(m_toolbar);
  btn->setText(text.c_str());
  btn->setGeometry(0, 0, 10, 10);
  btn->setCheckable(true);
  btn->setToolTip(tooltip.c_str());

  btn->setIcon(QIcon::fromTheme(icon.c_str()));

  return btn;
}

QAction* te::layout::ToolbarItemInside::createAction(std::string text, std::string objName, std::string icon, std::string tooltip, QWidget* parent)
{
  QAction *actionMenu = 0;

  if (!m_toolbar)
    return actionMenu;

  QWidget* parentToUse = parent;
  if (parentToUse == 0)
  {
    parentToUse = m_toolbar;
  }

  actionMenu = new QAction(text.c_str(), parentToUse);
  actionMenu->setObjectName(objName.c_str());

  actionMenu->setIcon(QIcon::fromTheme(icon.c_str()));
  actionMenu->setToolTip(tooltip.c_str());

  return actionMenu;
}

