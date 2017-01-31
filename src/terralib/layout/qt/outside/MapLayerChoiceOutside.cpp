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
  \file terralib/layout/qt/outside/MapLayerChoiceOutside.cpp

  \brief A dialog configure the input layer to address geocoding operation
*/ 

// TerraLib
#include "MapLayerChoiceOutside.h"
#include "terralib/common/Logger.h"
#include "terralib/qt/widgets/utils/DoubleListWidget.h"
#include "../../outside/MapLayerChoiceModel.h"
#include "../../core/enum/Enums.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../../core/Constants.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../core/ItemUtils.h"
#include "ui_MapLayerChoice.h"
#include "../../outside/MapLayerChoiceController.h"

#include <boost/lexical_cast.hpp>

// STL
#include <algorithm>

// Qt
#include <QGridLayout>
#include <QMessageBox>
#include <QString>
#include <QKeyEvent>

te::layout::MapLayerChoiceOutside::MapLayerChoiceOutside(AbstractOutsideController* controller, QWidget* parent)
  : QDialog(parent),
    AbstractOutsideView(controller),
    m_ui(new Ui::MapLayerChoice)
{
// add controls
  m_ui->setupUi(this);
  
  m_widget.reset(new te::qt::widgets::DoubleListWidget(m_ui->m_widget));
  
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_widget);
  displayLayout->addWidget(m_widget.get());

  m_widget->setLeftLabel(tr("Available Layer"));
  m_widget->setRightLabel(tr("Selected Layer"));

  m_ui->cmbUnit->blockSignals(true);
  m_ui->cmbUnit->setItemData(0, QVariant("Centimeter"));
  m_ui->cmbUnit->setItemData(1, QVariant("Millimeter"));
  m_ui->cmbUnit->blockSignals(false);

  /* In a QLineEdit, when you click the Enter button, an editFinished signal is triggered, 
  however in a window there are buttons set as default, other events such as DynamicPropertyChange 
  are sent to these buttons, and causes QLineEdit to lose focus for a short time. 
  This causes the editingFinished to be called 2x, since QEditLine's "lost focus" also calls this method. 
  To prevent such calls, no button is default in this window, just as it does not become default when clicked.
  By default the enter is to signal that the value has been modified, so no button should be default and get focus.*/
  m_ui->pBtnOK->setDefault(false);
  m_ui->pBtnOK->setAutoDefault(false);
  m_ui->pBtnCancel->setDefault(false);
  m_ui->pBtnCancel->setAutoDefault(false);
  m_ui->helpPushButton->setDefault(false);
  m_ui->helpPushButton->setAutoDefault(false);
}

te::layout::MapLayerChoiceOutside::~MapLayerChoiceOutside()
{

}

void te::layout::MapLayerChoiceOutside::init()
{
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
  MapLayerChoiceModel* model = dynamic_cast<MapLayerChoiceModel*>(abstractModel);
  if(!model)
  {
    return;
  }

  std::vector <std::string> namesToInput;
  std::vector <std::string> namesToOutput;

  // Layers From Map Items
  std::list<te::map::AbstractLayerPtr> selectedLayers = getSelectedLayers();//model->getSelectedLayers();
  
  // All Layers from Project
  std::list<te::map::AbstractLayerPtr> layers = getLayers();//model->getLayers();

  for (std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin(); it != selectedLayers.end(); ++it)
  {
    te::map::AbstractLayerPtr layer = (*it);
    if (std::find(layers.begin(), layers.end(), layer) != layers.end())
    {
      std::list<te::map::AbstractLayerPtr>::iterator findIt = std::find(layers.begin(), layers.end(), layer);
      namesToOutput.push_back(layer->getTitle());
    }
  }

  if (m_ui->pBtnOK && m_ui->pBtnCancel)
  {
    connect(m_ui->pBtnOK, SIGNAL(clicked()), this, SLOT(onOkPushButtonPressed()));
    connect(m_ui->pBtnCancel, SIGNAL(clicked()), this, SLOT(onCancelPushButtonPressed()));
  }
  
  namesToInput = intersectionLayersTitle(namesToOutput);
  
  m_widget->blockSignals(true);
  m_widget->setInputValues(namesToInput);
  m_widget->setOutputValues(namesToOutput);
  m_widget->blockSignals(false);
}

