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
  \file terralib/layout/qt/outside/LegendChoiceOutside.cpp

  \brief A dialog configure the input layer to address geocoding operation
*/

#include "LegendChoiceOutside.h"
#include "ui_LegendChoice.h"
#include "../../outside/LegendChoiceModel.h"
#include "../../outside/LegendChoiceController.h"
#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../../core/property/SharedProperties.h"
#include "../core/ItemUtils.h"

// TerraLib
#include "terralib/qt/widgets/utils/DoubleListWidget.h"

// STL
#include <string>

// Qt
#include <QGridLayout>
#include <QKeyEvent>
#include <QString>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QTextEdit>
#include <QColorDialog>
#include <QColor>
#include <QFont>
#include <QFontDialog>
#include <QComboBox>
#include <QVariant>
#include <QMessageBox>

te::layout::LegendChoiceOutside::LegendChoiceOutside(AbstractOutsideController* controller, QWidget* parent)
  : QDialog(parent),
  AbstractOutsideView(controller),
  m_ui(new Ui::LegendChoice)
{
  // add controls
  m_ui->setupUi(this);

  m_widget.reset(new te::qt::widgets::DoubleListWidget(m_ui->m_widget));

  QGridLayout* displayLayout = new QGridLayout(m_ui->m_widget);
  displayLayout->addWidget(m_widget.get());

  m_widget->setLeftLabel(tr("Available Layer"));
  m_widget->setRightLabel(tr("Selected Layer"));
  
  /* In a QLineEdit, when you click the Enter button, an editFinished signal is triggered,
  however in a window there are buttons set as default, other events such as DynamicPropertyChange
  are sent to these buttons, and causes QLineEdit to lose focus for a short time.
  This causes the editingFinished to be called 2x, since QEditLine's "lost focus" also calls this method.
  To prevent such calls, no button is default in this window, just as it does not become default when clicked.
  By default the enter is to signal that the value has been modified, so no button should be default and get focus.*/
  m_ui->m_okPushButton->setDefault(false);
  m_ui->m_okPushButton->setAutoDefault(false);
  m_ui->m_cancelPushButton->setDefault(false);
  m_ui->m_cancelPushButton->setAutoDefault(false);
  m_ui->helpPushButton->setDefault(false);
  m_ui->helpPushButton->setAutoDefault(false);
  m_ui->m_btnHierarchyFont->setAutoDefault(false);
  m_ui->m_btnHierarchyFontColor->setAutoDefault(false);
  m_ui->m_btnLayerTitleColor->setAutoDefault(false);
  m_ui->m_btnLayerTitleFont->setAutoDefault(false);

  if (m_ui->m_okPushButton && m_ui->m_cancelPushButton)
  {
    connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
    connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
  }
}

te::layout::LegendChoiceOutside::~LegendChoiceOutside()
{

}

void te::layout::LegendChoiceOutside::init()
{
  loadDoubleListWidget();

  load();
}

void te::layout::LegendChoiceOutside::onOkPushButtonClicked()
{
  Property prop = getSavedLayers();
  if (!prop.isNull())
  {
    emit updateProperty(prop);
    accept();
    return;
  }
  QMessageBox::information(this, tr("Legend Settings"), tr("No changes made."));
}

void te::layout::LegendChoiceOutside::onCancelPushButtonClicked()
{
  reject();
}

void te::layout::LegendChoiceOutside::setPosition(const double& x, const double& y)
{
  move(x, y);
  refresh();
}

te::gm::Coord2D te::layout::LegendChoiceOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

