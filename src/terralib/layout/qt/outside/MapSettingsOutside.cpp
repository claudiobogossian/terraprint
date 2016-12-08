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
#include "../core/ItemUtils.h"

#include "ui_MapSettings.h"
#include "ui_MapLayerChoice.h"
#include "MapSettingsController.h"

// Qt
#include <QMessageBox>
#include <QString>
#include <QFileDialog>

#include <boost/lexical_cast.hpp>

te::layout::MapSettingsOutside::MapSettingsOutside(AbstractOutsideController* controller, QWidget* parent)
  : QDialog(parent),
  AbstractOutsideView(controller),
  m_ui(new Ui::MapSettings),
  m_widget(0)
{
  m_ui->setupUi(this);

  MapSettingsController* controllerSettings = dynamic_cast<MapSettingsController*>(m_controller);
  m_mapChoice = controllerSettings->getMapLayerChoice();
  m_mapChoice->setParent(this);
  m_widget.reset(m_mapChoice);
  m_mapChoice->init();

  QGridLayout* layout = new QGridLayout(m_ui->m_widget_MapChoice);
  layout->addWidget(m_widget.get());
  
  connect(m_widget.get(), SIGNAL(closeWidget()), this, SLOT(onCancelPushButtonPressed()));
  connect(m_widget.get(), SIGNAL(updateWidgetProperty(Property)), this, SLOT(updateWidgetProperty(Property)));
}

te::layout::MapSettingsOutside::~MapSettingsOutside()
{
  
}

void te::layout::MapSettingsOutside::init()
{
  m_mapChoice->init();
}

void te::layout::MapSettingsOutside::load()
{
  m_mapChoice->load();
}

void te::layout::MapSettingsOutside::setPosition(const double& x, const double& y)
{
  move(x, y);
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

void te::layout::MapSettingsOutside::onCancelPushButtonPressed()
{
  reject();
}

void te::layout::MapSettingsOutside::updateWidgetProperty(Property prop)
{
  emit updateProperty(prop);
}
