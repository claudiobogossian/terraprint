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
#include "../../outside/MapSettingsController.h"

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

  m_ui->lneHeight->setValidator(new QDoubleValidator(0.0, 99999.999, 9, m_ui->lneHeight));
  m_ui->lneWidth->setValidator(new QDoubleValidator(0.0, 99999.999, 9, m_ui->lneWidth));

  m_ui->cmbUnit->setItemData(0, QVariant("Centimeter"));
  m_ui->cmbUnit->setItemData(1, QVariant("Millimeter"));

  MapSettingsController* controllerSettings = dynamic_cast<MapSettingsController*>(m_controller);
  MapLayerChoiceOutside *mapChoice = controllerSettings->getMapLayerChoice();
  mapChoice->setParent(this);
  m_widget.reset(mapChoice);
  mapChoice->init();

  QGridLayout* layout = new QGridLayout(m_ui->m_widget_MapChoice);
  layout->addWidget(m_widget.get());

  init();
}

te::layout::MapSettingsOutside::~MapSettingsOutside()
{
  
}


void te::layout::MapSettingsOutside::load()
{

  loadScaleCombobox();
  initDouble(m_ui->lneHeight, "height");
  initDouble(m_ui->lneWidth, "width");
  initCombo(m_ui->cmbUnit, "size_unit");
  initCombo(m_ui->cmbScale, "scale");
  initBool(m_ui->ckbFixedScale, "fixed_scale");

}

void te::layout::MapSettingsOutside::init()
{
  if (m_ui->pBtnOK && m_ui->pBtnCancel)
  {
    connect(m_ui->pBtnOK, SIGNAL(pressed()), this, SLOT(onOkPushButtonPressed()));
    connect(m_ui->pBtnCancel, SIGNAL(pressed()), this, SLOT(onCancelPushButtonPressed()));

  }

}

void te::layout::MapSettingsOutside::onOkPushButtonPressed()
{
  if (!m_ui->lneWidth->text().isEmpty() && !m_ui->lneHeight->text().isEmpty())
  {
    MapLayerChoiceOutside* mapLayerChoice = dynamic_cast<MapLayerChoiceOutside*>(m_widget.get());
    Property prop = mapLayerChoice->getSavedLayers();
    emit updateProperty(prop); 
    
    accept();
    return;
  }
  QMessageBox::information(this, tr("Map Settings"), tr("Set Width and Height."));
}

