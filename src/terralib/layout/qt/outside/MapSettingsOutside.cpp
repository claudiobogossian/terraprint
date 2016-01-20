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
  \file terralib/layout/qt/outside/MapSettingsOutside.cpp

  \brief A dialog configure the input layer to address geocoding operation
*/

// TerraLib
#include "MapSettingsOutside.h"
#include "MapLayerChoiceOutside.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "ui_MapSettings.h"
#include "../item/MapController.h"

// Qt
#include <QMessageBox>
#include <QString>
#include <QFileDialog>

te::layout::MapSettingsOutside::MapSettingsOutside(AbstractOutsideController* controller)
  : QDialog(0),
  AbstractOutsideView(controller),
  m_ui(new Ui::MapSettings),
  m_widget(0)
{
  m_ui->setupUi(this);

  m_widget.reset(new te::layout::MapLayerChoiceOutside(controller));

  QGridLayout* layout = new QGridLayout(m_ui->m_widget_MapChoice);
  layout->addWidget(m_widget.get());

  setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint
    | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);
  
  init();
}

te::layout::MapSettingsOutside::~MapSettingsOutside()
{
  
}

void te::layout::MapSettingsOutside::init()
{
  if (m_ui->pBtnOK && m_ui->pBtnCancel)
  {
    connect(m_ui->pBtnOK, SIGNAL(pressed()), this, SLOT(onOkPushButtonPressed()));
    connect(m_ui->pBtnCancel, SIGNAL(pressed()), this, SLOT(onCancelPushButtonPressed()));

  }

  initDouble(m_ui->lneHeight, "height");
  initDouble(m_ui->lneWidth, "width");


 /* m_ui->lblDPIInformation->clear();
  m_ui->lblDPIInformation->setText("DPI: 150");

  m_ui->txtEdtFile->setEnabled(false);
  
  if (m_ui->pBtnOK && m_ui->pBtnCancel && m_ui->pBtnSaveAs)
  {
    connect(m_ui->pBtnOK, SIGNAL(pressed()), this, SLOT(onOkPushButtonPressed()));
    connect(m_ui->pBtnCancel, SIGNAL(pressed()), this, SLOT(onCancelPushButtonPressed()));
    connect(m_ui->pBtnSaveAs, SIGNAL(clicked()), this, SLOT(onSaveAsClicked()));
    connect(m_ui->cmbDPI, SIGNAL(currentIndexChanged(const QString)), this, SLOT(onCurrentIndexChanged(const QString)));
  }

  QStringList list;
  list.append("150");
  list.append("300");
  list.append("600");
  list.append("1200");

  m_ui->cmbDPI->addItems(list);*/
}

void te::layout::MapSettingsOutside::onOkPushButtonPressed()
{
  if (!m_ui->lneWidth->text().isEmpty() && !m_ui->lneHeight->text().isEmpty())
  {
    accept();
    return;
  }
  QMessageBox::information(this, tr("Map Settings"), tr("Please set Width and Height."));
}

void te::layout::MapSettingsOutside::onCancelPushButtonPressed()
{
  reject();
}

void te::layout::MapSettingsOutside::onSaveAsClicked()
{
  /*QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"), QDir::currentPath(), tr("PDF Files (*.pdf)"));

  if (!fileName.isEmpty())
  {
    m_ui->txtEdtFile->setText(fileName);
  }
  if (fileName.endsWith(".pdf") == false)
  {
    fileName.append(".pdf");
  }*/
}

void te::layout::MapSettingsOutside::onCurrentIndexChanged(const QString & text)
{
  /*QPalette palette = m_ui->lblDPIInformation->palette();
  QString txtCurrentDPI = m_ui->cmbDPI->currentText();
  int dpi = txtCurrentDPI.toInt();
  if (dpi > 300)
  {
    //red text
    QBrush brushRed(QColor(255, 0, 0, 255));
    brushRed.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brushRed);

    m_ui->lblDPIInformation->clear();
    if (dpi < 900)
    {
      m_ui->lblDPIInformation->setText(tr("The DPI chosen is high. Can cause very large files."));
    }
    else
    {
      m_ui->lblDPIInformation->setText(tr("The DPI chosen is too high. Can cause very large files."));
    }
  }
  else
  {
    //black text
    QBrush brushBlack(QColor(0, 0, 0, 255));
    brushBlack.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brushBlack);

    QString txtDPI = "DPI: " + txtCurrentDPI;
    m_ui->lblDPIInformation->clear();
    m_ui->lblDPIInformation->setText(txtDPI);
  }
  m_ui->lblDPIInformation->setPalette(palette);
  */

}

void te::layout::MapSettingsOutside::setPosition(const double& x, const double& y)
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::MapSettingsOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

std::string te::layout::MapSettingsOutside::getFilePath()
{
  QString qTxt;// = m_ui->txtEdtFile->text();
 /* if (!qTxt.isEmpty())
  {
    if (!qTxt.endsWith(".pdf", Qt::CaseInsensitive))
    {
      qTxt += ".pdf";
      m_ui->txtEdtFile->clear();
      m_ui->txtEdtFile->setText(qTxt);
    }
  }*/
  return qTxt.toStdString();
}

int te::layout::MapSettingsOutside::getDPI()
{
  QString txtDPI;// = m_ui->cmbDPI->currentText();
  return txtDPI.toInt();
}

void te::layout::MapSettingsOutside::setCurrentDPI(int dpi)
{
 /* QString txt = QString::number(dpi);
  int index = m_ui->cmbDPI->findText(txt);
  m_ui->cmbDPI->setCurrentIndex(index);*/
}


void te::layout::MapSettingsOutside::initDouble(QWidget* widget, std::string nameComponent)
{
  MapController* controller = dynamic_cast<MapController*>(m_controller);
  if (!controller)
    return;

  std::ostringstream convert;
  convert.precision(15);
  Property prop = controller->getProperty(nameComponent);
  double number = prop.getValue().toDouble();
  convert << number;

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if (edit)
  {
    edit->setText(convert.str().c_str());
  }
}