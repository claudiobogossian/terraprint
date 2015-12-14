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
  \file DoubleTreeWidgetOutside.cpp

  \brief ...
*/

// TerraLib
#include "DoubleTreeWidgetOutside.h"
#include "ui_DoubleTreeWidget.h"

// Qt
#include <QIcon>
#include <QAbstractItemView>
#include <QTreeWidgetItem>
#include <QIcon>
#include <QVariant>
#include <QString>

te::layout::DoubleTreeWidgetOutside::DoubleTreeWidgetOutside(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
  m_ui(new Ui::DoubleTreeWidget)
{
  m_ui->setupUi(this);

  QStringList headerLabels;
  headerLabels.append(tr("Legend"));

  m_ui->m_leftTreeWidget->setColumnCount(1);
  m_ui->m_leftTreeWidget->setHeaderLabels(headerLabels);

  m_ui->m_rightTreeWidget->setColumnCount(1);
  m_ui->m_rightTreeWidget->setHeaderLabels(headerLabels);

// set icons
  m_ui->m_addToolButton->setIcon(QIcon::fromTheme("go-next"));
  m_ui->m_addAllToolButton->setIcon(QIcon::fromTheme("go-last"));
  m_ui->m_removeToolButton->setIcon(QIcon::fromTheme("go-previous"));
  m_ui->m_removeAllToolButton->setIcon(QIcon::fromTheme("go-first"));
  m_ui->m_upToolButton->setIcon(QIcon::fromTheme("go-up"));
  m_ui->m_downToolButton->setIcon(QIcon::fromTheme("go-down"));

// set selection mode
  m_ui->m_leftTreeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
  m_ui->m_rightTreeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

// connect signals and slots
  connect(m_ui->m_addToolButton, SIGNAL(pressed()), this, SLOT(onAddToolButtonPressed()));
  connect(m_ui->m_addAllToolButton, SIGNAL(pressed()), this, SLOT(onAddAllToolButtonPressed()));
  connect(m_ui->m_removeToolButton, SIGNAL(pressed()), this, SLOT(onRemoveToolButtonPressed()));
  connect(m_ui->m_removeAllToolButton, SIGNAL(pressed()), this, SLOT(onRemoveAllToolButtonPressed()));
  connect(m_ui->m_leftTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onLeftListItemSelectionChanged()));
  connect(m_ui->m_rightTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onRightListItemSelectionChanged()));
  connect(m_ui->m_upToolButton, SIGNAL(pressed()), this, SLOT(onUpToolButtonPressed()));
  connect(m_ui->m_downToolButton, SIGNAL(pressed()), this, SLOT(onDownToolButtonPressed()));
}

te::layout::DoubleTreeWidgetOutside::~DoubleTreeWidgetOutside()
{
}

Ui::DoubleTreeWidget* te::layout::DoubleTreeWidgetOutside::getForm() const
{
  return m_ui.get();
}

void te::layout::DoubleTreeWidgetOutside::setInputValues(std::vector<std::string> values)
{
  m_ui->m_leftTreeWidget->clear();

  if(values.empty() == false)
    m_ui->m_addAllToolButton->setEnabled(true);
  else
    m_ui->m_addAllToolButton->setEnabled(false);

  for(size_t i = 0; i < values.size(); ++i)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem(m_ui->m_leftTreeWidget);
    item->setText(0, values[i].c_str());

    m_ui->m_leftTreeWidget->addTopLevelItem(item);
  }
}

void te::layout::DoubleTreeWidgetOutside::setInputDataValues(std::vector<std::string> values, std::vector<int> ids)
{
  m_ui->m_leftTreeWidget->clear();

  if (values.size() != ids.size())
    return;

  if (values.empty() == false)
    m_ui->m_addAllToolButton->setEnabled(true);
  else
    m_ui->m_addAllToolButton->setEnabled(false);

  for (size_t i = 0; i < values.size(); ++i)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem(m_ui->m_leftTreeWidget);
    item->setText(0, values[i].c_str());
    item->setData(0, Qt::UserRole, QVariant(ids[i]));

    m_ui->m_leftTreeWidget->addTopLevelItem(item);
  }
}

void te::layout::DoubleTreeWidgetOutside::setOutputValues(std::vector<std::string> values)
{
  m_ui->m_rightTreeWidget->clear();

  if(values.empty() == false)
    m_ui->m_removeAllToolButton->setEnabled(true);
  else
    m_ui->m_removeAllToolButton->setEnabled(false);

  for(size_t i = 0; i < values.size(); ++i)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem(m_ui->m_rightTreeWidget);
    item->setText(0, values[i].c_str());

    m_ui->m_rightTreeWidget->addTopLevelItem(item);
  }
}