te::layout::Property te::layout::MapLayerChoiceOutside::getSavedLayers()
{  
  m_layersOnTheRight = m_widget->getOutputValues();

  std::list<te::map::AbstractLayerPtr> layerListMap = getLayers();//model->getLayers();

  std::vector<std::string>::iterator itString = m_layersOnTheRight.begin();
  for ( ; itString != m_layersOnTheRight.end() ; ++itString)
  {
    std::list<te::map::AbstractLayerPtr>::iterator it = layerListMap.begin();
    for( ; it != layerListMap.end() ; ++it)
    {
      te::map::AbstractLayerPtr layer = it->get();
      std::string nameLayer = layer->getTitle();

      QString qNameLayer(nameLayer.c_str());
      nameLayer = ItemUtils::convert2StdString(qNameLayer);

      std::string name = (*itString);
      if(nameLayer.compare(name) == 0)
      {
        m_layersSelected.push_back(layer);
      }
    }
  }    

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  // Layers From Map Items
  std::list<te::map::AbstractLayerPtr> selectedLayers = getSelectedLayers();

  Property prop = getProperty("layers");
  prop.setValue(m_layersSelected, dataType->getDataTypeLayerList());

  m_layersSelected.clear();

  return prop;
}

void te::layout::MapLayerChoiceOutside::onCancelPushButtonClicked()
{
 
}

void te::layout::MapLayerChoiceOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::MapLayerChoiceOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

std::vector<std::string> te::layout::MapLayerChoiceOutside::intersectionLayersTitle(std::vector<std::string> output)
{
  std::vector <std::string> namesToInput;

  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
  MapLayerChoiceModel* model = dynamic_cast<MapLayerChoiceModel*>(abstractModel);
  if (!model)
  {
    return namesToInput;
  }

  // All Layers from Project
  std::list<te::map::AbstractLayerPtr> layers = getLayers();

  for (std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin(); it != layers.end(); ++it)
  {
    te::map::AbstractLayerPtr layer = (*it);
    std::string nameLayer = layer->getTitle();
    if (std::find(output.begin(), output.end(), nameLayer) == output.end())
    {
      namesToInput.push_back(layer->getTitle());
    }
  }
  return namesToInput;
}

std::list<te::map::AbstractLayerPtr> te::layout::MapLayerChoiceOutside::getLayers()
{
  std::list<te::map::AbstractLayerPtr> currentLayers;
  if (!m_controller)
    return currentLayers;

  MapLayerChoiceController* controllerlayers = dynamic_cast<MapLayerChoiceController*>(m_controller);
  if (!controllerlayers)
    currentLayers;

  return   controllerlayers->getlistLayers();;
}

std::list<te::map::AbstractLayerPtr> te::layout::MapLayerChoiceOutside::getSelectedLayers()
{
  std::list<te::map::AbstractLayerPtr> currentLayers;
  MapLayerChoiceController* controllerlayers = dynamic_cast<MapLayerChoiceController*>(m_controller);
  if (!controllerlayers)
    currentLayers;

  return   controllerlayers->getSelectedlayers();
}

te::layout::Property te::layout::MapLayerChoiceOutside::getProperty(std::string name)
{
  std::list<te::map::AbstractLayerPtr> currentLayers;
  MapLayerChoiceController* controller = dynamic_cast<MapLayerChoiceController*>(m_controller);
  if (!controller)
    currentLayers;

  return   controller->getProperty(name);
}

void te::layout::MapLayerChoiceOutside::on_lneWidth_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneWidth->isModified())
  {
    return;
  }

  MapLayerChoiceController* controller = dynamic_cast<MapLayerChoiceController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    double widthSize = m_ui->lneWidth->text().trimmed().replace(",", ".").toDouble();

    std::string unit = controller->getProperty("size_unit").getOptionByCurrentChoice().convertToString();

    if (unit == "Centimeter")
    {
      widthSize = cm2mm(widthSize);
    }

    Property prop = controller->getProperty("width");
    prop.setValue(widthSize, dataType->getDataTypeDouble());

    emit updateWidgetProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus (the editingFinished is automatically
    called in the "lost focus") */
    m_ui->lneWidth->setModified(false);
  }
}

