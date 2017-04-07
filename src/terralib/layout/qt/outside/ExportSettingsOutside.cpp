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
  \file terralib/layout/qt/outside/SVGDialogOutside.cpp

  \brief A dialog configure the input layer to address geocoding operation
*/

// TerraLib
#include "ExportSettingsOutside.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../core/ItemUtils.h"
#include "ui_ExportSettings.h"

#include <terralib/qt/widgets/Utils.h>

// Qt
#include <QMessageBox>
#include <QString>
#include <QFileDialog>

te::layout::ExportSettingsOutside::ExportSettingsOutside(AbstractOutsideController* controller, QWidget* parent)
  : QDialog(parent),
  AbstractOutsideView(controller),
  m_ui(new Ui::ExportSettings)
{
  m_ui->setupUi(this);

  init();
}

te::layout::ExportSettingsOutside::~ExportSettingsOutside()
{
  
}

void te::layout::ExportSettingsOutside::init()
{
  m_ui->lblDPIInformation->clear();
  m_ui->lblDPIInformation->setText("DPI: 150");
  m_ui->lblDPIInformation->setWordWrap(true); // multiple lines, label text is wrapped where necessary at word-breaks

  m_ui->txtEdtFile->setEnabled(false);
  m_ui->cmbDPI->setEditable(false);
  m_ui->cmbFileFormat->setEditable(false);
  
  if (m_ui->pBtnOK && m_ui->pBtnCancel && m_ui->pBtnSaveAs)
  {
    connect(m_ui->pBtnOK, SIGNAL(clicked()), this, SLOT(onOkPushButtonPressed()));
    connect(m_ui->pBtnCancel, SIGNAL(clicked()), this, SLOT(onCancelPushButtonPressed()));
    connect(m_ui->pBtnSaveAs, SIGNAL(clicked()), this, SLOT(onSaveAsClicked()));
    connect(m_ui->cmbDPI, SIGNAL(currentIndexChanged(const QString)), this, SLOT(onCurrentDPIIndexChanged(const QString)));
    connect(m_ui->cmbFileFormat, SIGNAL(currentIndexChanged(const QString)), this, SLOT(onCurrentFileFormatIndexChanged(const QString)));
  }

  QStringList listDPI;
  listDPI.append("150");
  listDPI.append("300");
  listDPI.append("600");
  listDPI.append("1200");

  m_ui->cmbDPI->addItems(listDPI);

  QStringList listFileFormat;
  listFileFormat.append("JPEG");
  listFileFormat.append("PNG");
  listFileFormat.append("SVG");
  listFileFormat.append("PDF");

  m_ui->cmbFileFormat->addItems(listFileFormat);

  m_ui->cmbDPI->blockSignals(true);
  m_ui->cmbFileFormat->blockSignals(true);

  m_ui->cmbDPI->setCurrentIndex(0);
  m_ui->cmbFileFormat->setCurrentIndex(0);

  m_ui->cmbDPI->blockSignals(false);
  m_ui->cmbFileFormat->blockSignals(false);

}

void te::layout::ExportSettingsOutside::onOkPushButtonPressed()
{
  if (!m_ui->txtEdtFile->text().isEmpty())
  {
    accept();
    return;
  }
  QMessageBox::information(this, tr("Export Settings"), tr("Please complete the filename."));
}

void te::layout::ExportSettingsOutside::onCancelPushButtonPressed()
{
  reject();
}

void te::layout::ExportSettingsOutside::onSaveAsClicked()
{
  QString lowercaseCurrentFileFormat = currentFileFormat().toLower();
  QString currentFileFormatExtension = "*." + lowercaseCurrentFileFormat;
  QString concatenatedFileFormat = tr("Files") + " (*." + lowercaseCurrentFileFormat + ")";

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), te::qt::widgets::GetFilePathFromSettings(lowercaseCurrentFileFormat), concatenatedFileFormat);

  if (!fileName.isEmpty())
  {
    m_ui->txtEdtFile->setText(fileName);
  }
  if (fileName.endsWith(currentFileFormatExtension) == false)
  {
    fileName.append(currentFileFormatExtension);
  }

  QFileInfo fileInfo(fileName);
  te::qt::widgets::AddFilePathToSettings(fileInfo.absolutePath(), lowercaseCurrentFileFormat);
}

void te::layout::ExportSettingsOutside::onCurrentDPIIndexChanged(const QString & text)
{
  QPalette palette = m_ui->lblDPIInformation->palette();
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
}

void te::layout::ExportSettingsOutside::onCurrentFileFormatIndexChanged(const QString & text)
{

}
void te::layout::ExportSettingsOutside::setPosition(const double& x, const double& y)
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::ExportSettingsOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

std::string te::layout::ExportSettingsOutside::getFilePath()
{
  QString lowercaseCurrentFileFormat = currentFileFormat().toLower();
  QString qTxt = m_ui->txtEdtFile->text();
  if (!qTxt.isEmpty())
  {
    QString fileFormat = "." + lowercaseCurrentFileFormat;
    if (!qTxt.endsWith(fileFormat, Qt::CaseInsensitive))
    {
      qTxt += fileFormat;
      m_ui->txtEdtFile->clear();
      m_ui->txtEdtFile->setText(qTxt);
    }
  }
  std::string txt = ItemUtils::convert2StdString(qTxt);
  return txt;
}

int te::layout::ExportSettingsOutside::getDPI()
{
  QString txtDPI = m_ui->cmbDPI->currentText();
  return txtDPI.toInt();
}

void te::layout::ExportSettingsOutside::setCurrentDPI(int dpi)
{
  QString txt = QString::number(dpi);
  int index = m_ui->cmbDPI->findText(txt);
  m_ui->cmbDPI->setCurrentIndex(index);
}

void te::layout::ExportSettingsOutside::setFixedFileFormat(const std::string& fileFormat)
{
  QString txt = ItemUtils::convert2QString(fileFormat);
  int index = m_ui->cmbFileFormat->findText(txt);
  if (index != -1)
  {
    m_fixedFileFormat = fileFormat;
    m_ui->cmbFileFormat->setCurrentIndex(index);
    m_ui->cmbFileFormat->setEnabled(false);
    return;
  }
  m_ui->cmbFileFormat->setEnabled(true);
}

bool te::layout::ExportSettingsOutside::hasFixedFileFormat()
{
  bool result = true;

  if (m_fixedFileFormat.empty() || m_fixedFileFormat.compare("") == 0)
  {
    result = false;
  }

  return result;
}

QString te::layout::ExportSettingsOutside::currentFileFormat()
{
  return m_ui->cmbFileFormat->currentText();
}