void te::layout::DoubleTreeWidgetOutside::setOutputDataValues(std::vector<std::string> values, std::vector<int> ids)
{
  m_ui->m_rightTreeWidget->clear();

  if (values.size() != ids.size())
    return;

  if (values.empty() == false)
    m_ui->m_removeAllToolButton->setEnabled(true);
  else
    m_ui->m_removeAllToolButton->setEnabled(false);

  for (size_t i = 0; i < values.size(); ++i)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem(m_ui->m_rightTreeWidget);
    item->setText(0, values[i].c_str());
    item->setData(0, Qt::UserRole, QVariant(ids[i]));

    m_ui->m_rightTreeWidget->addTopLevelItem(item);
  }
}

void te::layout::DoubleTreeWidgetOutside::setFixedOutputValues(std::vector<std::string> values, std::string iconName)
{
  for(size_t i = 0; i < values.size(); ++i)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem(m_ui->m_rightTreeWidget);
    item->setText(0, values[i].c_str());
    item->setFlags(Qt::ItemIsEnabled);
    item->setIcon(0, QIcon::fromTheme(iconName.c_str()));
  }
}

void te::layout::DoubleTreeWidgetOutside::setLeftLabel(std::string value)
{
  m_ui->m_leftItemsLabel->setText(value.c_str());
}

void te::layout::DoubleTreeWidgetOutside::setRightLabel(std::string value)
{
  m_ui->m_rightItemsLabel->setText(value.c_str());
}

std::vector<std::string> te::layout::DoubleTreeWidgetOutside::getOutputValues()
{
  std::vector<std::string> vec;

  int count = m_ui->m_rightTreeWidget->topLevelItemCount();

  for(int i = 0; i < count; ++i)
  {
    vec.push_back(m_ui->m_rightTreeWidget->topLevelItem(i)->text(0).toLatin1().data());
  }

  return vec;
}

std::vector<int> te::layout::DoubleTreeWidgetOutside::getOutputDataValues()
{
  std::vector<int> vec;

  int count = m_ui->m_rightTreeWidget->topLevelItemCount();

  for (int i = 0; i < count; ++i)
  {
    QVariant v = m_ui->m_rightTreeWidget->topLevelItem(i)->data(0, Qt::UserRole);

    if (v.isValid())
      vec.push_back(v.toInt());
  }

  return vec;
}

void te::layout::DoubleTreeWidgetOutside::clearInputValues()
{
  m_ui->m_leftTreeWidget->clear();

  m_ui->m_addToolButton->setEnabled(false);
  m_ui->m_addAllToolButton->setEnabled(false);
}

void te::layout::DoubleTreeWidgetOutside::clearOutputValues()
{
  m_ui->m_rightTreeWidget->clear();

  m_ui->m_removeToolButton->setEnabled(false);
  m_ui->m_removeAllToolButton->setEnabled(false);
}

void te::layout::DoubleTreeWidgetOutside::setVisibledOrderButtons(bool state)
{
  m_ui->m_upToolButton->setVisible(state);
  m_ui->m_downToolButton->setVisible(state);
}

void te::layout::DoubleTreeWidgetOutside::onAddToolButtonPressed()
{
  QList<QTreeWidgetItem*> list = m_ui->m_leftTreeWidget->selectedItems();

  if(list.empty() == false)
    m_ui->m_removeAllToolButton->setEnabled(true);

  for(int i = 0; i < list.size(); ++i)
  {
    int row = m_ui->m_leftTreeWidget->indexOfTopLevelItem(list[i]);

    QTreeWidgetItem* item = m_ui->m_leftTreeWidget->takeTopLevelItem(row);

    m_ui->m_rightTreeWidget->addTopLevelItem(item);
  }

  if (m_ui->m_leftTreeWidget->topLevelItemCount() == 0)
  {
    m_ui->m_addToolButton->setEnabled(false);
    m_ui->m_addAllToolButton->setEnabled(false);
  }

  emit itemChanged();
}

void te::layout::DoubleTreeWidgetOutside::onAddAllToolButtonPressed()
{
  m_ui->m_leftTreeWidget->selectAll();

  QList<QTreeWidgetItem*> list = m_ui->m_leftTreeWidget->selectedItems();

  if(list.empty() == false)
    m_ui->m_removeAllToolButton->setEnabled(true);

  for(int i = 0; i < list.size(); ++i)
  {
    int row = m_ui->m_leftTreeWidget->indexOfTopLevelItem(list[i]);

    QTreeWidgetItem* item = m_ui->m_leftTreeWidget->takeTopLevelItem(row);

    m_ui->m_rightTreeWidget->addTopLevelItem(item);
  }

  m_ui->m_addToolButton->setEnabled(false);
  m_ui->m_addAllToolButton->setEnabled(false);

  emit itemChanged();
}