std::vector<std::string> te::layout::LegendChoiceOutside::intersectionLayersTitle(const std::vector<std::string>& output)
{
  std::vector <std::string> namesToInput;

  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
  LegendChoiceModel* model = dynamic_cast<LegendChoiceModel*>(abstractModel);
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

std::list<te::map::AbstractLayerPtr> te::layout::LegendChoiceOutside::getLayers()
{
  std::list<te::map::AbstractLayerPtr> currentLayers;
  if (!m_controller)
    return currentLayers;

  LegendChoiceController* controllerlayers = dynamic_cast<LegendChoiceController*>(m_controller);
  if (!controllerlayers)
    return currentLayers;

  return controllerlayers->getlistLayers();;
}

std::list<te::map::AbstractLayerPtr> te::layout::LegendChoiceOutside::getSelectedLayers()
{
  std::list<te::map::AbstractLayerPtr> currentLayers;
  LegendChoiceController* controllerlayers = dynamic_cast<LegendChoiceController*>(m_controller);
  if (!controllerlayers)
    return currentLayers;

  return controllerlayers->getSelectedlayers();
}

te::layout::Property te::layout::LegendChoiceOutside::getProperty(const std::string& name)
{
  te::layout::Property prop;
  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (!controller)
    return prop;

  return controller->getProperty(name);
}

void te::layout::LegendChoiceOutside::load()
{
  std::string nameProperty = "";

  SharedProperties sharedProps;
  initCombo(m_ui->m_cmbConnectedTo, sharedProps.getItemObserver());

  nameProperty = "offset_between_columns";
  m_ui->m_txtDisplacementBetweenColumns->setValidator(new  QDoubleValidator(this));
  initDouble(m_ui->m_txtDisplacementBetweenColumns, nameProperty);

  nameProperty = "displacement_between_symbols";
  m_ui->m_txtDisplacementBetweenSymbols->setValidator(new  QDoubleValidator(this));
  initDouble(m_ui->m_txtDisplacementBetweenSymbols, nameProperty);

  nameProperty = "displacement_between_symbols_and_texts";
  m_ui->m_txtDisplacementBetweenSymbolsAndTexts->setValidator(new  QDoubleValidator(this));
  initDouble(m_ui->m_txtDisplacementBetweenSymbolsAndTexts, nameProperty);

  nameProperty = "displacement_between_titles";
  m_ui->m_txtDisplacementBetweenTitles->setValidator(new  QDoubleValidator(this));
  initDouble(m_ui->m_txtDisplacementBetweenTitles, nameProperty);

  nameProperty = "border_displacement";
  m_ui->m_txtDisplacementBorder->setValidator(new  QDoubleValidator(this));
  initDouble(m_ui->m_txtDisplacementBorder, nameProperty);

  nameProperty = "symbol_size";
  m_ui->m_txtSymbolSize->setValidator(new  QDoubleValidator(this));
  initDouble(m_ui->m_txtSymbolSize, nameProperty);

  nameProperty = "grouping_offset_pair";
  m_ui->m_txtDisplacementHierarchyPair->setValidator(new  QDoubleValidator(this));
  initDouble(m_ui->m_txtDisplacementHierarchyPair, nameProperty);

  nameProperty = "rows";
  m_ui->m_txtRows->setValidator(new QIntValidator(this));
  initInt(m_ui->m_txtRows, nameProperty);

  nameProperty = "font_title";
  std::string propColor = "font_title_color";
  initTextEdit(m_ui->m_txtLayerFontTitle, nameProperty, propColor);

  nameProperty = "font_legend";
  propColor = "font_legend_color";
  initTextEdit(m_ui->m_txtHierarchyFont, nameProperty, propColor);

  nameProperty = "font_title_color";
  initColor(m_ui->m_lblLayerTitleColorFont, nameProperty);

  nameProperty = "font_legend_color";
  initColor(m_ui->m_lblHierarchyColorFont, nameProperty);
}

void te::layout::LegendChoiceOutside::on_m_btnLayerTitleFont_clicked()
{
  std::string nameProperty = "font_title";
  std::string propColor = "font_title_color";
  Font font = configFont(m_ui->m_txtLayerFontTitle, nameProperty, propColor);

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("font_title");
    prop.setValue(font, dataType->getDataTypeFont());
    emit updateProperty(prop);
    initTextEdit(m_ui->m_txtLayerFontTitle, nameProperty, propColor);
  }
}

void te::layout::LegendChoiceOutside::on_m_btnHierarchyFont_clicked()
{
  std::string nameProperty = "font_legend";
  std::string propColor = "font_legend_color";
  Font font = configFont(m_ui->m_txtHierarchyFont, nameProperty, propColor);

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("font_legend");
    prop.setValue(font, dataType->getDataTypeFont());
    emit updateProperty(prop);
    initTextEdit(m_ui->m_txtHierarchyFont, nameProperty, propColor);
  }
}

void te::layout::LegendChoiceOutside::on_m_btnLayerTitleColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->m_lblLayerTitleColorFont);

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("font_title_color");
    prop.setValue(color, dataType->getDataTypeColor());
    emit updateProperty(prop);
  }
}

void te::layout::LegendChoiceOutside::on_m_btnHierarchyFontColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->m_lblHierarchyColorFont);

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("font_legend_color");
    prop.setValue(color, dataType->getDataTypeColor());
    emit updateProperty(prop);
  }
}

void te::layout::LegendChoiceOutside::on_m_txtSymbolSize_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->m_txtSymbolSize->isModified())
  {
    return;
  }

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    double value = m_ui->m_txtSymbolSize->text().toDouble();

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("symbol_size");
    prop.setValue(value, dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->m_txtSymbolSize->setModified(false);
  }
}