void te::layout::MapLayerChoiceOutside::on_lneHeight_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method 
    when QLineEdit loses focus (the editingFinished is automatically 
    called in the "lost focus") */
  if (!m_ui->lneHeight->isModified())
  {
    return;
  }

  MapLayerChoiceController* controller = dynamic_cast<MapLayerChoiceController*>(m_controller);
  if (controller){
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    double heightSize = m_ui->lneHeight->text().trimmed().replace(",", ".").toDouble();

    std::string unit = controller->getProperty("size_unit").getOptionByCurrentChoice().convertToString();
    if (unit == "Centimeter")
    {
      heightSize = cm2mm(heightSize);
    }

    Property prop = controller->getProperty("height");
    prop.setValue(heightSize, dataType->getDataTypeDouble());
    emit updateWidgetProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus (the editingFinished is automatically
    called in the "lost focus") */
    m_ui->lneHeight->setModified(false);
  }
}

void te::layout::MapLayerChoiceOutside::on_cmbScale_currentIndexChanged(const QString & text)
{
  MapLayerChoiceController* controller = dynamic_cast<MapLayerChoiceController*>(m_controller);

  QString copyText = text;
  copyText.remove('.');
  double inputValue = (double)copyText.remove('.').toInt();

  if (inputValue > 0.0)
  {
    if (controller)
    {
      std::string currentValue = boost::lexical_cast<std::string>(inputValue);
      string formatedString = formatScaleValue(currentValue);

      m_ui->cmbScale->setItemText(m_ui->cmbScale->currentIndex(), ItemUtils::convert2QString(formatedString));
      m_ui->cmbScale->setItemData(m_ui->cmbScale->currentIndex(), QVariant(inputValue));

      Property prop = controller->getProperty("scale");
      EnumDataType* dataType = Enums::getInstance().getEnumDataType();

      prop.setValue(inputValue, dataType->getDataTypeDouble());

      emit updateWidgetProperty(prop);
    }
  }
  else
  {
    if (controller)
    {
      Property prop = controller->getProperty("scale");
      EnumDataType* dataType = Enums::getInstance().getEnumDataType();
      QVariant selectedValue = m_ui->cmbScale->itemData(m_ui->cmbScale->currentIndex());

      if (selectedValue.toDouble() == 0.0){
        return;
      }

      prop.setValue(selectedValue.toDouble(), dataType->getDataTypeDouble());

      emit updateWidgetProperty(prop);
    }
  }
}

void te::layout::MapLayerChoiceOutside::on_cmbUnit_currentIndexChanged(const QString & text)
{
  MapLayerChoiceController* controller = dynamic_cast<MapLayerChoiceController*>(m_controller);
  if (controller)
  {
    std::string cm = "Centimeter";
    std::string mm = "Millimeter";

    std::string selectedUnit = ItemUtils::convert2StdString(m_ui->cmbUnit->itemData(m_ui->cmbUnit->currentIndex()).toString());

    Property prop = controller->getProperty("size_unit");
    if (selectedUnit != prop.getOptionByCurrentChoice().convertToString())
    {
      if (selectedUnit == cm)
      {
        double cmWidth = mm2cm(m_ui->lneWidth->text().toDouble());
        m_ui->lneWidth->setText(QString::number(cmWidth, 'f', CENTIMETER_PRECISION));

        double cmHeight = mm2cm(m_ui->lneHeight->text().toDouble());
        m_ui->lneHeight->setText(QString::number(cmHeight, 'f', CENTIMETER_PRECISION));

        QString qUnit("cm");

        m_ui->lblUnitW->setText(qUnit);
        m_ui->lblUnitH->setText(qUnit);

        QDoubleValidator* validator = new QDoubleValidator(0.0, 9999999999, CENTIMETER_PRECISION, this);
        validator->setNotation(QDoubleValidator::StandardNotation);

        m_ui->lneWidth->setValidator(validator);
        m_ui->lneHeight->setValidator(validator);

      }
      if (selectedUnit == mm)
      {
        double mmWidth = cm2mm(m_ui->lneWidth->text().toDouble());
        m_ui->lneWidth->setText(QString::number(mmWidth, 'f', MILLIMETER_PRECISION));

        double mmHeight = cm2mm(m_ui->lneHeight->text().toDouble());
        m_ui->lneHeight->setText(QString::number(mmHeight, 'f', MILLIMETER_PRECISION));

        QString qUnit("mm");

        m_ui->lblUnitW->setText(qUnit);
        m_ui->lblUnitH->setText(qUnit);

        QDoubleValidator* validator = new QDoubleValidator(0.0, 999999999.99, MILLIMETER_PRECISION, this);
        validator->setNotation(QDoubleValidator::StandardNotation);

        m_ui->lneWidth->setValidator(validator);
        m_ui->lneHeight->setValidator(validator);

      }
    }

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(selectedUnit, dataType->getDataTypeStringList());

    prop.setValue(selectedUnit, dataType->getDataTypeStringList());
    prop.setOptionChoice(variant);

    emit updateWidgetProperty(prop);
  }
}