void te::layout::MapSettingsOutside::onCancelPushButtonPressed()
{
  reject();
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



void te::layout::MapSettingsOutside::initDouble(QWidget* widget, std::string nameComponent)
{
  MapSettingsController* controller = dynamic_cast<MapSettingsController*>(m_controller);
  if (!controller)
    return;

  std::ostringstream convert;
  convert.precision(3);

  std::string unit = controller->getProperty("size_unit").getOptionByCurrentChoice().convertToString();

  if ((nameComponent == "height" || nameComponent == "width") && unit == "Centimeter"){

    Property prop = controller->getProperty(nameComponent);
    double number = prop.getValue().toDouble();
    double convertedNumber = mm2cm(number);
    convert << convertedNumber;

  }
  else{

    Property prop = controller->getProperty(nameComponent);
    double number = prop.getValue().toDouble();
    convert << number;
  }

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if (edit)
  {
    edit->setText(convert.str().c_str());
  }
}

void te::layout::MapSettingsOutside::initCombo(QWidget* widget, std::string nameComponent){

  MapSettingsController* controller = dynamic_cast<MapSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getProperty(nameComponent);

  QComboBox* combo = dynamic_cast<QComboBox*>(widget);

  if (!combo)
    return;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  int index = -1;
  QVariant variant;

  if (prop.getType() == dataType->getDataTypeDouble())
  {
    variant.setValue(prop.getValue().toDouble());
  }
  else if (prop.getType() == dataType->getDataTypeInt())
  {
    variant.setValue(prop.getValue().toInt());
  }
  else if (prop.getType() == dataType->getDataTypeString())
  {
    std::string txt = prop.getValue().toString();
    QString qText = ItemUtils::convert2QString(txt);

    variant.setValue(qText);
  }
  else if (prop.getType() == dataType->getDataTypeStringList()){

    std::string txt = prop.getOptionByCurrentChoice().toString();
    QString qText = ItemUtils::convert2QString(txt);

    variant.setValue(qText);

  }

  QString value = variant.toString();
  variant.setValue(value);



  if (nameComponent == "size_unit"){

    index = combo->findData(variant);
  
  }
  else{

    index = combo->findData(variant, Qt::DisplayRole);

  }

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

}

void te::layout::MapSettingsOutside::on_lneWidth_editingFinished(){

  MapSettingsController* controller = dynamic_cast<MapSettingsController*>(m_controller);
  if (controller){
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    double widthSize = m_ui->lneWidth->text().trimmed().replace(",", ".").toDouble();


    std::string unit = controller->getProperty("size_unit").getOptionByCurrentChoice().convertToString();

    if (unit == "Centimeter"){
     
      widthSize = cm2mm(widthSize);

    }

    Variant variant;
    variant.setValue(widthSize, dataType->getDataTypeDouble());

    Property prop = controller->getProperty("width");
    prop.setValue(variant);
    
    emit updateProperty(prop);
  }

}

void te::layout::MapSettingsOutside::on_lneHeight_editingFinished(){

  MapSettingsController* controller = dynamic_cast<MapSettingsController*>(m_controller);
  if (controller){
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    double heightSize = m_ui->lneHeight->text().trimmed().replace(",", ".").toDouble();

    std::string unit = controller->getProperty("size_unit").getOptionByCurrentChoice().convertToString();
    if (unit == "Centimeter"){

      heightSize = cm2mm(heightSize);

    }

    Variant variant;
    variant.setValue(heightSize, dataType->getDataTypeDouble());

    Property prop = controller->getProperty("height");
    prop.setValue(variant);
    emit updateProperty(prop);
  }

}

void te::layout::MapSettingsOutside::loadScaleCombobox(){

  MapSettingsController* controller = dynamic_cast<MapSettingsController*>(m_controller);
  if (controller){
    Property prop = controller->getProperty("scale");
    int currentScale = (int) prop.getValue().toDouble();


    std::string stringValue =  formatScaleValue(boost::lexical_cast<std::string>(currentScale));
    std::string concatString = TR_LAYOUT("Initial Scale (") + stringValue + ")";
    m_ui->cmbScale->addItem(concatString.c_str(), QVariant((double)controller->getProperty("scale").getValue().toDouble()));
  
  }

  m_ui->cmbScale->addItem("1 000", QVariant((double)1000));
  m_ui->cmbScale->addItem("2 000", QVariant((double)2000));
  m_ui->cmbScale->addItem("2 500", QVariant((double)2500));
  m_ui->cmbScale->addItem("5 000", QVariant((double)5000));
  m_ui->cmbScale->addItem("10 000", QVariant((double)10000));
  m_ui->cmbScale->addItem("20 000", QVariant((double)20000));
  m_ui->cmbScale->addItem("25 000", QVariant((double)25000));
  m_ui->cmbScale->addItem("50 000", QVariant((double)50000));
  m_ui->cmbScale->addItem("100 000", QVariant((double)100000));
  m_ui->cmbScale->addItem("200 000", QVariant((double)200000));
  m_ui->cmbScale->addItem("250 000", QVariant((double)250000));
  m_ui->cmbScale->addItem("500 000", QVariant((double)500000));
  m_ui->cmbScale->addItem("1 000 000", QVariant((double)1000000));
  m_ui->cmbScale->addItem("2 000 000", QVariant((double)2000000));
  m_ui->cmbScale->addItem("2 500 000", QVariant((double)2500000));
  m_ui->cmbScale->addItem("5 000 000", QVariant((double)5000000));
  m_ui->cmbScale->addItem("10 000 000", QVariant((double)10000000));
  m_ui->cmbScale->addItem("20 000 000", QVariant((double)20000000));
  m_ui->cmbScale->addItem("25 000 000", QVariant((double)25000000));
  m_ui->cmbScale->addItem("50 000 000", QVariant((double)50000000));
  m_ui->cmbScale->addItem("100 000 000", QVariant((double)100000000));

}

void te::layout::MapSettingsOutside::on_cmbScale_currentIndexChanged(const QString & text){



  MapSettingsController* controller = dynamic_cast<MapSettingsController*>(m_controller);

  double inputValue = text.toDouble();

  if (inputValue > 0.0){
    if (controller)
    {
      string formatedString = formatScaleValue(text.toStdString());

      m_ui->cmbScale->setItemText(m_ui->cmbScale->currentIndex(), ItemUtils::convert2QString(formatedString));
      m_ui->cmbScale->setItemData(m_ui->cmbScale->currentIndex(), QVariant((double)inputValue));

      Property prop = controller->getProperty("scale");
      EnumDataType* dataType = Enums::getInstance().getEnumDataType();
      
      Variant variant;
      
      variant.setValue(inputValue, dataType->getDataTypeDouble());
      prop.setValue(variant);

      emit updateProperty(prop);
    }
  }
  else{

    if (controller)
    {
      Property prop = controller->getProperty("scale");
      EnumDataType* dataType = Enums::getInstance().getEnumDataType();
      QVariant selectedValue = m_ui->cmbScale->itemData(m_ui->cmbScale->currentIndex());
      
      if (selectedValue.toDouble() == 0.0){
        return;
      }

      Variant variant;
      variant.setValue(selectedValue, dataType->getDataTypeDouble());
      prop.setValue(variant);

      emit updateProperty(prop);

    }
  }

}

void te::layout::MapSettingsOutside::on_cmbUnit_currentIndexChanged(const QString & text){
  
  MapSettingsController* controller = dynamic_cast<MapSettingsController*>(m_controller);
  if (controller)
  {

    std::string cm = "Centimeter";
    std::string mm = "Millimeter";

    std::string selectedUnit =  ItemUtils::convert2StdString(m_ui->cmbUnit->itemData(m_ui->cmbUnit->currentIndex()).toString());
    //std::string stdText = ItemUtils::convert2StdString(text);

    Property prop = controller->getProperty("size_unit");
    if (selectedUnit != prop.getOptionByCurrentChoice().convertToString()){
      if (selectedUnit == cm){
        
        double cmWidth = mm2cm(m_ui->lneWidth->text().toDouble());
        m_ui->lneWidth->setText(QString(boost::lexical_cast<std::string>(cmWidth).c_str()));

        double cmHeight = mm2cm(m_ui->lneHeight->text().toDouble());
        m_ui->lneHeight->setText(QString(boost::lexical_cast<std::string>(cmHeight).c_str()));
    }
      
      if (selectedUnit == mm){

        double mmWidth = cm2mm(m_ui->lneWidth->text().toDouble());
        m_ui->lneWidth->setText(QString(boost::lexical_cast<std::string>(mmWidth).c_str()));

        double mmHeight = cm2mm(m_ui->lneHeight->text().toDouble());
        m_ui->lneHeight->setText(QString(boost::lexical_cast<std::string>(mmHeight).c_str()));

    }

    }

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(selectedUnit, dataType->getDataTypeStringList());

    prop.setValue(variant);
    prop.setOptionChoice(variant);

    emit updateProperty(prop);
  }
}

double te::layout::MapSettingsOutside::mm2cm(double mmSize){
  return mmSize / 10.;
}

double  te::layout::MapSettingsOutside::cm2mm(double cmSize){
  return cmSize * 10.;
}

void  te::layout::MapSettingsOutside::on_ckbFixedScale_clicked(){
    
  MapSettingsController* controller = dynamic_cast<MapSettingsController*>(m_controller);
  if (controller)
  {
    Property prop = controller->getProperty("fixed_scale");

    bool isBlocked = m_ui->ckbFixedScale->isChecked();
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(isBlocked, dataType->getDataTypeBool());

    prop.setValue(variant);

    emit updateProperty(prop);

  }

}

void te::layout::MapSettingsOutside::initBool(QWidget* widget, std::string nameComponent)
{
  MapSettingsController* controller = dynamic_cast<MapSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getProperty(nameComponent);

  QCheckBox* chk = dynamic_cast<QCheckBox*>(widget);

  if (chk)
  {
    chk->setChecked(prop.getValue().toBool());
  }
}

std::string  te::layout::MapSettingsOutside::formatScaleValue(std::string inputValue){

  string formatedValue = inputValue;

  string formatedString = "";


  int size = (int) formatedValue.size();
  
  int indexSpace = 1;
  

  for (int i = size - 1; i >= 0; i--){

    if (((indexSpace % 3) == 0) && i > 0){

      std::string tempBuffer = " ";
      tempBuffer += formatedValue[i];
      tempBuffer += formatedString;

      formatedString = tempBuffer;

    
    }
    else{

      formatedString = formatedValue[i] + formatedString;

    }

    indexSpace++;
  
  }

  return formatedString;
}