void te::layout::LegendChoiceOutside::on_m_txtRows_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->m_txtRows->isModified())
  {
    return;
  }

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    int value = m_ui->m_txtRows->text().toInt();

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("rows");
    prop.setValue(value, dataType->getDataTypeInt());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->m_txtRows->setModified(false);
  }
}

void te::layout::LegendChoiceOutside::on_m_txtDisplacementBetweenSymbols_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->m_txtDisplacementBetweenSymbols->isModified())
  {
    return;
  }

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    double value = m_ui->m_txtDisplacementBetweenSymbols->text().toDouble();

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("displacement_between_symbols");
    prop.setValue(value, dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->m_txtDisplacementBetweenSymbols->setModified(false);
  }
}

void te::layout::LegendChoiceOutside::on_m_txtDisplacementBorder_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->m_txtDisplacementBorder->isModified())
  {
    return;
  }

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    double value = m_ui->m_txtDisplacementBorder->text().toDouble();

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("border_displacement");
    prop.setValue(value, dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->m_txtDisplacementBorder->setModified(false);
  }
}

void te::layout::LegendChoiceOutside::on_m_txtDisplacementBetweenTitles_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->m_txtDisplacementBetweenTitles->isModified())
  {
    return;
  }

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    double value = m_ui->m_txtDisplacementBetweenTitles->text().toDouble();

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("displacement_between_titles");
    prop.setValue(value, dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->m_txtDisplacementBetweenTitles->setModified(false);
  }
}

void te::layout::LegendChoiceOutside::on_m_txtDisplacementBetweenColumns_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->m_txtDisplacementBetweenColumns->isModified())
  {
    return;
  }

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    double value = m_ui->m_txtDisplacementBetweenColumns->text().toDouble();

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("offset_between_columns");
    prop.setValue(value, dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->m_txtDisplacementBetweenColumns->setModified(false);
  }
}

void te::layout::LegendChoiceOutside::on_m_txtDisplacementBetweenSymbolsAndTexts_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->m_txtDisplacementBetweenSymbolsAndTexts->isModified())
  {
    return;
  }

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    double value = m_ui->m_txtDisplacementBetweenSymbolsAndTexts->text().toDouble();

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("displacement_between_symbols_and_texts");
    prop.setValue(value, dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->m_txtDisplacementBetweenSymbolsAndTexts->setModified(false);
  }
}

void te::layout::LegendChoiceOutside::on_m_txtDisplacementHierarchyPair_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->m_txtDisplacementHierarchyPair->isModified())
  {
    return;
  }

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    double value = m_ui->m_txtDisplacementHierarchyPair->text().toDouble();

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty("grouping_offset_pair");
    prop.setValue(value, dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->m_txtDisplacementHierarchyPair->setModified(false);
  }
}

void te::layout::LegendChoiceOutside::on_m_cmbConnectedTo_currentIndexChanged(const QString & text)
{
  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (controller)
  {
    SharedProperties sharedProps;
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;

    std::string stdText = ItemUtils::convert2StdString(text);

    variant.setValue(stdText, dataType->getDataTypeItemObserver());
    Property prop = controller->getProperty(sharedProps.getItemObserver());
    prop.setValue(stdText, dataType->getDataTypeItemObserver());
    prop.setOptionChoice(variant);
    emit updateProperty(prop);
    loadDoubleListWidget(); // reload the double list widget with layers name
  }
}

void te::layout::LegendChoiceOutside::keyPressEvent(QKeyEvent * e)
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

void te::layout::LegendChoiceOutside::initDouble(QWidget* widget, const std::string& nameComponent)
{
  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (!controller)
    return;

  std::ostringstream convert;
  convert.precision(3);

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

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if (edit)
  {
    edit->blockSignals(true);
    edit->setText(convert.str().c_str());
    edit->blockSignals(false);
  }
}

