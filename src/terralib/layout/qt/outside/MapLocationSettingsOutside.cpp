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
#include "MapLocationSettingsOutside.h"
#include "MapLayerChoiceOutside.h"
#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../core/ItemUtils.h"
#include "../../core/property/SharedProperties.h"

#include "ui_LocationMapSettings.h"
#include "ui_MapLayerChoice.h"
#include "MapLocationSettingsController.h"

// Qt
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QColorDialog>

#include <boost/lexical_cast.hpp>

te::layout::MapLocationSettingsOutside::MapLocationSettingsOutside(AbstractOutsideController* controller, QWidget* parent)
  : QDialog(parent),
  AbstractOutsideView(controller),
  m_ui(new Ui::LocationMapSettings),
  ui_MapLayerChoice(new Ui::MapLayerChoice),
  m_widget(0)
{
  m_ui->setupUi(this);

  MapLocationSettingsController* controllerSettings = dynamic_cast<MapLocationSettingsController*>(m_controller);
  m_mapChoice = controllerSettings->getMapLayerChoice();
  m_mapChoice->setParent(this);
  m_widget.reset(m_mapChoice);
  m_mapChoice->init();

  SharedProperties sharedProps;

  initCombo(m_ui->cmbConnected, sharedProps.getItemObserver());

  QGridLayout* layout = new QGridLayout(m_ui->m_widget_MapChoice);
  layout->addWidget(m_widget.get());
  
  connect(m_widget.get(), SIGNAL(closeWidget()), this, SLOT(onCancelPushButtonPressed()));
  connect(m_widget.get(), SIGNAL(updateWidgetProperty(Property)), this, SLOT(updateWidgetProperty(Property)));

  loadColor(m_ui->frameFillColor, "reference_box_fill_color");
  loadColor(m_ui->frameContourColor, "reference_box_contour_color");

}


te::layout::MapLocationSettingsOutside::~MapLocationSettingsOutside()
{
  
}

void te::layout::MapLocationSettingsOutside::init()
{
  m_mapChoice->init();

  SharedProperties sharedProps;

  initCombo(m_ui->cmbConnected, sharedProps.getItemObserver());
}


void te::layout::MapLocationSettingsOutside::load()
{
  m_mapChoice->load();
}

void te::layout::MapLocationSettingsOutside::setPosition(const double& x, const double& y)
{
  move(x, y);
  refresh();
}

te::gm::Coord2D te::layout::MapLocationSettingsOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::MapLocationSettingsOutside::onCancelPushButtonPressed()
{
  reject();
}


void te::layout::MapLocationSettingsOutside::updateWidgetProperty(Property prop)
{
  emit updateProperty(prop);
 
}


void te::layout::MapLocationSettingsOutside::initCombo(QWidget* widget, std::string nameComponent)
{
  MapLocationSettingsController* controller = dynamic_cast<MapLocationSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getProperty(nameComponent);

  QComboBox* combo = dynamic_cast<QComboBox*>(widget);

  if (!combo)
    return;

  combo->blockSignals(true);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  int index = -1;
  QVariant variant;

  if (prop.getType() == dataType->getDataTypeBool())
  {
    variant.setValue(te::layout::Property::GetValueAs<bool>(prop));
  }
  else if (prop.getType() == dataType->getDataTypeDouble())
  {
    variant.setValue(te::layout::Property::GetValueAs<double>(prop));
  }
  else if (prop.getType() == dataType->getDataTypeInt())
  {
    variant.setValue(te::layout::Property::GetValueAs<int>(prop));
  }
  else if (prop.getType() == dataType->getDataTypeString())
  {
    std::string txt = te::layout::Property::GetValueAs<std::string>(prop);
    QString qText = ItemUtils::convert2QString(txt);
    variant.setValue(qText);
  }
  else if (prop.getType() == dataType->getDataTypeStringList())
  {
    std::string txt = prop.getOptionByCurrentChoice().toString();
    QString qText = ItemUtils::convert2QString(txt);
    variant.setValue(qText);
    if (combo->count() == 0)
    {
      addComboOptions(combo, prop.getOptionChoices());
    }
  }
  else if (prop.getType() == dataType->getDataTypeItemObserver())
  {
    EnumObjectType* dataTypeObj = Enums::getInstance().getEnumObjectType();
    QStringList list;
    list.append("");
    list = controller->getItemNames(list, dataTypeObj->getMapItem());
    combo->addItems(list);

    std::string txt = te::layout::Property::GetValueAs<std::string>(prop);
    QString qText = ItemUtils::convert2QString(txt);
    variant.setValue(qText);
  }

  //When the value is not a QString
  QString value = variant.toString();
  variant.setValue(value);

  index = combo->findData(variant, Qt::DisplayRole);
  if (index == -1)
  {
    index = combo->findText(value);
    if (index != -1)
    {
      combo->setCurrentIndex(index);
    }
  }
  else
  {
    combo->setCurrentIndex(index);
  }
  combo->blockSignals(false);
}

