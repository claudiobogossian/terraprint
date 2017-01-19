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
#include "../../qt/outside/NorthSettingsController.h"
#include "../../core/property/NorthSettingsConfigProperties.h"
#include "../../core/property/Variant.h"
#include "../../core/enum/Enums.h"
#include "../core/ItemUtils.h"
#include "../../core/Constants.h"

// Qt
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QColorDialog>
#include <QLineEdit>
#include <QRadioButton>
#include <QVariant>
#include <QComboBox>


te::layout::NorthSettingsOutside::NorthSettingsOutside(AbstractOutsideController* controller, QWidget* parent)
  : QDialog(parent),
  AbstractOutsideView(controller),
  m_ui(new Ui::NorthSettings)
{
  m_ui->setupUi(this);

  init();

  /* In a QLineEdit, when you click the Enter button, an editFinished signal is triggered,
  however in a window there are buttons set as default, other events such as DynamicPropertyChange
  are sent to these buttons, and causes QLineEdit to lose focus for a short time.
  This causes the editingFinished to be called 2x, since QEditLine's "lost focus" also calls this method.
  To prevent such calls, no button is default in this window, just as it does not become default when clicked.
  By default the enter is to signal that the value has been modified, so no button should be default and get focus.*/
  m_ui->btnColor->setDefault(false);
  m_ui->btnColor->setAutoDefault(false);
  m_ui->helpPushButton->setDefault(false);
  m_ui->helpPushButton->setAutoDefault(false);
  m_ui->nBtnCancel->setDefault(false);
  m_ui->nBtnCancel->setAutoDefault(false);
  m_ui->nBtnOK->setDefault(false);
  m_ui->nBtnOK->setAutoDefault(false);
}

te::layout::NorthSettingsOutside::~NorthSettingsOutside()
{
  
}

void te::layout::NorthSettingsOutside::init()
{
    m_ui->colorNorth->setAutoFillBackground(true);
    m_ui->rdbCentimeters->setChecked(false);
    m_ui->rdbMillimeters->setChecked(true);
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

  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
  NorthSettingsModel* model = dynamic_cast<NorthSettingsModel*>(abstractModel);
  if (!model)
    return;

  initDouble(m_ui->lineEditNorthHeight, "height");
  initDouble(m_ui->lineEditNorthWidth, "width");
  initCombo(m_ui->cbNorth, "northArrow_type");
  initColor(m_ui->colorNorth, "color");

}

void te::layout::NorthSettingsOutside::on_nBtnOK_clicked()
{
  accept();
}

void te::layout::NorthSettingsOutside::on_nBtnCancel_clicked()
{
  reject();
}

void te::layout::NorthSettingsOutside::on_rdbMillimeters_clicked()
{
  NorthSettingsController* controller = dynamic_cast<NorthSettingsController*>(m_controller);
  if (controller)
  {
    Property propW = controller->getNorthProperty("width");
    double numberW = te::layout::Property::GetValueAs<double>(propW);

    Property propH = controller->getNorthProperty("height");
    double numberH = te::layout::Property::GetValueAs<double>(propH);


    QDoubleValidator* validator = new QDoubleValidator(0.0, 999999999.9, MILLIMETER_PRECISION, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    m_ui->lineEditNorthWidth->setText(QString::number(numberW));
    m_ui->lineEditNorthHeight->setText(QString::number(numberH));

    m_ui->lineEditNorthHeight->setValidator(validator);
    m_ui->lineEditNorthWidth->setValidator(validator);
  }
}

void te::layout::NorthSettingsOutside::on_rdbCentimeters_clicked()
{
  NorthSettingsController* controller = dynamic_cast<NorthSettingsController*>(m_controller);
  if (controller)
  {
    Property propW = controller->getNorthProperty("width");
    double numberW = te::layout::Property::GetValueAs<double>(propW);
    numberW = numberW / 10.; //convert to cm

    Property propH = controller->getNorthProperty("height");
    double numberH = te::layout::Property::GetValueAs<double>(propH);
    numberH = numberH / 10.; //convert to cm

    QDoubleValidator* validator = new QDoubleValidator(0.0, 999999999, CENTIMETER_PRECISION, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    m_ui->lineEditNorthWidth->setText(QString::number(numberW));
    m_ui->lineEditNorthHeight->setText(QString::number(numberH));

    m_ui->lineEditNorthHeight->setValidator(validator);
    m_ui->lineEditNorthWidth->setValidator(validator);
  }
}

void te::layout::NorthSettingsOutside::on_cbNorth_currentIndexChanged(const QString & text)
{
  NorthSettingsController* controller = dynamic_cast<NorthSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;

    std::string stdText = ItemUtils::convert2StdString(text);

    variant.setValue(stdText, dataType->getDataTypeStringList());
    Property prop = controller->getNorthProperty("northArrow_type");
    prop.setValue(stdText, dataType->getDataTypeStringList());
    prop.setOptionChoice(variant);
    emit updateProperty(prop);
  }
}

void te::layout::NorthSettingsOutside::on_lineEditNorthWidth_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lineEditNorthWidth->isModified())
  {
    return;
  }

  NorthSettingsController* controller = dynamic_cast<NorthSettingsController*>(m_controller);
  if (controller)
  {
    QString text = m_ui->lineEditNorthWidth->text();
    double width = text.toDouble();
    if (m_ui->rdbCentimeters->isChecked() == true)
    {
      width = width * 10.;
    }

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getNorthProperty("width");
    prop.setValue(width, dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lineEditNorthWidth->setModified(false);
  }
}

