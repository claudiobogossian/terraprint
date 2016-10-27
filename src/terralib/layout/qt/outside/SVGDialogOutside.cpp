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
#include "SVGDialogOutside.h"
#include "terralib/common/Logger.h"
#include "terralib/qt/widgets/utils/DoubleListWidget.h"
#include "../core/ItemUtils.h"
#include "../../core/enum/Enums.h"
#include "../../core/property/GenericVariant.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../../outside/SVGDialogController.h"
#include "../../outside/SVGDialogModel.h"
#include "ui_SVGView.h"

// STL
#include <algorithm>

// Qt
#include <QGridLayout>
#include <QMessageBox>
#include <QString>
#include <QImage>
#include <QPainter>
#include <QIcon>
#include <QDir>
#include <QtSvg/QSvgRenderer>
#include <QListWidgetItem>
#include <QFileDialog>

te::layout::SVGDialogOutside::SVGDialogOutside(AbstractOutsideController* controller, QWidget* parent)
  : QDialog(parent),
    AbstractOutsideView(controller),
    m_ui(new Ui::SVGView), 
    m_property(0),
    m_currentFile("")
{
  m_ui->setupUi(this);

  connect(m_ui->OK, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->CANCEL, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
}

te::layout::SVGDialogOutside::~SVGDialogOutside()
{
}

void te::layout::SVGDialogOutside::init()
{
  SVGDialogController* controller = dynamic_cast<SVGDialogController*>(m_controller);
  if (!controller)
    return;

  Property pWidth = controller->getProperty("width");
  Property pHeight = controller->getProperty("height");

  m_svgWidth = pWidth.getValue().toDouble();
  m_svgHeight = pHeight.getValue().toDouble();

  Property pX = controller->getProperty("x");
  Property pY = controller->getProperty("y");

  m_initFile = "";
  loadSvgImages();

}

void te::layout::SVGDialogOutside::onOkPushButtonClicked()
{


  SVGDialogController* controller = dynamic_cast<SVGDialogController*>(m_controller);
  if (!controller)
    return;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  QListWidgetItem * item = m_ui->m_listWidget->currentItem();

  if (item == 0)
  {
    reject();
    return;
  }

  QString txt = item->text();

  Property propDir = controller->getProperty("file_dir");

  std::string selectedFile = propDir.getValue().toString() + "\\" + ItemUtils::convert2StdString(txt);

  if (m_currentFile == selectedFile){
    reject();
    return;
  }

  Variant variant;
  variant.setValue(selectedFile, dataType->getDataTypeString());

  Property propFileName = controller->getProperty("file_name");
  propFileName.setValue(variant);


  Property pWidth = controller->getProperty("width");
  Property pHeight = controller->getProperty("height");

  pWidth.setValue(m_svgWidth, dataType->getDataTypeDouble());
  pHeight.setValue(m_svgHeight, dataType->getDataTypeDouble());


  Property pX = controller->getProperty("x");
  Property pY = controller->getProperty("y");

  m_currentFile = selectedFile;

  emit updateProperty(pWidth);
  emit updateProperty(pHeight);

  emit updateProperty(propFileName);

  reject();
  
}

void te::layout::SVGDialogOutside::onCancelPushButtonClicked()
{
  reject();
}

void te::layout::SVGDialogOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::SVGDialogOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::SVGDialogOutside::setPathsProperty( Property prop )
{
  m_property = prop;
}

void te::layout::SVGDialogOutside::on_m_listWidget_itemDoubleClicked(QListWidgetItem * item)
{

  SVGDialogController* controller = dynamic_cast<SVGDialogController*>(m_controller);
  if (!controller)
    return;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  QString txt = item->text();

  Property propDir = controller->getProperty("file_dir");

  std::string selectedFile = propDir.getValue().toString() + "\\" + ItemUtils::convert2StdString(txt);

  if (m_currentFile == selectedFile){
    return;
  }

  Variant variant;
  variant.setValue(selectedFile, dataType->getDataTypeString());

  Property propFileName = controller->getProperty("file_name");
  propFileName.setValue(variant);


  Property pWidth = controller->getProperty("width");
  Property pHeight = controller->getProperty("height");

  pWidth.setValue(m_svgWidth, dataType->getDataTypeDouble());
  pHeight.setValue(m_svgHeight, dataType->getDataTypeDouble());


  Property pX = controller->getProperty("x");
  Property pY = controller->getProperty("y");

  m_currentFile = selectedFile;

  emit updateProperty(pWidth);
  emit updateProperty(pHeight);

  emit updateProperty(propFileName);

}

void te::layout::SVGDialogOutside::on_btnSelectPath_clicked()
{

  SVGDialogController* controller = dynamic_cast<SVGDialogController*>(m_controller);
  if (!controller)
    return;

  QString dir = QFileDialog::getExistingDirectory(0, tr("Open Directory"),
    "/home",
    QFileDialog::ShowDirsOnly
    | QFileDialog::DontResolveSymlinks);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  std::string svgDir = ItemUtils::convert2StdString(dir);

  QString labelPath = ItemUtils::convert2QString(svgDir);
  m_ui->lblPath->setText(labelPath);


  Property prop = controller->getProperty("file_dir");
  prop.setValue(svgDir, dataType->getDataTypePath());

  emit updateProperty(prop);

  loadSvgImages();

}

void te::layout::SVGDialogOutside::loadSvgImages()
{

  SVGDialogController* controller = dynamic_cast<SVGDialogController*>(m_controller);
  if (!controller)
    return;

  m_ui->m_listWidget->clear();

  Property propDir = controller->getProperty("file_dir");
  QString labelPath = ItemUtils::convert2QString(propDir.getValue().toString());
  if (labelPath.isEmpty()){
    return;
  }
  m_ui->lblPath->setText(labelPath);

  QString directory = ItemUtils::convert2QString(propDir.getValue().toString());
  QDir svgDir(directory);
  svgDir.setNameFilters(QStringList() << "*.svg");
  QStringList fileList = svgDir.entryList();

  for (int i = 0; i < fileList.size(); i++)
  {
    QString fDir = fileList.at(i);

    QString file(directory + "\\" + fDir);

    QSvgRenderer svgRenderer(file);

    QPixmap svgPixmap(svgRenderer.defaultSize());
    svgPixmap.fill();
    QPainter pixPainter(&svgPixmap);
    svgRenderer.render(&pixPainter);

    QIcon ico(svgPixmap);

    QListWidgetItem *item = new QListWidgetItem(ico, fDir);

    QSize size(90, 90);
    m_ui->m_listWidget->setIconSize(size);

    m_ui->m_listWidget->insertItem(i, item);
  }


}