void  te::layout::MapLayerChoiceOutside::on_ckbFixedScale_clicked()
{
  MapLayerChoiceController* controller = dynamic_cast<MapLayerChoiceController*>(m_controller);
  if (controller)
  {
    Property prop = controller->getProperty("fixed_scale");

    bool isBlocked = m_ui->ckbFixedScale->isChecked();
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    prop.setValue(isBlocked, dataType->getDataTypeBool());

    emit updateWidgetProperty(prop);
  }
}

void te::layout::MapLayerChoiceOutside::load()
{
  loadScaleCombobox();
  initDouble(m_ui->lneHeight, "height");
  initDouble(m_ui->lneWidth, "width");
  initCombo(m_ui->cmbUnit, "size_unit");
  initCombo(m_ui->cmbScale, "scale");
  initBool(m_ui->ckbFixedScale, "fixed_scale");
  if (m_ui->cmbUnit->currentIndex() == 0)
  {
    QString qUnit("cm");
    m_ui->lblUnitW->setText(qUnit);
    m_ui->lblUnitH->setText(qUnit);

    QDoubleValidator* validator = new QDoubleValidator(0.0, 9999999999, CENTIMETER_PRECISION, this);
    validator->setNotation(QDoubleValidator::StandardNotation);
    
    m_ui->lneWidth->blockSignals(true);
    m_ui->lneHeight->blockSignals(true);
    m_ui->lneWidth->setValidator(validator);
    m_ui->lneHeight->setValidator(validator);
    m_ui->lneWidth->blockSignals(false);
    m_ui->lneHeight->blockSignals(false);
  }
  else
  {
    QString qUnit("mm");
    m_ui->lblUnitW->setText(qUnit);
    m_ui->lblUnitH->setText(qUnit);

    QDoubleValidator* validator = new QDoubleValidator(0.0, 9999999999, MILLIMETER_PRECISION, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    m_ui->lneWidth->blockSignals(true);
    m_ui->lneHeight->blockSignals(true);
    m_ui->lneWidth->setValidator(validator);
    m_ui->lneHeight->setValidator(validator);
    m_ui->lneWidth->blockSignals(false);
    m_ui->lneHeight->blockSignals(false);
  }

}

void te::layout::MapLayerChoiceOutside::onOkPushButtonPressed()
{
  if (!m_ui->lneWidth->text().isEmpty() && !m_ui->lneHeight->text().isEmpty())
  {
    Property prop = getSavedLayers();
    emit updateWidgetProperty(prop);

    accept();
    emit closeWidget();
    return;
  }
  QMessageBox::information(this, tr("Map Settings"), tr("Set Width and Height."));
}

void te::layout::MapLayerChoiceOutside::onCancelPushButtonPressed()
{
  reject();
  emit closeWidget();
}

void te::layout::MapLayerChoiceOutside::initDouble(QWidget* widget, std::string nameComponent)
{
  MapLayerChoiceController* controller = dynamic_cast<MapLayerChoiceController*>(m_controller);
  if (!controller)
    return;

  std::ostringstream convert;
  convert.precision(3);

  std::string unit = controller->getProperty("size_unit").getOptionByCurrentChoice().convertToString();

  if ((nameComponent == "height" || nameComponent == "width") && unit == "Centimeter")
  {
    Property prop = controller->getProperty(nameComponent);
    double number = te::layout::Property::GetValueAs<double>(prop);
    double convertedNumber = mm2cm(number);
    convert << convertedNumber;
  }
  else
  {
    Property prop = controller->getProperty(nameComponent);
    if (prop.isNull())
    {
      convert << 0;
    }
    else
    {
      double number = te::layout::Property::GetValueAs<double>(prop);
      convert << number;
    }
  }

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if (edit)
  {
    edit->blockSignals(true);
    edit->setText(convert.str().c_str());
    edit->blockSignals(false);
  }
}

void te::layout::MapLayerChoiceOutside::initCombo(QWidget* widget, std::string nameComponent)
{
  MapLayerChoiceController* controller = dynamic_cast<MapLayerChoiceController*>(m_controller);
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

  if (prop.getType() == dataType->getDataTypeDouble())
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
  combo->blockSignals(false);
}

void te::layout::MapLayerChoiceOutside::loadScaleCombobox()
{
  MapLayerChoiceController* controller = dynamic_cast<MapLayerChoiceController*>(m_controller);

  m_ui->cmbScale->blockSignals(true);

  if (controller)
  {
    Property initialProp = controller->getProperty("scale");
    if (initialProp.isNull())
      return;

    int currentScale = (int)te::layout::Property::GetValueAs<double>(initialProp);

    std::string stringValue = formatScaleValue(boost::lexical_cast<std::string>(currentScale));

    Property scaleValue = controller->getProperty("scale");

    double dValue = te::layout::Property::GetValueAs<double>(scaleValue);

    QVariant vScale = dValue;

    m_ui->cmbScale->addItem(stringValue.c_str(), vScale);
  }

  m_ui->cmbScale->addItem("1.000", QVariant((double)1000));
  m_ui->cmbScale->addItem("2.000", QVariant((double)2000));
  m_ui->cmbScale->addItem("2.500", QVariant((double)2500));
  m_ui->cmbScale->addItem("5.000", QVariant((double)5000));
  m_ui->cmbScale->addItem("10.000", QVariant((double)10000));
  m_ui->cmbScale->addItem("20.000", QVariant((double)20000));
  m_ui->cmbScale->addItem("25.000", QVariant((double)25000));
  m_ui->cmbScale->addItem("50.000", QVariant((double)50000));
  m_ui->cmbScale->addItem("100.000", QVariant((double)100000));
  m_ui->cmbScale->addItem("200.000", QVariant((double)200000));
  m_ui->cmbScale->addItem("250.000", QVariant((double)250000));
  m_ui->cmbScale->addItem("500.000", QVariant((double)500000));
  m_ui->cmbScale->addItem("1.000.000", QVariant((double)1000000));
  m_ui->cmbScale->addItem("2.000.000", QVariant((double)2000000));
  m_ui->cmbScale->addItem("2.500.000", QVariant((double)2500000));
  m_ui->cmbScale->addItem("5.000.000", QVariant((double)5000000));
  m_ui->cmbScale->addItem("10.000.000", QVariant((double)10000000));
  m_ui->cmbScale->addItem("20.000.000", QVariant((double)20000000));
  m_ui->cmbScale->addItem("25.000.000", QVariant((double)25000000));
  m_ui->cmbScale->addItem("50.000.000", QVariant((double)50000000));
  m_ui->cmbScale->addItem("100.000.000", QVariant((double)100000000));

  m_ui->cmbScale->blockSignals(false);
}

double te::layout::MapLayerChoiceOutside::mm2cm(double mmSize)
{
  return mmSize / 10.;
}

double  te::layout::MapLayerChoiceOutside::cm2mm(double cmSize)
{
  return cmSize * 10.;
}

void te::layout::MapLayerChoiceOutside::initBool(QWidget* widget, std::string nameComponent)
{
  MapLayerChoiceController* controller = dynamic_cast<MapLayerChoiceController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getProperty(nameComponent);
  if (prop.isNull())
    return;

  QCheckBox* chk = dynamic_cast<QCheckBox*>(widget);

  if (chk)
  {
    chk->blockSignals(true);
    chk->setChecked(te::layout::Property::GetValueAs<bool>(prop));
    chk->blockSignals(false);
  }
}

std::string  te::layout::MapLayerChoiceOutside::formatScaleValue(std::string inputValue)
{
  string formatedValue = inputValue;

  string formatedString = "";

  int size = (int)formatedValue.size();

  int indexSpace = 1;

  for (int i = size - 1; i >= 0; i--)
  {
    if (((indexSpace % 3) == 0) && i > 0){

      std::string tempBuffer = ".";
      tempBuffer += formatedValue[i];
      tempBuffer += formatedString;

      formatedString = tempBuffer;
    }
    else
    {
      formatedString = formatedValue[i] + formatedString;
    }
    indexSpace++;
  }
  return formatedString;
}

void te::layout::MapLayerChoiceOutside::keyPressEvent(QKeyEvent * e)
{
  /* Qt Doc: If the user presses the Esc key in a dialog, QDialog::reject() will be called.
  This will cause the window to close: The close event cannot be ignored. */
  if (e->key() != Qt::Key_Escape)
  {
    QDialog::keyPressEvent(e);
  }
  else 
  {
    e->ignore();
  }
}

