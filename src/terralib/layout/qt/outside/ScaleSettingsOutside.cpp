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
#include "../../core/property/SharedProperties.h"
#include "ScaleSettingsController.h"
#include "../core/ItemUtils.h"
#include "ui_ScaleSettings.h"

// Qt
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QValidator>
#include <QComboBox>
#include <QCheckBox> 
#include <QFontDialog>

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
  
}

void te::layout::ScaleSettingsOutside::load()
{
  SharedProperties sharedProps;

  initCombo(m_ui->cmbConnectedTo, sharedProps.getItemObserver());
  initCombo(m_ui->cmbType, "scale_type");
  initCombo(m_ui->cmbUnit, "Unit");
  initBool(m_ui->chkOnlyFirstAndLastValue, "only_first_and_last_value");
  initDouble(m_ui->txtScale, "scale");
  initInt(m_ui->txtNumberOfBreaks, "number_of_breaks");
  initDouble(m_ui->txtScaleGapX, "scale_width_rect_gap");
  initDouble(m_ui->txtScaleGapY, "scale_height_rect_gap");
  initDouble(m_ui->txtScaleGapXInUnit, "scale_in_unit_width_rect_gap");
  initTextEdit(m_ui->txtFont, "font");
}

void te::layout::ScaleSettingsOutside::setPosition(const double& x, const double& y)
{
  move(x, y);
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

void te::layout::ScaleSettingsOutside::on_pBtnCancel_clicked()
{
  reject();
}

void te::layout::ScaleSettingsOutside::on_pbtnFont_clicked()
{
  Font font = configFont(m_ui->txtFont);
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(font, dataType->getDataTypeFont());
    Property prop = controller->getScaleProperty("font");
    prop.setValue(variant);
    emit updateProperty(prop);
    initTextEdit(m_ui->txtFont, "font");
  }
}

void te::layout::ScaleSettingsOutside::on_cmbConnectedTo_currentIndexChanged(const QString & text)
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (controller)
  {
    SharedProperties sharedProps;
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;

    std::string stdText = ItemUtils::convert2StdString(text);

    variant.setValue(stdText, dataType->getDataTypeItemObserver());
    Property prop = controller->getScaleProperty(sharedProps.getItemObserver());
    prop.setValue(variant);
    prop.setOptionChoice(variant);
    emit updateProperty(prop);
    initDouble(m_ui->txtScale, "scale");
  }
}

void te::layout::ScaleSettingsOutside::on_cmbType_currentIndexChanged(const QString & text)
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;

    std::string stdText = ItemUtils::convert2StdString(text);

    variant.setValue(stdText, dataType->getDataTypeStringList());
    Property prop = controller->getScaleProperty("scale_type");
    prop.setValue(variant);
    prop.setOptionChoice(variant);
    emit updateProperty(prop);
  }
}

void te::layout::ScaleSettingsOutside::on_cmbUnit_currentIndexChanged(const QString & text)
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;

    std::string stdText = ItemUtils::convert2StdString(text);

    variant.setValue(stdText, dataType->getDataTypeStringList());
    Property prop = controller->getScaleProperty("Unit");
    prop.setValue(variant);
    prop.setOptionChoice(variant);
    emit updateProperty(prop);
    initDouble(m_ui->txtScaleGapXInUnit, "scale_in_unit_width_rect_gap");
  }
}

void te::layout::ScaleSettingsOutside::on_chkOnlyFirstAndLastValue_clicked()
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->chkOnlyFirstAndLastValue->isChecked(), dataType->getDataTypeBool());
    Property prop = controller->getScaleProperty("only_first_and_last_value");
    prop.setValue(variant);
    emit updateProperty(prop);
  }
}

void te::layout::ScaleSettingsOutside::on_txtScaleGapXInUnit_editingFinished()
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->txtScaleGapXInUnit->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->getScaleProperty("scale_in_unit_width_rect_gap");
    prop.setValue(variant);
    emit updateProperty(prop);
    initDouble(m_ui->txtScaleGapX, "scale_width_rect_gap");
  }
}

void te::layout::ScaleSettingsOutside::on_txtScaleGapX_editingFinished()
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->txtScaleGapX->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->getScaleProperty("scale_width_rect_gap");
    prop.setValue(variant);
    emit updateProperty(prop);
    initDouble(m_ui->txtScaleGapXInUnit, "scale_in_unit_width_rect_gap");
  }
}

void te::layout::ScaleSettingsOutside::on_txtScaleGapY_editingFinished()
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->txtScaleGapY->text().toDouble(), dataType->getDataTypeDouble());
    Property prop = controller->getScaleProperty("scale_height_rect_gap");
    prop.setValue(variant);
    emit updateProperty(prop);
  }
}

void te::layout::ScaleSettingsOutside::on_txtNumberOfBreaks_editingFinished()
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->txtNumberOfBreaks->text().toDouble(), dataType->getDataTypeInt());
    Property prop = controller->getScaleProperty("number_of_breaks");
    prop.setValue(variant);
    emit updateProperty(prop);
  }
}

void te::layout::ScaleSettingsOutside::initString(QWidget* widget, std::string nameComponent)
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getScaleProperty(nameComponent);

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if (edit)
  {
    std::string txt = prop.getValue().toString();
    QString qText = ItemUtils::convert2QString(txt);

    edit->setText(qText);
    return;
  }
}