void te::layout::DoubleTreeWidgetOutside::onRemoveToolButtonPressed()
{
  QList<QTreeWidgetItem*> list = m_ui->m_rightTreeWidget->selectedItems();

  if(list.empty() == false)
    m_ui->m_addAllToolButton->setEnabled(true);

  for(int i = 0; i < list.size(); ++i)
  {
    int row = m_ui->m_rightTreeWidget->indexOfTopLevelItem(list[i]);

    QTreeWidgetItem* item = m_ui->m_rightTreeWidget->takeTopLevelItem(row);

    m_ui->m_leftTreeWidget->addTopLevelItem(item);
  }

  if (m_ui->m_rightTreeWidget->topLevelItemCount() == 0)
  {
    m_ui->m_removeToolButton->setEnabled(false);
    m_ui->m_removeAllToolButton->setEnabled(false);
    m_ui->m_upToolButton->setEnabled(false);
    m_ui->m_downToolButton->setEnabled(false);
  }

  emit itemChanged();
}

void te::layout::DoubleTreeWidgetOutside::onRemoveAllToolButtonPressed()
{
  m_ui->m_rightTreeWidget->selectAll();

  QList<QTreeWidgetItem*> list = m_ui->m_rightTreeWidget->selectedItems();

  if(list.empty() == false)
    m_ui->m_addAllToolButton->setEnabled(true);

  for(int i = 0; i < list.size(); ++i)
  {
    int row = m_ui->m_rightTreeWidget->indexOfTopLevelItem(list[i]);

    QTreeWidgetItem* item = m_ui->m_rightTreeWidget->takeTopLevelItem(row);

    m_ui->m_leftTreeWidget->addTopLevelItem(item);
  }

  m_ui->m_removeToolButton->setEnabled(false);
  m_ui->m_removeAllToolButton->setEnabled(false);
  m_ui->m_upToolButton->setEnabled(false);
  m_ui->m_downToolButton->setEnabled(false);

  emit itemChanged();
}

void te::layout::DoubleTreeWidgetOutside::onDownToolButtonPressed()
{
  int count = m_ui->m_rightTreeWidget->topLevelItemCount();

  if (count == m_ui->m_rightTreeWidget->selectedItems().size())
    return;

  for(int i = count-1; i >= 0; --i)
  {
    QTreeWidgetItem* currentItem = m_ui->m_rightTreeWidget->topLevelItem(i);

    if(i == count-1)
      continue;

    if(currentItem->isSelected())
    {
      QTreeWidgetItem* itemAfter = m_ui->m_rightTreeWidget->topLevelItem(i + 1);

      bool wasSelected = itemAfter->isSelected();

      m_ui->m_rightTreeWidget->takeTopLevelItem(i);
      m_ui->m_rightTreeWidget->takeTopLevelItem(i);

      m_ui->m_rightTreeWidget->insertTopLevelItem(i, itemAfter);
      m_ui->m_rightTreeWidget->insertTopLevelItem(i + 1, currentItem);

      currentItem->setSelected(true);

      if(wasSelected)
        itemAfter->setSelected(true);
    }
  }
}

void te::layout::DoubleTreeWidgetOutside::onUpToolButtonPressed()
{
  int count = m_ui->m_rightTreeWidget->topLevelItemCount();

  if (count == m_ui->m_rightTreeWidget->selectedItems().size())
    return;

  for(int i = 0; i < count; ++i)
  {
    QTreeWidgetItem* currentItem = m_ui->m_rightTreeWidget->topLevelItem(i);

    if(i == 0)
      continue;

    if(currentItem->isSelected())
    {
      QTreeWidgetItem* itemBefore = m_ui->m_rightTreeWidget->topLevelItem(i - 1);

      bool wasSelected = itemBefore->isSelected();

      m_ui->m_rightTreeWidget->takeTopLevelItem(i);
      m_ui->m_rightTreeWidget->takeTopLevelItem(i - 1);

      m_ui->m_rightTreeWidget->insertTopLevelItem(i - 1, currentItem);
      m_ui->m_rightTreeWidget->insertTopLevelItem(i, itemBefore);

      currentItem->setSelected(true);

      if(wasSelected)
        itemBefore->setSelected(true);
    }
  }
}

void te::layout::DoubleTreeWidgetOutside::onLeftListItemSelectionChanged()
{
  if (m_ui->m_leftTreeWidget->selectedItems().size() > 0)
  {
    m_ui->m_addToolButton->setEnabled(true);
    m_ui->m_addAllToolButton->setEnabled(true);
  }
  else
  {
    m_ui->m_addToolButton->setEnabled(false);
  }
}

void te::layout::DoubleTreeWidgetOutside::onRightListItemSelectionChanged()
{
  if (m_ui->m_rightTreeWidget->selectedItems().size() > 0)
  {
    m_ui->m_removeToolButton->setEnabled(true);
    m_ui->m_removeAllToolButton->setEnabled(true);
    m_ui->m_upToolButton->setEnabled(true);
    m_ui->m_downToolButton->setEnabled(true);
  }
  else
  {
    m_ui->m_removeToolButton->setEnabled(false);
    m_ui->m_upToolButton->setEnabled(false);
    m_ui->m_downToolButton->setEnabled(false);
  }
}

