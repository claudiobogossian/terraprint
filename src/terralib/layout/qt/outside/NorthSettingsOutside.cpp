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
#include "NorthSettingsOutside.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "ui_NorthSettings.h"
#include "../../outside/NorthSettingsModel.h"
#include "../../outside/NorthSettingsController.h"
#include "../../core/property/NorthSettingsConfigProperties.h"
#include "../../core/property/Variant.h"
#include "../../core/enum/Enums.h"
// Qt
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QColorDialog>

te::layout::NorthSettingsOutside::NorthSettingsOutside(AbstractOutsideController* controller)
  : QDialog(0),
  AbstractOutsideView(controller),
  m_ui(new Ui::NorthSettings)
{
  m_ui->setupUi(this);

  setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint
    | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);
  
  init();
}

te::layout::NorthSettingsOutside::~NorthSettingsOutside()
{
  
}

void te::layout::NorthSettingsOutside::init()
{
    m_ui->colorNorth->setAutoFillBackground(true);
    m_ui->cbNorth->hide();
    //ui->doubleSpinBoxNorthHeight->setValue(true);
    //ui->doubleSpinBoxNorthWidth->setValue(false);

}

te::color::RGBAColor te::layout::NorthSettingsOutside::configColor(QWidget* widget)
{
    te::color::RGBAColor rgbaColor;

    QPalette ptt(widget->palette());
    QBrush brush = ptt.brush(widget->backgroundRole());

    QColor bColor = brush.color();
    rgbaColor.setColor(bColor.red(), bColor.green(), bColor.blue());

   QColor color = QColorDialog::getColor(brush.color(), this, tr("Color"));

    if (!color.isValid())
      return rgbaColor;

    QPalette paltt(widget->palette());
    paltt.setColor(widget->backgroundRole(), color);
    widget->setPalette(paltt);
    widget->setAutoFillBackground(true);

    rgbaColor.setColor(color.red(), color.green(), color.blue());

    return rgbaColor;
}

void te::layout::NorthSettingsOutside::load()
{
}

void te::layout::NorthSettingsOutside::onOkPushButtonPressed()
{
  /*if (!m_ui->txtEdtFile->text().isEmpty())
  {
    accept();
    return;
  }
  QMessageBox::information(this, tr("PDF Settings"), tr("Please complete the filename."));*/
}

void te::layout::NorthSettingsOutside::onCancelPushButtonPressed()
{
  reject();
}

void te::layout::NorthSettingsOutside::onSaveAsClicked()
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

void te::layout::NorthSettingsOutside::on_pbNorthLineColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->colorNorth);
  NorthSettingsController* controller = dynamic_cast<NorthSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(color, dataType->getDataTypeColor());
    Property prop = controller->updateProperty(m_northSettings->getLineColor(), variant, m_northType);
    emit updateProperty(prop);
  }
}

void te::layout::NorthSettingsOutside::setPosition(const double& x, const double& y)
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::NorthSettingsOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