void te::layout::LegendChoiceOutside::initCombo(QWidget* widget, const std::string& nameComponent)
{
  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
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

void te::layout::LegendChoiceOutside::addComboOptions(QComboBox* combo, std::vector<Variant> options)
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

void te::layout::LegendChoiceOutside::initInt(QWidget* widget, const std::string& nameComponent)
{
  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (!controller)
    return;

  std::ostringstream convert;
  Property prop = controller->getProperty(nameComponent);
  if (!prop.isNull())
  {
    convert << te::layout::Property::GetValueAs<int>(prop);

    QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
    if (edit)
    {
      std::string txt = convert.str();
      QString qText = ItemUtils::convert2QString(txt);
      edit->setText(qText);
    }
  }
}

void te::layout::LegendChoiceOutside::initColor(QWidget* widget, const std::string& nameComponent)
{
  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getProperty(nameComponent);
  if (!prop.isNull())
  {
    const te::color::RGBAColor& color = te::layout::Property::GetValueAs<te::color::RGBAColor>(prop);
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
}

void te::layout::LegendChoiceOutside::initTextEdit(QWidget* widget, const std::string& nameComponent, const std::string& nameFontComponent)
{
  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getProperty(nameComponent);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  QTextEdit* edit = dynamic_cast<QTextEdit*>(widget);
  if (edit)
  {
    int pointSize = 0;
    std::string family = "";

    if (prop.getType() == dataType->getDataTypeFont())
    {
      QString txt = fontHTML(prop, nameFontComponent);
      edit->setHtml(txt);
    }
    else
    {
      std::string txt = te::layout::Property::GetValueAs<std::string>(prop);
      QString qText = ItemUtils::convert2QString(txt);
      edit->setText(qText);
    }
  }
}

QString te::layout::LegendChoiceOutside::fontHTML(const Property& prop, const std::string& nameFontColorComponent)
{
  QString txt;

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (!controller)
    return txt;

  Property prop_font_color = controller->getProperty(nameFontColorComponent);

  int pointSize = te::layout::Property::GetValueAs<Font>(prop).getPointSize();
  std::string family = te::layout::Property::GetValueAs<Font>(prop).getFamily();

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

te::color::RGBAColor te::layout::LegendChoiceOutside::configColor(QWidget* widget)
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

te::layout::Font te::layout::LegendChoiceOutside::configFont(QWidget* widget, const std::string& nameFontComponent, const std::string& nameFontColorComponent)
{
  Font font;

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (!controller)
    return font;

  Property prop = controller->getProperty(nameFontComponent);

  font = te::layout::Property::GetValueAs<Font>(prop);
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
    QString txt = fontHTML(prop, nameFontColorComponent);
    edit->setHtml(txt);

  }

  return font;
}

te::layout::Font te::layout::LegendChoiceOutside::qFont2Font(QFont qFont)
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

QFont te::layout::LegendChoiceOutside::font2QFont(Font font)
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

te::layout::Property te::layout::LegendChoiceOutside::getSavedLayers()
{
  Property prop;
  std::vector<std::string>  vSelectedURIs;

  LegendChoiceController* controller = dynamic_cast<LegendChoiceController*>(m_controller);
  if (!controller)
    return prop;

  m_layersOnTheRight = m_widget->getOutputValues();

  std::list<te::map::AbstractLayerPtr> layerListMap = getLayers();

  std::vector<std::string>::iterator itString = m_layersOnTheRight.begin();
  for (; itString != m_layersOnTheRight.end(); ++itString)
  {
    std::list<te::map::AbstractLayerPtr>::iterator it = layerListMap.begin();
    for (; it != layerListMap.end(); ++it)
    {
      te::map::AbstractLayerPtr layer = it->get();
      std::string nameLayer = layer->getTitle();

      QString qNameLayer(nameLayer.c_str());
      nameLayer = ItemUtils::convert2StdString(qNameLayer);

      std::string name = (*itString);
      if (nameLayer.compare(name) == 0)
      {
        std::string uri = controller->searchLayerToURI(layer);
        vSelectedURIs.push_back(uri);
        m_layersSelected.push_back(layer);
      }
    }
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  prop = getProperty("visible_layers_uri");
  prop.setValue(vSelectedURIs, dataType->getDataTypeStringVector());

  m_layersSelected.clear();

  return prop;
}

void te::layout::LegendChoiceOutside::loadDoubleListWidget()
{
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
  LegendChoiceModel* model = dynamic_cast<LegendChoiceModel*>(abstractModel);
  if (!model)
  {
    return;
  }

  std::vector <std::string> namesToInput;
  std::vector <std::string> namesToOutput;

  // Layers From Map Items
  std::list<te::map::AbstractLayerPtr> selectedLayers = getSelectedLayers();

  // All Layers from Project
  std::list<te::map::AbstractLayerPtr> layers = getLayers();

  for (std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin(); it != selectedLayers.end(); ++it)
  {
    te::map::AbstractLayerPtr layer = (*it);
    if (std::find(layers.begin(), layers.end(), layer) != layers.end())
    {
      std::list<te::map::AbstractLayerPtr>::iterator findIt = std::find(layers.begin(), layers.end(), layer);
      namesToOutput.push_back(layer->getTitle());
    }
  }

  namesToInput = intersectionLayersTitle(namesToOutput);

  m_widget->blockSignals(true);
  m_widget->setInputValues(namesToInput);
  m_widget->setOutputValues(namesToOutput);
  m_widget->blockSignals(false);
}