void te::layout::ScaleSettingsOutside::initInt(QWidget* widget, std::string nameComponent)
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (!controller)
    return;

  std::ostringstream convert;
  Property prop = controller->getScaleProperty(nameComponent);
  convert << prop.getValue().toInt();

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if (edit)
  {
    std::string txt = convert.str();
    QString qText = ItemUtils::convert2QString(txt);
    edit->setText(qText);
  }
}

void te::layout::ScaleSettingsOutside::initDouble(QWidget* widget, std::string nameComponent)
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (!controller)
    return;

  std::ostringstream convert;
  convert.precision(15);
  Property prop = controller->getScaleProperty(nameComponent);
  double number = prop.getValue().toDouble();
  convert << number;

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if (edit)
  {
    std::string txt = convert.str();
    QString qText = ItemUtils::convert2QString(txt);
    edit->setText(qText);
  }
}

void te::layout::ScaleSettingsOutside::initBool(QWidget* widget, std::string nameComponent)
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getScaleProperty(nameComponent);

  QCheckBox* chk = dynamic_cast<QCheckBox*>(widget);

  if (chk)
  {
    chk->setChecked(prop.getValue().toBool());
  }
}

void te::layout::ScaleSettingsOutside::initCombo(QWidget* widget, std::string nameComponent)
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getScaleProperty(nameComponent);

  QComboBox* combo = dynamic_cast<QComboBox*>(widget);

  if (!combo)
    return;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  int index = -1;
  QVariant variant;

  if (prop.getType() == dataType->getDataTypeBool())
  {
    variant.setValue(prop.getValue().toBool());
  }
  else if (prop.getType() == dataType->getDataTypeDouble())
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

    std::string txt = prop.getValue().toString();
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
}

void te::layout::ScaleSettingsOutside::initTextEdit(QWidget* widget, std::string nameComponent)
{
  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getScaleProperty(nameComponent);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  QTextEdit* edit = dynamic_cast<QTextEdit*>(widget);
  if (edit)
  {
    int pointSize = 0;
    std::string family = "";

    if (prop.getType() == dataType->getDataTypeFont())
    {
      if (widget->objectName().compare("txtFont") == 0)
      {
        QString txt = fontHTML(prop);
        edit->setHtml(txt);
      }
    }
    else
    {
      std::string txt = prop.getValue().toString();
      QString qText = ItemUtils::convert2QString(txt);
      edit->setText(qText);
    }
  }
}

void te::layout::ScaleSettingsOutside::addComboOptions(QComboBox* combo, std::vector<Variant> options)
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

QString te::layout::ScaleSettingsOutside::fontHTML(const Property& prop)
{
  QString txt;

  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (!controller)
    return txt;

  Property prop_font_color = controller->getScaleProperty("font_color");

  int pointSize = prop.getValue().toFont().getPointSize();
  std::string family = prop.getValue().toFont().getFamily();

  QString qPointSize = QString::number(pointSize);
  QString qFamily = ItemUtils::convert2QString(family);

  // First piece
  QString colorFontHtml = "<font color=\"black\" face=\" " + qFamily + "\" size=\"3\">";
  QString txtFirst = "A";
  QString endHtml = "</font>";

  // Second piece
  QString secondColorFontHtml = "<font color=\"black\" face=\"Arial\" size=\"3\">";
  QString secondTxt = "[" + qFamily + "," + qPointSize + "]";
  QString secondEndHtml = "</font>";

  txt = colorFontHtml + txtFirst + endHtml;
  txt += " " + secondColorFontHtml + secondTxt + secondEndHtml;
  return txt;
}

te::layout::Font te::layout::ScaleSettingsOutside::configFont(QWidget* widget)
{
  Font font;

  ScaleSettingsController* controller = dynamic_cast<ScaleSettingsController*>(m_controller);
  if (!controller)
    return font;

  Property prop = controller->getScaleProperty("font");

  font = prop.getValue().toFont();
  QFont qFont = font2QFont(font);

  bool ok = false;
  QFont newFont = QFontDialog::getFont(&ok, qFont, this);

  if (ok) {
    // the user clicked OK and font is set to the font the user selected
    font = qFont2Font(newFont);
  }

  QTextEdit* edit = dynamic_cast<QTextEdit*>(widget);
  if (edit)
  {
    edit->clear();
    QString txt = fontHTML(prop);
    edit->setHtml(txt);
    
  }

  return font;
}

te::layout::Font te::layout::ScaleSettingsOutside::qFont2Font(QFont qFont)
{
  Font font;

  QString qFontName = qFont.family();
  std::string fontName = ItemUtils::convert2StdString(qFontName);

  font.setFamily(fontName);
  font.setPointSize(qFont.pointSize());
  font.setBold(qFont.bold());
  font.setItalic(qFont.italic());
  font.setUnderline(qFont.underline());
  font.setStrikeout(qFont.strikeOut());
  font.setKerning(qFont.kerning());

  return font;
}

QFont te::layout::ScaleSettingsOutside::font2QFont(Font font)
{
  int pointSize = font.getPointSize();
  std::string family = font.getFamily();
  bool bold = font.isBold();
  bool italic = font.isItalic();
  bool kerning = font.isKerning();
  bool strikeOut = font.isStrikeout();
  bool underline = font.isUnderline();

  QString qFamily = ItemUtils::convert2QString(family);

  QFont qFont(qFamily, pointSize);
  qFont.setBold(bold);
  qFont.setItalic(italic);
  qFont.setKerning(kerning);
  qFont.setStrikeOut(strikeOut);
  qFont.setUnderline(underline);

  return qFont;
}