void te::layout::NorthSettingsOutside::on_lineEditNorthHeight_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lineEditNorthHeight->isModified())
  {
    return;
  }

  NorthSettingsController* controller = dynamic_cast<NorthSettingsController*>(m_controller);
  if (controller)
  {
    QString text = m_ui->lineEditNorthHeight->text();
    double height = text.toDouble();
    if (m_ui->rdbCentimeters->isChecked() == true)
    {
      height = height * 10.;
    }

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getNorthProperty("height");
    prop.setValue(height, dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lineEditNorthHeight->setModified(false);
  }
}

void te::layout::NorthSettingsOutside::on_btnColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->colorNorth);
  NorthSettingsController* controller = dynamic_cast<NorthSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getNorthProperty("color");
    prop.setValue(color, dataType->getDataTypeColor());
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

void te::layout::NorthSettingsOutside::initDouble(QWidget* widget, std::string nameComponent)
{
  NorthSettingsController* controller = dynamic_cast<NorthSettingsController*>(m_controller);
  if (!controller)
    return;

  std::ostringstream convert;
  convert.precision(15);
  Property prop = controller->getNorthProperty(nameComponent);
  double number = te::layout::Property::GetValueAs<double>(prop);
  convert << number;

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if (edit)
  {
    std::string txt = convert.str();
    QString qText = ItemUtils::convert2QString(txt);
    edit->setText(qText);
  }
}

void te::layout::NorthSettingsOutside::initBool(QWidget* widget, std::string nameComponent)
{
  NorthSettingsController* controller = dynamic_cast<NorthSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getNorthProperty(nameComponent);

  QRadioButton* chk = dynamic_cast<QRadioButton*>(widget);

  if (chk)
  {
    chk->setChecked(te::layout::Property::GetValueAs<bool>(prop));
  }
}

void te::layout::NorthSettingsOutside::initColor(QWidget* widget, std::string nameComponent)
{
  NorthSettingsController* controller = dynamic_cast<NorthSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getNorthProperty(nameComponent);

  te::color::RGBAColor color = te::layout::Property::GetValueAs<te::color::RGBAColor>(prop);
  QColor qcolor(color.getRed(), color.getGreen(), color.getBlue());

  if (!qcolor.isValid())
    return;

  if (!widget)
    return;

  QPalette paltt(widget->palette());
  paltt.setColor(widget->backgroundRole(), qcolor);
  widget->setPalette(paltt);
  widget->setAutoFillBackground(true);
}

void te::layout::NorthSettingsOutside::addComboOptions(QComboBox* combo, std::vector<Variant> options)
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

void te::layout::NorthSettingsOutside::initCombo(QWidget* widget, std::string nameComponent)
{
  NorthSettingsController* controller = dynamic_cast<NorthSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getNorthProperty(nameComponent);

  QComboBox* combo = dynamic_cast<QComboBox*>(widget);

  if (!combo)
    return;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  int index = -1;
  QVariant variant;
  
  if (prop.getType() == dataType->getDataTypeStringList())
  {
    std::string txt = prop.getOptionByCurrentChoice().toString();
    QString qText = ItemUtils::convert2QString(txt);
    variant.setValue(qText);
    if (combo->count() == 0)
    {
      addComboOptions(combo, prop.getOptionChoices());
    }
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
}

