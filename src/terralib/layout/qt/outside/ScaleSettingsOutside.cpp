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
  \file terralib/layout/qt/outside/ScaleSettingsOutside.cpp

  \brief A dialog configure the input layer to address geocoding operation
*/

// TerraLib
#include "ScaleSettingsOutside.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "ui_ScaleSettings.h"

// Qt
#include <QMessageBox>
#include <QString>
#include <QFileDialog>

te::layout::ScaleSettingsOutside::ScaleSettingsOutside(AbstractOutsideController* controller)
  : QDialog(0),
  AbstractOutsideView(controller),
  m_ui(new Ui::ScaleSettings)
{
  m_ui->setupUi(this);

  setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint
    | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);
  
  init();
}

te::layout::ScaleSettingsOutside::~ScaleSettingsOutside()
{
  
}

void te::layout::ScaleSettingsOutside::init()
{  
  /*if (m_ui->pBtnOK && m_ui->pBtnCancel && m_ui->pBtnSaveAs)
  {
    connect(m_ui->pBtnOK, SIGNAL(pressed()), this, SLOT(onOkPushButtonPressed()));
    connect(m_ui->pBtnCancel, SIGNAL(pressed()), this, SLOT(onCancelPushButtonPressed()));
    connect(m_ui->pBtnSaveAs, SIGNAL(clicked()), this, SLOT(onSaveAsClicked()));
    connect(m_ui->cmbDPI, SIGNAL(currentIndexChanged(const QString)), this, SLOT(onCurrentIndexChanged(const QString)));
  }*/
}

void te::layout::ScaleSettingsOutside::onOkPushButtonPressed()
{
 
}

void te::layout::ScaleSettingsOutside::onCancelPushButtonPressed()
{
  reject();
}

void te::layout::ScaleSettingsOutside::onSaveAsClicked()
{

}

void te::layout::ScaleSettingsOutside::onCurrentIndexChanged(const QString & text)
{
 
}

void te::layout::ScaleSettingsOutside::setPosition(const double& x, const double& y)
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::ScaleSettingsOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

std::string te::layout::ScaleSettingsOutside::getFilePath()
{
  std::string result = "";
  return result;
}

int te::layout::ScaleSettingsOutside::getDPI()
{
  return 0;
}

void te::layout::ScaleSettingsOutside::setCurrentDPI(int dpi)
{
 
}

