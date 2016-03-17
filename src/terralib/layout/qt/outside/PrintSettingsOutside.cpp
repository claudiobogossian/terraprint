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
  \file terralib/layout/qt/outside/PrintSettingsOutside.cpp

  \brief A dialog configure the input layer to address geocoding operation
*/

// TerraLib
#include "PrintSettingsOutside.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../core/ItemUtils.h"
#include "../core/Scene.h"
#include "../core/View.h"
#include "ui_PrintSettings.h"

// STL
#include <iostream>

// Qt
#include <QMessageBox>
#include <QPrinter>
#include <QPrinterInfo>
#include <QStringList>
#include <QListWidgetItem>
#include <QIcon>
#include <QList>

te::layout::PrintSettingsOutside::PrintSettingsOutside(Scene* scene, AbstractOutsideController* controller, QWidget* parent)
  : QDialog(parent),
  AbstractOutsideView(controller),
  m_scene(scene),
  m_printer(0),
  m_ui(new Ui::PrintSettings)
{
  m_ui->setupUi(this);

  m_dpiList.append("150");
  m_dpiList.append("300");
  m_dpiList.append("600");

  init();
}

te::layout::PrintSettingsOutside::~PrintSettingsOutside()
{
  
}

void te::layout::PrintSettingsOutside::init()
{
  m_ui->rdoDraft->setChecked(false);
  m_ui->rdoNormal->setChecked(true);
  m_ui->rdoFineDetail->setChecked(false);

  QString dpi = m_dpiList[1];

  m_ui->lblNumberDPI->clear();
  m_ui->lblNumberDPI->setText(dpi);

  m_ui->lstPrinter->setMovement(QListView::Static);

  QString nameIcon("layout-printer");
  QIcon icon = QIcon::fromTheme(nameIcon);

  QPrinterInfo defaultPrinter = QPrinterInfo::defaultPrinter();

  QList<QPrinterInfo> list = QPrinterInfo::availablePrinters();
  QStringList nameList;
  int row = 0;
  foreach(QPrinterInfo info, list)
  {
    QString name = info.printerName();
    QListWidgetItem *newItem = new QListWidgetItem(icon, name);
    m_ui->lstPrinter->insertItem(row, newItem);
  }

  QString defaultPrinterName = defaultPrinter.printerName();
  QList<QListWidgetItem*> listFind = m_ui->lstPrinter->findItems(defaultPrinterName, Qt::MatchExactly);

  if (!listFind.isEmpty())
  {
    QListWidgetItem* defaultItem = listFind.first();
    m_ui->lstPrinter->setCurrentItem(defaultItem);
  }
}

void te::layout::PrintSettingsOutside::on_pBtnOK_clicked()
{
  accept();
}

void te::layout::PrintSettingsOutside::on_pBtnCancel_clicked()
{
  reject();
}

void te::layout::PrintSettingsOutside::on_helpPushButton_clicked()
{

}

void te::layout::PrintSettingsOutside::on_lstPrinter_currentItemChanged(QListWidgetItem * current, QListWidgetItem * previous)
{
  if (!m_scene || !m_printer)
    return;
  
  QList<QPrinterInfo> list = QPrinterInfo::availablePrinters();
  QStringList nameList;
  foreach(QPrinterInfo info, list)
  {
    QString name = info.printerName();
    QString currentText = current->text();
    if (name.compare(currentText) == 0)
    {
      m_printer->setPrinterName(name);
    }
  }
}

void te::layout::PrintSettingsOutside::on_rdoDraft_clicked()
{
  m_ui->rdoDraft->setChecked(true);
  m_ui->rdoNormal->setChecked(false);
  m_ui->rdoFineDetail->setChecked(false);

  QString dpi = m_dpiList[0];
  m_ui->lblNumberDPI->setText(dpi);

  int dpiNumber = dpi.toInt();
  m_printer->setResolution(dpiNumber);
}

void te::layout::PrintSettingsOutside::on_rdoNormal_clicked()
{
  m_ui->rdoDraft->setChecked(false);
  m_ui->rdoNormal->setChecked(true);
  m_ui->rdoFineDetail->setChecked(false);

  QString dpi = m_dpiList[1];
  m_ui->lblNumberDPI->setText(dpi);

  int dpiNumber = dpi.toInt();
  m_printer->setResolution(dpiNumber);
}

void te::layout::PrintSettingsOutside::on_rdoFineDetail_clicked()
{
  m_ui->rdoDraft->setChecked(false);
  m_ui->rdoNormal->setChecked(false);
  m_ui->rdoFineDetail->setChecked(true);

  QString dpi = m_dpiList[2];
  m_ui->lblNumberDPI->setText(dpi);

  int dpiNumber = dpi.toInt();
  m_printer->setResolution(dpiNumber);
}

QString te::layout::PrintSettingsOutside::getPrinterName()
{
  QString name;// = m_ui->txtPrinterName->text();
  return name;
}

int te::layout::PrintSettingsOutside::getDPI()
{
  QString txtDPI = m_ui->lblNumberDPI->text();
  return txtDPI.toInt();
}

void te::layout::PrintSettingsOutside::setPosition(const double& x, const double& y)
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::PrintSettingsOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::PrintSettingsOutside::setPrinter(QPrinter* printer)
{
  m_printer = printer;
  if (printer)
  {
    QString qDpi = m_ui->lblNumberDPI->text();
    int dpi = qDpi.toInt();
    m_printer->setResolution(dpi);
  }
}