void te::layout::MapLocationSettingsOutside::addComboOptions(QComboBox* combo, std::vector<Variant> options)
{
  QStringList list;
  for (std::vector<Variant>::iterator it = options.begin(); it != options.end(); ++it)
  {
    std::string value = (*it).toString();
    QString qValue = ItemUtils::convert2QString(value);
    list.append(qValue);
  }
  combo->addItems(list);
}

void te::layout::MapLocationSettingsOutside::on_cmbConnected_currentIndexChanged(const QString & text)
{
  MapLocationSettingsController* controller = dynamic_cast<MapLocationSettingsController*>(m_controller);
  if (!controller)
    return;


  SharedProperties sharedProps;

  Property pConnected = controller->getProperty(sharedProps.getItemObserver());

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  pConnected.setValue(text.toStdString(), dataType->getDataTypeItemObserver());

  emit updateProperty(pConnected);

  
}

void te::layout::MapLocationSettingsOutside::on_colorButton_pressed()
{

  te::color::RGBAColor color = configColor(m_ui->frameFillColor);
  MapLocationSettingsController* controller = dynamic_cast<MapLocationSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    Property prop = controller->getProperty("reference_box_fill_color");
    prop.setValue(color, dataType->getDataTypeColor());
    emit updateProperty(prop);
  }

}

void te::layout::MapLocationSettingsOutside::on_cColorButton_pressed()
{
  te::color::RGBAColor color = configColor(m_ui->frameContourColor);
  MapLocationSettingsController* controller = dynamic_cast<MapLocationSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("reference_box_contour_color");
    prop.setValue(color, dataType->getDataTypeColor());
    emit updateProperty(prop);
  }
  
}

te::color::RGBAColor te::layout::MapLocationSettingsOutside::configColor(QWidget* widget)
{
  te::color::RGBAColor rgbaColor;

  QPalette ptt(widget->palette());
  QBrush brush = ptt.brush(widget->backgroundRole());

  QColor bColor = brush.color();
  rgbaColor.setColor(bColor.red(), bColor.green(), bColor.blue());

  QColor color = QColorDialog::getColor(brush.color(), this, "Color", QColorDialog::ShowAlphaChannel);

  if (!color.isValid())
    return rgbaColor;

  QPalette paltt(widget->palette());
  paltt.setColor(widget->backgroundRole(), color);
  widget->setPalette(paltt);
  widget->setAutoFillBackground(true);

  rgbaColor.setColor(color.red(), color.green(), color.blue());

  return rgbaColor;
}

void te::layout::MapLocationSettingsOutside::loadColor(QWidget* widget, std::string nameComponent)
{
  MapLocationSettingsController* controller = dynamic_cast<MapLocationSettingsController*>(m_controller);
  if (controller)
  {
    Property prop = controller->getProperty(nameComponent);

    const te::color::RGBAColor& rgbaColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(prop);

    QColor color(rgbaColor.getRed(), rgbaColor.getGreen(), rgbaColor.getBlue(), rgbaColor.getAlpha());

    QPalette paltt(widget->palette());
    paltt.setColor(widget->backgroundRole(), color);
    widget->setPalette(paltt);
    widget->setAutoFillBackground(true);

  }

}
