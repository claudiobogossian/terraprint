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
  \file GridSettingsOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridSettingsOutside.h"
#include "ui_GridSettings.h"
#include "../../core/property/Variant.h"
#include "../../core/property/PlanarGridSettingsConfigProperties.h"
#include "../../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../../core/property/SharedProperties.h"
#include "../../core/enum/Enums.h"
#include "../../outside/GridSettingsModel.h"
#include "terralib/common/StringUtils.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../../core/Font.h"
#include "../core/ItemUtils.h"
#include "../outside/InputCoordDialog.h"
#include "GridSettingsController.h"
#include "../../core/Constants.h"
// STL
#include <string>
#include <sstream> 

// Qt
#include <QColorDialog> 
#include <QFrame>
#include <QLineEdit>
#include <QValidator>
#include <QComboBox>
#include <QCheckBox> 
#include <QMessageBox>
#include <QObjectList>

//boost
#include <boost/lexical_cast.hpp>

te::layout::GridSettingsOutside::GridSettingsOutside(AbstractOutsideController* controller, QWidget* parent) :
  QDialog(parent),
  AbstractOutsideView(controller),
  m_ui(new Ui::GridSettings),
  m_planarGridSettings(new PlanarGridSettingsConfigProperties),
  m_geodesicGridSettings(new GeodesicGridSettingsConfigProperties),
  m_lineWidthIncrement(0.5)
{
  m_ui->setupUi(this);

  m_ui->m_lineWidthDoubleSpinBox->setDecimals(MILLIMETER_PRECISION);
  m_ui->m_planarLineWidthDoubleSpinBox->setDecimals(MILLIMETER_PRECISION);
  
  /* In a QLineEdit, when you click the Enter button, an editFinished signal is triggered,
  however in a window there are buttons set as default, other events such as DynamicPropertyChange
  are sent to these buttons, and causes QLineEdit to lose focus for a short time.
  This causes the editingFinished to be called 2x, since QEditLine's "lost focus" also calls this method.
  To prevent such calls, no button is default in this window, just as it does not become default when clicked.
  By default the enter is to signal that the value has been modified, so no button should be default and get focus.*/
  m_ui->pbClose->setDefault(false);
  m_ui->pbClose->setAutoDefault(false);
  m_ui->helpPushButton->setDefault(false);
  m_ui->helpPushButton->setAutoDefault(false);
  m_ui->pbCornerTextGeoColor->setDefault(false);
  m_ui->pbCornerTextGeoColor->setAutoDefault(false);
  m_ui->pbGridTextGeoColor->setDefault(false);
  m_ui->pbGridTextGeoColor->setAutoDefault(false);
  m_ui->pbGridTextPlanarColor->setDefault(false);
  m_ui->pbGridTextPlanarColor->setAutoDefault(false);
  m_ui->pbLineColor->setDefault(false);
  m_ui->pbLineColor->setAutoDefault(false);
  m_ui->pbPlanarLineColor->setDefault(false);
  m_ui->pbPlanarLineColor->setAutoDefault(false);

  //removing this tab because it has not been implemented yet
  m_ui->tbwSettings->removeTab(2);

  init();
}

te::layout::GridSettingsOutside::~GridSettingsOutside()
{
  if(m_planarGridSettings)
  {
    delete m_planarGridSettings;
    m_planarGridSettings = 0;
  }

  if(m_geodesicGridSettings)
  {
    delete m_geodesicGridSettings;
    m_geodesicGridSettings = 0;
  }
}

void te::layout::GridSettingsOutside::init()
{
  m_ui->groupBox6_2_2->setVisible(false);
  m_ui->textLabel9_2_2->setVisible(false);
  m_ui->cmbCornerGeoFont->setVisible(false);
  m_ui->textLabel10_2_2->setVisible(false);
  m_ui->cmbCornerGeoTextSize->setVisible(false);
  m_ui->fraCornerTextGeoColor->setVisible(false);
  m_ui->pbCornerTextGeoColor->setVisible(false);

  m_ui->lneVrtPlanarDisplacement->setValidator(new  QDoubleValidator(this));
  m_ui->lneHrzPlanarDisplacement->setValidator(new  QDoubleValidator(this));

  m_ui->lneVrtGeoDisplacement->setValidator(new  QDoubleValidator(this));
  m_ui->lneHrzGeoDisplacement->setValidator(new  QDoubleValidator(this));

  m_ui->lneCornerHrzGeoDisplacement->setValidator(new  QDoubleValidator(this));
  m_ui->lneCornerVrtGeoDisplacement->setValidator(new  QDoubleValidator(this));

  m_ui->lneHrzPlanarGap->setValidator(new  QDoubleValidator(this));
  m_ui->lneVrtPlanarGap->setValidator(new  QDoubleValidator(this));

  m_ui->lneX1->setValidator(new  QDoubleValidator(this));
  m_ui->lneY1->setValidator(new  QDoubleValidator(this));
  m_ui->lneX2->setValidator(new  QDoubleValidator(this));
  m_ui->lneY2->setValidator(new  QDoubleValidator(this));
  m_ui->lneX3->setValidator(new  QDoubleValidator(this));
  m_ui->lneY3->setValidator(new  QDoubleValidator(this));
  m_ui->lneX4->setValidator(new  QDoubleValidator(this));
  m_ui->lneY4->setValidator(new  QDoubleValidator(this));
  m_ui->cmbScale->hide();
  m_ui->ckDefineScale->hide();

  m_ui->fraCornerTextGeoColor->setAutoFillBackground(true);
  m_ui->fraGridTextGeoColor->setAutoFillBackground(true);
  m_ui->fraGridTextPlanarColor->setAutoFillBackground(true);
  m_ui->fraPlanarLineColor->setAutoFillBackground(true);  

  double minimumValue = Utils::getLineWidthMinimumValue();
  m_ui->m_planarLineWidthDoubleSpinBox->setMinimum(minimumValue);
  m_ui->m_lineWidthDoubleSpinBox->setMinimum(minimumValue);
  
  m_ui->m_planarLineWidthDoubleSpinBox->setKeyboardTracking(false);
  m_ui->m_lineWidthDoubleSpinBox->setKeyboardTracking(false);

  // increment
  m_ui->m_planarLineWidthDoubleSpinBox->setSingleStep(m_lineWidthIncrement);
  m_ui->m_lineWidthDoubleSpinBox->setSingleStep(m_lineWidthIncrement);
}

void te::layout::GridSettingsOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::GridSettingsOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::GridSettingsOutside::blockComponents()
{
  QObjectList list = children();

  QList<QObject*>::Iterator it;
  for(it = list.begin() ; it != list.end() ; ++it)
  {
    QObject* obj = (*it);
    QWidget* w = dynamic_cast<QWidget*>(obj);
    if(w)
    {
      if(w->isVisible())
      {
        w->setEnabled(false);
      }
    }
  }
}

void te::layout::GridSettingsOutside::unblockComponents()
{
  QObjectList list = children();

  QList<QObject*>::Iterator it;
  for(it = list.begin() ; it != list.end() ; ++it)
  {
    QObject* obj = (*it);
    QWidget* w = dynamic_cast<QWidget*>(obj);
    if(w)
    {
      if(w->isVisible())
      {
        w->setEnabled(true);
      }
    }
  }
}

bool te::layout::GridSettingsOutside::checkValidDegreeValue(const QString &value)
{
  int                  degree = 0, minute = 0;
  float                second = 0;
  int                  status = 0;
  std::basic_string <char>::size_type index;

  std::string strDegree = ItemUtils::convert2StdString(value);
  if((index=strDegree.find("°")) !=std::string::npos)  
  {
    strDegree.replace(index,1,"");
  }
  if((index=strDegree.find("°")) !=std::string::npos)
  {
    strDegree.replace(index,1,"");
  }

  if((index=strDegree.find("'")) !=std::string::npos)  
  {
    strDegree.replace(index,1,"");
  }
  if((index=strDegree.find("'")) !=std::string::npos)  
  {
    strDegree.replace(index,1,"");
  }

  status=sscanf(strDegree.c_str(),"%d %d %f",&degree,&minute,&second);
  if(status!=3)  return false;

  return true;

}

te::color::RGBAColor te::layout::GridSettingsOutside::configColor( QWidget* widget )
{
  te::color::RGBAColor rgbaColor;

  QPalette ptt(widget->palette());
  QBrush brush = ptt.brush(widget->backgroundRole());

  QColor bColor = brush.color();
  rgbaColor.setColor(bColor.red(), bColor.green(), bColor.blue());

  QColor color = QColorDialog::getColor(brush.color(),this, tr("Color") );

  if(!color.isValid())  
    return rgbaColor;

  QPalette paltt(widget->palette());
  paltt.setColor(widget->backgroundRole(), color);
  widget->setPalette(paltt);
  widget->setAutoFillBackground(true);

  rgbaColor.setColor(color.red(), color.green(), color.blue());

  return rgbaColor;
}

void te::layout::GridSettingsOutside::load(const std::string& gridName)
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if (!controller)
    return;

  PlanarGridSettingsConfigProperties planarConfig;
  GeodesicGridSettingsConfigProperties geodesicConfig;
  if (gridName == planarConfig.getGridSettings())
  {
    m_ui->tabType->setCurrentIndex(0);
  }
  else if (gridName == geodesicConfig.getGridSettings())
  {
    m_ui->tabType->setCurrentIndex(1);
  }

  /* Grid */
  
  SharedProperties sharedProps;
  
  initCombo(m_ui->cmbUnit, m_planarGridSettings->getUnit(), 0);
  
  initBool(m_ui->chkShowPlanar, m_planarGridSettings->getVisible(), 0);

  initCombo(m_ui->cmbPlanarStyle, m_planarGridSettings->getStyle(), 0);

  initCombo(m_ui->cmbGridStyle, m_geodesicGridSettings->getStyle(), 0);
  
  initBool(m_ui->chkShowGeodesic, m_geodesicGridSettings->getVisible(), 0);

  ///* Line */
  
  initDouble(m_ui->lneHrzPlanarGap, m_planarGridSettings->getHorizontalLineGap(), 0);
  
  initDouble(m_ui->lneVrtPlanarGap, m_planarGridSettings->getVerticalLineGap(), 0);
  
  initDouble(m_ui->lneHorizontalGap, m_geodesicGridSettings->getHorizontalLineGap(), 0);
  
  initDouble(m_ui->lneVerticalGap, m_geodesicGridSettings->getVerticalLineGap(), 0);

  initColor(m_ui->fraPlanarLineColor, m_planarGridSettings->getLineColor(), 0);

  initCombo(m_ui->cmbPlanarLineType, m_planarGridSettings->getLineStyle(), 0);

  initCombo(m_ui->cmbLineType, m_geodesicGridSettings->getLineStyle(), 0);

  initDouble(m_ui->m_planarLineWidthDoubleSpinBox, m_planarGridSettings->getLineWidth(), 0);

  initDouble(m_ui->m_lineWidthDoubleSpinBox, m_geodesicGridSettings->getLineWidth(), 0);

  initColor(m_ui->fraLineColor, m_geodesicGridSettings->getLineColor(), 0);
  
  initInt(m_ui->lneSecPrecision, m_geodesicGridSettings->getSecondsPrecisionText(), 0);

  ///*Text: Basic Configuration*/

  initCombo(m_ui->cmbPlanarTextSize, m_planarGridSettings->getFont(), 0);

  initCombo(m_ui->cmbPlanarFont, m_planarGridSettings->getFont(), 0);

  initColor(m_ui->fraGridTextPlanarColor, m_planarGridSettings->getTextColor(), 0);

  initBool(m_ui->chkSuperscriptPlanarText, m_planarGridSettings->getSuperscriptText(), 0);

  initCombo(m_ui->cmbGeoFont, m_geodesicGridSettings->getFont(), 0);

  initCombo(m_ui->cmbGeoTextSize, m_geodesicGridSettings->getFont(), 0);

  initColor(m_ui->fraGridTextGeoColor, m_geodesicGridSettings->getTextColor(), 0);

  initBool(m_ui->chkSuperscriptGeoText, m_geodesicGridSettings->getSuperscriptText(), 0);

  ///*Text: Advanced configuration*/

  initDouble(m_ui->xGridInitialPoint_planar_textField, m_planarGridSettings->getVerticalLineInitial(), 0);

  initDouble(m_ui->yGridInitialPoint_planar_textField, m_planarGridSettings->getHorizontalLineInitial(), 0);

  initDouble(m_ui->xGridInitialPoint_geo_textField, m_geodesicGridSettings->getVerticalLineInitial(), 0);

  initDouble(m_ui->yGridInitialPoint_geo_textField, m_geodesicGridSettings->getHorizontalLineInitial(), 0);

  initBool(m_ui->chkBottomPlanarText, m_planarGridSettings->getBottomText(), 0);

  initBool(m_ui->chkLeftPlanarText, m_planarGridSettings->getLeftText(), 0);

  initBool(m_ui->chkRightPlanarText, m_planarGridSettings->getRightText(), 0);

  initBool(m_ui->chkTopPlanarText, m_planarGridSettings->getTopText(), 0);

  initBool(m_ui->chkBottomGeoText, m_geodesicGridSettings->getBottomText(), 0);

  initBool(m_ui->chkLeftGeoText, m_geodesicGridSettings->getLeftText(), 0);

  initBool(m_ui->chkRightGeoText, m_geodesicGridSettings->getRightText(), 0);

  initBool(m_ui->chkTopGeoText, m_geodesicGridSettings->getTopText(), 0);

  initBool(m_ui->chkBottomRotatePlanarText, m_planarGridSettings->getBottomRotateText(), 0);

  initBool(m_ui->chkLeftRotatePlanarText, m_planarGridSettings->getLeftRotateText(), 0);

  initBool(m_ui->chkRightRotatePlanarText, m_planarGridSettings->getRightRotateText(), 0);

  initBool(m_ui->chkTopRotatePlanarText, m_planarGridSettings->getTopRotateText(), 0);

  initBool(m_ui->chkBottomRotateGeoText, m_geodesicGridSettings->getBottomRotateText(), 0);

  initBool(m_ui->chkLeftRotateGeoText, m_geodesicGridSettings->getLeftRotateText(), 0);

  initBool(m_ui->chkRightRotateGeoText, m_geodesicGridSettings->getRightRotateText(), 0);

  initBool(m_ui->chkTopRotateGeoText, m_geodesicGridSettings->getTopRotateText(), 0);

  initDouble(m_ui->lneVrtPlanarDisplacement, m_planarGridSettings->getVerticalLineDisplacement(), 0);

  initBool(m_ui->chkVisibleTextsPlanarText, m_planarGridSettings->getVisibleAllTexts(), 0);

  initDouble(m_ui->lneHrzPlanarDisplacement, m_planarGridSettings->getHorizontalLineDisplacement(), 0);

  initDouble(m_ui->lneVrtGeoDisplacement, m_geodesicGridSettings->getVerticalLineDisplacement(), 0);

  initDouble(m_ui->lneHrzGeoDisplacement, m_geodesicGridSettings->getHorizontalLineDisplacement(), 0);

  initBool(m_ui->chkDegreesGeoText, m_geodesicGridSettings->getDegreesText(), 0);

  initBool(m_ui->chkMinutesGeoText, m_geodesicGridSettings->getMinutesText(), 0);

  initBool(m_ui->chkSecondsGeoText, m_geodesicGridSettings->getSecondsText(), 0);

  initBool(m_ui->chkVisibleTextsGeoText, m_geodesicGridSettings->getVisibleAllTexts(), 0);

  /* Gaps general configuration */

  initBool(m_ui->chkPlanarSyncGaps, m_planarGridSettings->getSyncGaps(), 0);

  initBool(m_ui->chkGeoSyncGaps, m_geodesicGridSettings->getSyncGaps(), 0);

  m_ui->chkDegreesGeoText->setChecked(true);
  setGeodesicValues();
}

void te::layout::GridSettingsOutside::setGeodesicValues()
{
  if (m_ui->chkDegreesGeoText->isChecked())
  {
    setGeodesicValues2GMS();
  }
  else
  {
    setGeodesicValues2Degrees();
  }
}

void te::layout::GridSettingsOutside::setGeodesicValues2GMS()
{
  QString y = m_ui->yGridInitialPoint_geo_textField->text();
  QString x = m_ui->xGridInitialPoint_geo_textField->text();
  setMask(m_ui->yGridInitialPoint_geo_textField, m_ui->xGridInitialPoint_geo_textField);
  m_ui->yGridInitialPoint_geo_textField->setText(ItemUtils::DD2DMS(y));
  m_ui->xGridInitialPoint_geo_textField->setText(ItemUtils::DD2DMS(x));

  y = m_ui->lneVerticalGap->text();
  x = m_ui->lneHorizontalGap->text();
  setMask(m_ui->lneVerticalGap, m_ui->lneHorizontalGap);
  m_ui->lneVerticalGap->setText(ItemUtils::DD2DMS(y));
  m_ui->lneHorizontalGap->setText(ItemUtils::DD2DMS(x));
}

void te::layout::GridSettingsOutside::setGeodesicValues2Degrees()
{
  QString y = m_ui->yGridInitialPoint_geo_textField->text();
  QString x = m_ui->xGridInitialPoint_geo_textField->text();
  setMask(m_ui->yGridInitialPoint_geo_textField, m_ui->xGridInitialPoint_geo_textField);
  m_ui->yGridInitialPoint_geo_textField->setText(ItemUtils::DMS2DD(y));
  m_ui->xGridInitialPoint_geo_textField->setText(ItemUtils::DMS2DD(x));

  y = m_ui->lneVerticalGap->text();
  x = m_ui->lneHorizontalGap->text();
  setMask(m_ui->lneVerticalGap, m_ui->lneHorizontalGap);
  m_ui->lneVerticalGap->setText(ItemUtils::DMS2DD(y));
  m_ui->lneHorizontalGap->setText(ItemUtils::DMS2DD(x));
}

void te::layout::GridSettingsOutside::setMask(QLineEdit *lat, QLineEdit *lon)
{
  if(m_ui->chkDegreesGeoText->isChecked() == false)
  {
    lat->setValidator(new QDoubleValidator(this));
    lon->setValidator(new QDoubleValidator(this));
  }
  else
  {
    QRegExp regExpLat("[\\+\\-]?[\\d]{1,2}°[\\d]{1,2}'[\\d]{1,2}.[\\d]{1,2}''");
    QValidator* validatorLat = new QRegExpValidator(regExpLat, 0);
    lat->setValidator(validatorLat);

    QRegExp regExpLong("[\\+\\-]?[\\d]{1,3}°[\\d]{1,2}'[ \\d]{1,2}.[ \\d]{1,2}''");
    QValidator* validatorLong = new QRegExpValidator(regExpLong, 0);
    lon->setValidator(validatorLong);
  }
}

void te::layout::GridSettingsOutside::on_pbClose_clicked()
{
  accept();
}

void te::layout::GridSettingsOutside::on_helpPushButton_clicked()
{

}

void te::layout::GridSettingsOutside::on_cmbUnit_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;

    std::string stdText = ItemUtils::convert2StdString(text);

    variant.setValue(stdText, dataType->getDataTypeString());
    Property prop = controller->getProperty(m_planarGridSettings->getUnit());
    prop.setValue(stdText, dataType->getDataTypeString());
    prop.setOptionChoice(variant);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkShowPlanar_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getVisible());
    prop.setValue(m_ui->chkShowPlanar->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarStyle_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;

    std::string stdText = ItemUtils::convert2StdString(text);

    variant.setValue(stdText, dataType->getDataTypeStringList());
    Property prop = controller->getProperty(m_planarGridSettings->getStyle());
    prop.setValue(stdText, dataType->getDataTypeStringList());
    prop.setOptionChoice(variant);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbGridStyle_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;

    std::string stdText = ItemUtils::convert2StdString(text);

    variant.setValue(stdText, dataType->getDataTypeStringList());
    Property prop = controller->getProperty(m_geodesicGridSettings->getStyle());
    prop.setValue(stdText, dataType->getDataTypeStringList());
    prop.setOptionChoice(variant);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkShowGeodesic_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getVisible());
    prop.setValue(m_ui->chkShowGeodesic->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneHrzPlanarGap_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneHrzPlanarGap->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getHorizontalLineGap());
    prop.setValue(m_ui->lneHrzPlanarGap->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    // refresh (sync both gap)
    initDouble(m_ui->lneVrtPlanarGap, m_planarGridSettings->getVerticalLineGap(), 0);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneHrzPlanarGap->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneVrtPlanarGap_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneVrtPlanarGap->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getVerticalLineGap());
    prop.setValue(m_ui->lneVrtPlanarGap->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    // refresh (sync both gap)
    initDouble(m_ui->lneHrzPlanarGap, m_planarGridSettings->getHorizontalLineGap(), 0);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneVrtPlanarGap->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneHorizontalGap_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneHorizontalGap->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    QString lneHorizontalGap = m_ui->lneHorizontalGap->text();
    if (m_ui->chkDegreesGeoText->isChecked())
    {
      lneHorizontalGap = ItemUtils::DMS2DD(lneHorizontalGap);
    }
    Property prop = controller->getProperty(m_geodesicGridSettings->getHorizontalLineGap());
    prop.setValue(lneHorizontalGap.toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    // refresh (sync both gap)
    initDouble(m_ui->lneVerticalGap, m_geodesicGridSettings->getVerticalLineGap(), 0);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneHorizontalGap->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneVerticalGap_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneVerticalGap->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    QString lneVerticalGap = m_ui->lneVerticalGap->text();
    std::string gapValue = ItemUtils::convert2StdString(lneVerticalGap);
    if (m_ui->chkDegreesGeoText->isChecked())
    {
      lneVerticalGap = ItemUtils::DMS2DD(lneVerticalGap);
    }
    Property prop = controller->getProperty(m_geodesicGridSettings->getVerticalLineGap());
    prop.setValue(lneVerticalGap.toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    // refresh (sync both gap)
    initDouble(m_ui->lneHorizontalGap, m_geodesicGridSettings->getHorizontalLineGap(), 0);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneVerticalGap->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneSecPrecision_editingFinished(){

  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneSecPrecision->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    QString secPrecision = m_ui->lneSecPrecision->text();
    Property prop = controller->getProperty(m_geodesicGridSettings->getSecondsPrecisionText());
    prop.setValue(secPrecision.toInt(), dataType->getDataTypeInt());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneSecPrecision->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_pbPlanarLineColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraPlanarLineColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getLineColor());
    prop.setValue(color, dataType->getDataTypeColor());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarLineType_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;

    std::string stdText = ItemUtils::convert2StdString(text);

    variant.setValue(stdText, dataType->getDataTypeStringList());
    Property prop = controller->getProperty(m_planarGridSettings->getLineStyle());
    prop.setValue(stdText, dataType->getDataTypeStringList());
    prop.setOptionChoice(variant);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_m_planarLineWidthDoubleSpinBox_valueChanged(double d)
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  { 
    double value = d;
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getLineWidth());
    prop.setValue(value, dataType->getDataTypeDouble());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbLineType_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;

    std::string stdText = ItemUtils::convert2StdString(text);

    variant.setValue(stdText, dataType->getDataTypeStringList());
    Property prop = controller->getProperty(m_geodesicGridSettings->getLineStyle());
    prop.setValue(stdText, dataType->getDataTypeStringList());
    prop.setOptionChoice(variant);
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_m_lineWidthDoubleSpinBox_valueChanged(double d)
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    double value = d;
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLineWidth());
    prop.setValue(value, dataType->getDataTypeDouble());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_pbLineColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraLineColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLineColor());
    prop.setValue(color, dataType->getDataTypeColor());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarTextSize_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    Property prop_font = controller->getProperty(m_planarGridSettings->getFont());
    if (!prop_font.isNull())
    {
      Font font = te::layout::Property::GetValueAs<Font>(prop_font);
      font.setPointSize(text.toInt());
 
      Property prop = controller->getProperty(m_planarGridSettings->getFont());
      prop.setValue(font, dataType->getDataTypeFont());
      emit updateProperty(prop);
    }
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarFont_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    Property prop_font = controller->getProperty(m_planarGridSettings->getFont());
    if (!prop_font.isNull())
    {
      Font font = te::layout::Property::GetValueAs<Font>(prop_font);

      std::string stdText = ItemUtils::convert2StdString(text);

      font.setFamily(stdText);

      Property prop = controller->getProperty(m_planarGridSettings->getFont());
      prop.setValue(font, dataType->getDataTypeFont());
      emit updateProperty(prop);
    }
  }
}

void te::layout::GridSettingsOutside::on_pbGridTextPlanarColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraGridTextPlanarColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getTextColor());
    prop.setValue(color, dataType->getDataTypeColor());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkSuperscriptPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getSuperscriptText());
    prop.setValue(m_ui->chkSuperscriptPlanarText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbGeoFont_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    Property prop_font = controller->getProperty(m_geodesicGridSettings->getFont());
    if (!prop_font.isNull())
    {
      Font font = te::layout::Property::GetValueAs<Font>(prop_font);

      std::string stdText = ItemUtils::convert2StdString(text);

      font.setFamily(stdText);

      Property prop = controller->getProperty(m_geodesicGridSettings->getFont());
      prop.setValue(font, dataType->getDataTypeFont());
      emit updateProperty(prop);
    }
  }
}

void te::layout::GridSettingsOutside::on_cmbGeoTextSize_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    Property prop_font = controller->getProperty(m_geodesicGridSettings->getFont());
    if (!prop_font.isNull())
    {
      Font font = te::layout::Property::GetValueAs<Font>(prop_font);
      font.setPointSize(text.toInt());

      Property prop = controller->getProperty(m_geodesicGridSettings->getFont());
      prop.setValue(font, dataType->getDataTypeFont());
      emit updateProperty(prop);
    }
  }
}

void te::layout::GridSettingsOutside::on_pbGridTextGeoColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraGridTextGeoColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getTextColor());
    prop.setValue(color, dataType->getDataTypeColor());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkSuperscriptGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getSuperscriptText());
    prop.setValue(m_ui->chkSuperscriptGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_xGridInitialPoint_planar_textField_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->xGridInitialPoint_planar_textField->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getVerticalLineInitial());
    prop.setValue(m_ui->xGridInitialPoint_planar_textField->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->xGridInitialPoint_planar_textField->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_yGridInitialPoint_planar_textField_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->yGridInitialPoint_planar_textField->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getHorizontalLineInitial());
    prop.setValue(m_ui->yGridInitialPoint_planar_textField->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->yGridInitialPoint_planar_textField->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_xGridInitialPoint_geo_textField_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->xGridInitialPoint_geo_textField->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    QString xGridInitialPoint_geo_textField = m_ui->xGridInitialPoint_geo_textField->text();
    if (m_ui->chkDegreesGeoText->isChecked())
    {
      xGridInitialPoint_geo_textField = ItemUtils::DMS2DD(xGridInitialPoint_geo_textField);
    }
    Property prop = controller->getProperty(m_geodesicGridSettings->getVerticalLineInitial());
    prop.setValue(xGridInitialPoint_geo_textField.toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->xGridInitialPoint_geo_textField->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_yGridInitialPoint_geo_textField_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->yGridInitialPoint_geo_textField->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    QString yGridInitialPoint_geo_textField = m_ui->yGridInitialPoint_geo_textField->text();
    if (m_ui->chkDegreesGeoText->isChecked())
    {
      yGridInitialPoint_geo_textField = ItemUtils::DMS2DD(yGridInitialPoint_geo_textField);
    }
    Property prop = controller->getProperty(m_geodesicGridSettings->getHorizontalLineInitial());
    prop.setValue(yGridInitialPoint_geo_textField.toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->yGridInitialPoint_geo_textField->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_chkBottomPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getBottomText());
    prop.setValue(m_ui->chkBottomPlanarText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkLeftPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getLeftText());
    prop.setValue(m_ui->chkLeftPlanarText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkRightPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getRightText());
    prop.setValue(m_ui->chkRightPlanarText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkTopPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getTopText());
    prop.setValue(m_ui->chkTopPlanarText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkBottomGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getBottomText());
    prop.setValue(m_ui->chkBottomGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkLeftGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLeftText());
    prop.setValue(m_ui->chkLeftGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkRightGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getRightText());
    prop.setValue(m_ui->chkRightGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkTopGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getTopText());
    prop.setValue(m_ui->chkTopGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkBottomRotatePlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getBottomRotateText());
    prop.setValue(m_ui->chkBottomRotatePlanarText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkLeftRotatePlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getLeftRotateText());
    prop.setValue(m_ui->chkLeftRotatePlanarText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkRightRotatePlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getRightRotateText());
    prop.setValue(m_ui->chkRightRotatePlanarText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkTopRotatePlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getTopRotateText());
    prop.setValue(m_ui->chkTopRotatePlanarText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkBottomRotateGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getBottomRotateText());
    prop.setValue(m_ui->chkBottomRotateGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkLeftRotateGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLeftRotateText());
    prop.setValue(m_ui->chkLeftRotateGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkRightRotateGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getRightRotateText());
    prop.setValue(m_ui->chkRightRotateGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkTopRotateGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getTopRotateText());
    prop.setValue(m_ui->chkTopRotateGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneVrtPlanarDisplacement_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneVrtPlanarDisplacement->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getVerticalLineDisplacement());
    prop.setValue(m_ui->lneVrtPlanarDisplacement->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneVrtPlanarDisplacement->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_chkVisibleTextsPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getVisibleAllTexts());
    prop.setValue(m_ui->chkVisibleTextsPlanarText->isChecked(), dataType->getDataTypeBool());

    bool opt = m_ui->chkVisibleTextsPlanarText->isChecked();
    m_ui->chkBottomPlanarText->setChecked(opt);
    m_ui->chkLeftPlanarText->setChecked(opt);
    m_ui->chkRightPlanarText->setChecked(opt);
    m_ui->chkTopPlanarText->setChecked(opt);

    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneHrzPlanarDisplacement_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneHrzPlanarDisplacement->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getHorizontalLineDisplacement());
    prop.setValue(m_ui->lneHrzPlanarDisplacement->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneHrzPlanarDisplacement->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneVrtGeoDisplacement_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneVrtGeoDisplacement->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getVerticalLineDisplacement());
    prop.setValue(m_ui->lneVrtGeoDisplacement->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneVrtGeoDisplacement->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneHrzGeoDisplacement_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneHrzGeoDisplacement->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getVerticalLineDisplacement());
    prop.setValue(m_ui->lneHrzGeoDisplacement->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneHrzGeoDisplacement->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_chkDegreesGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getDegreesText());
    prop.setValue(m_ui->chkDegreesGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
    setGeodesicValues();
  }
}

void te::layout::GridSettingsOutside::on_chkMinutesGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getMinutesText());
    prop.setValue(m_ui->chkMinutesGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkSecondsGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getSecondsText());
    prop.setValue(m_ui->chkSecondsGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkVisibleTextsGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getVisibleAllTexts());
    prop.setValue(m_ui->chkVisibleTextsGeoText->isChecked(), dataType->getDataTypeBool());

    bool opt = m_ui->chkVisibleTextsGeoText->isChecked();
    m_ui->chkBottomGeoText->setChecked(opt);
    m_ui->chkLeftGeoText->setChecked(opt);
    m_ui->chkRightGeoText->setChecked(opt);
    m_ui->chkTopGeoText->setChecked(opt);

    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbCornerGeoFont_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;

    std::string stdText = ItemUtils::convert2StdString(text);

    Property prop = controller->getProperty(m_geodesicGridSettings->getFontTextCorner());
    prop.setValue(stdText, dataType->getDataTypeString());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_cmbCornerGeoTextSize_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getPointTextSizeCorner());
    prop.setValue(text.toInt(), dataType->getDataTypeInt());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_pbCornerTextGeoColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraCornerTextGeoColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getTextColorCorner());
    prop.setValue(color, dataType->getDataTypeColor());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneCornerHrzGeoDisplacement_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneCornerHrzGeoDisplacement->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLneCornerHrzDisplacement());
    prop.setValue(m_ui->lneCornerHrzGeoDisplacement->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneCornerHrzGeoDisplacement->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneCornerVrtGeoDisplacement_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneCornerVrtGeoDisplacement->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLneCornerVrtDisplacement());
    prop.setValue(m_ui->lneCornerVrtGeoDisplacement->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneCornerVrtGeoDisplacement->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_chkLowerRightCornerGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLowerRightCornerText());
    prop.setValue(m_ui->chkLowerRightCornerGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkUpperRightCornerGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getUpperRightCornerText());
    prop.setValue(m_ui->chkUpperRightCornerGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkLowerLeftCornerGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLowerLeftCornerText());
    prop.setValue(m_ui->chkLowerLeftCornerGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkUpperLeftCornerGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getUpperLeftCornerText());
    prop.setValue(m_ui->chkUpperLeftCornerGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkVisibleCornerTextsGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getVisibleCornerTextsText());
    prop.setValue(m_ui->chkVisibleCornerTextsGeoText->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_ckDefineScale_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getDefineScale());
    prop.setValue(m_ui->ckDefineScale->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_chkPlanarSyncGaps_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_planarGridSettings->getSyncGaps());
    prop.setValue(m_ui->chkPlanarSyncGaps->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);

    // refresh
    if (m_ui->chkPlanarSyncGaps->isChecked())
    {
      initDouble(m_ui->lneVrtPlanarGap, m_planarGridSettings->getVerticalLineGap(), 0);
    }
  }
}

void te::layout::GridSettingsOutside::on_chkGeoSyncGaps_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if (controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getSyncGaps());
    prop.setValue(m_ui->chkGeoSyncGaps->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);

    // refresh
    if (m_ui->chkGeoSyncGaps->isChecked())
    {
      initDouble(m_ui->lneVerticalGap, m_geodesicGridSettings->getVerticalLineGap(), 0);
    }
  }
}

void te::layout::GridSettingsOutside::on_cmbScale_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getScale());
    prop.setValue(text.toInt(), dataType->getDataTypeString());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_ckbClip_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getClip());
    prop.setValue(m_ui->ckbClip->isChecked(), dataType->getDataTypeBool());
    emit updateProperty(prop);
  }
}

void te::layout::GridSettingsOutside::on_lneX1_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneX1->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLneX1());
    prop.setValue(m_ui->lneX1->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneX1->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneX2_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneX2->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLneX2());
    prop.setValue(m_ui->lneX2->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneX2->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneY1_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneY1->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLneY1());
    prop.setValue(m_ui->lneY1->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneY1->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneY2_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneY2->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLneY2());
    prop.setValue(m_ui->lneY2->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneY2->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneX3_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneX3->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLneX3());
    prop.setValue(m_ui->lneX3->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneX3->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneX4_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneX4->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLneX4());
    prop.setValue(m_ui->lneX4->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneX4->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneY3_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneY3->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLneY3());
    prop.setValue(m_ui->lneY3->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneY3->setModified(false);
  }
}

void te::layout::GridSettingsOutside::on_lneY4_editingFinished()
{
  /* Avoid executing unnecessary code in the editingFinished method
  when QLineEdit loses focus (the editingFinished is automatically
  called in the "lost focus") */
  if (!m_ui->lneY4->isModified())
  {
    return;
  }

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property prop = controller->getProperty(m_geodesicGridSettings->getLneY4());
    prop.setValue(m_ui->lneY4->text().toDouble(), dataType->getDataTypeDouble());
    emit updateProperty(prop);

    /* Avoid executing unnecessary code in the editingFinished method
    when QLineEdit loses focus */
    m_ui->lneY4->setModified(false);
  }
}

void te::layout::GridSettingsOutside::initString( QWidget* widget, std::string nameComponent, EnumType* gridType )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent);

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if(edit)
  {
    std::string txt = te::layout::Property::GetValueAs<std::string>(prop);
    QString qText = ItemUtils::convert2QString(txt);

    edit->setText(qText);
    return;
  }
}

void te::layout::GridSettingsOutside::initInt( QWidget* widget, std::string nameComponent, EnumType* gridType )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  std::ostringstream convert;
  Property prop = controller->getProperty(nameComponent);
  convert << te::layout::Property::GetValueAs<int>(prop);

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if(edit)
  {
    std::string txt = convert.str();
    QString qText = ItemUtils::convert2QString(txt);
    edit->setText(qText);
  }
}

void te::layout::GridSettingsOutside::initDouble( QWidget* widget, std::string nameComponent, EnumType* gridType )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if (!controller)
    return;

  std::ostringstream convert;
  convert.precision(15);
  Property prop = controller->getProperty(nameComponent);
  double number = te::layout::Property::GetValueAs<double>(prop);
  convert << number;

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if (edit)
  {
    std::string txt = convert.str();
    QString qText = ItemUtils::convert2QString(txt);
    edit->setText(qText);
  }
  else
  {
    QDoubleSpinBox* spin = dynamic_cast<QDoubleSpinBox*>(widget);
    if (spin)
    {
      spin->blockSignals(true);
      spin->setValue(number);
      spin->blockSignals(false);
    }
  }
}

void te::layout::GridSettingsOutside::initBool( QWidget* widget, std::string nameComponent, EnumType* gridType )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent);

  QCheckBox* chk = dynamic_cast<QCheckBox*>(widget);
  
  if(chk)
  {
    chk->setChecked(te::layout::Property::GetValueAs<bool>(prop));
  }
}

void te::layout::GridSettingsOutside::initColor( QWidget* widget, std::string nameComponent, EnumType* gridType )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent);
    
  const te::color::RGBAColor& color = te::layout::Property::GetValueAs<te::color::RGBAColor>(prop);
  QColor qcolor(color.getRed(), color.getGreen(), color.getBlue());

  if(!qcolor.isValid())  
    return;

  if(!widget)
    return;

  QPalette paltt(widget->palette());
  paltt.setColor(widget->backgroundRole(), qcolor);
  widget->setPalette(paltt);
  widget->setAutoFillBackground(true);
}

void te::layout::GridSettingsOutside::addComboOptions(QComboBox* combo, std::vector<Variant> options)
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

void te::layout::GridSettingsOutside::on_btnHorizontalGap_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getProperty(m_geodesicGridSettings->getHorizontalLineGap());

  QString currentValue = ItemUtils::convert2QString(boost::lexical_cast<std::string>(te::layout::Property::GetValueAs<double>(prop)));
  std::string stdCurrentValue = ItemUtils::convert2StdString(currentValue);
  InputCoordDialog degreeDialog(stdCurrentValue, 0, 180, this);

  if (degreeDialog.exec() == 1){

    QString lneHorizontalGap;
    lneHorizontalGap = ItemUtils::convert2QString(degreeDialog.getCoordvalueDMS());
    if (ItemUtils::convert2QString(degreeDialog.getCoordvalueDD()).toDouble() > 0.0)
    {
      m_ui->lneHorizontalGap->setText(lneHorizontalGap);

      EnumDataType* dataType = Enums::getInstance().getEnumDataType();

      double dValue = ItemUtils::convert2QString(degreeDialog.getCoordvalueDD()).toDouble();
      if (dValue > 0.0)
      {
        Property prop = controller->getProperty(m_geodesicGridSettings->getHorizontalLineGap());
        prop.setValue(dValue, dataType->getDataTypeDouble());
        emit updateProperty(prop);

        // refresh (sync both gap)
        Property propSyncGaps = controller->getProperty(m_geodesicGridSettings->getSyncGaps());
        bool syncGaps = te::layout::Property::GetValueAs<bool>(propSyncGaps);
        if (syncGaps)
        {
          m_ui->lneVerticalGap->blockSignals(true);
          m_ui->lneVerticalGap->setText(lneHorizontalGap);
          m_ui->lneVerticalGap->blockSignals(false);
        }
      }
    }
  }
}

void te::layout::GridSettingsOutside::on_btnVerticalGap_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getProperty(m_geodesicGridSettings->getVerticalLineGap());

  QString lneVerticalGap;

  QString currentValue = ItemUtils::convert2QString(prop.getValue()->toString());
  std::string stdCurrentValue = ItemUtils::convert2StdString(currentValue);
  InputCoordDialog degreeDialog(stdCurrentValue, 0, 90, this);
  if (degreeDialog.exec() == 1){
   
    lneVerticalGap = ItemUtils::convert2QString(degreeDialog.getCoordvalueDMS());

    if (ItemUtils::convert2QString(degreeDialog.getCoordvalueDD()).toDouble() > 0.0){

      m_ui->lneVerticalGap->setText(lneVerticalGap);

      EnumDataType* dataType = Enums::getInstance().getEnumDataType();

      double dValue = ItemUtils::convert2QString(degreeDialog.getCoordvalueDD()).toDouble();

      Property prop = controller->getProperty(m_geodesicGridSettings->getVerticalLineGap());
      prop.setValue(dValue, dataType->getDataTypeDouble());
      emit updateProperty(prop);

      // refresh (sync both gap)
      Property propSyncGaps = controller->getProperty(m_geodesicGridSettings->getSyncGaps());
      bool syncGaps = te::layout::Property::GetValueAs<bool>(propSyncGaps);
      if (syncGaps)
      {
        m_ui->lneHorizontalGap->blockSignals(true);
        m_ui->lneHorizontalGap->setText(lneVerticalGap);
        m_ui->lneHorizontalGap->blockSignals(false);
      }
    }
  }
}

void te::layout::GridSettingsOutside::on_btnInitialPointX_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getProperty(m_geodesicGridSettings->getVerticalLineInitial());

  QString lneIntialPointX;

  QString currentValue = ItemUtils::convert2QString(prop.getValue()->toString());
  std::string stdCurrentValue = ItemUtils::convert2StdString(currentValue);
  InputCoordDialog degreeDialog(stdCurrentValue, -179, 179, this);
  if (degreeDialog.exec() == 1){

    lneIntialPointX = ItemUtils::convert2QString(degreeDialog.getCoordvalueDMS());

    m_ui->xGridInitialPoint_geo_textField->setText(lneIntialPointX);

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    
    double dValue = ItemUtils::convert2QString(degreeDialog.getCoordvalueDD()).toDouble();

    Property prop = controller->getProperty(m_geodesicGridSettings->getVerticalLineInitial());
    prop.setValue(dValue, dataType->getDataTypeDouble());

    emit updateProperty(prop);

  }
}

void te::layout::GridSettingsOutside::on_btnInitialPointY_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if (!controller)
    return;

  Property prop = controller->getProperty(m_geodesicGridSettings->getHorizontalLineInitial());

  QString lneIntialPointY;

  QString currentValue = ItemUtils::convert2QString(prop.getValue()->toString());
  std::string stdCurrentValue = ItemUtils::convert2StdString(currentValue);
  InputCoordDialog degreeDialog(stdCurrentValue, -89, 89, this);
  if (degreeDialog.exec() == 1){

    lneIntialPointY = ItemUtils::convert2QString(degreeDialog.getCoordvalueDMS());

    m_ui->yGridInitialPoint_geo_textField->setText(lneIntialPointY);

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    double dValue = ItemUtils::convert2QString(degreeDialog.getCoordvalueDD()).toDouble();

    Property prop = controller->getProperty(m_geodesicGridSettings->getHorizontalLineInitial());
    prop.setValue(dValue, dataType->getDataTypeDouble());

    emit updateProperty(prop);

  }
}

void te::layout::GridSettingsOutside::initCombo( QWidget* widget, std::string nameComponent, EnumType* gridType )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent);

  QComboBox* combo = dynamic_cast<QComboBox*>(widget);

  if(!combo)
    return;

  combo->blockSignals(true);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  int index = -1;
  QVariant variant;

  if(prop.getType() == dataType->getDataTypeBool())
  {
    variant.setValue(te::layout::Property::GetValueAs<bool>(prop));
  }
  else if(prop.getType() == dataType->getDataTypeDouble())
  {
    variant.setValue(te::layout::Property::GetValueAs<double>(prop));
  }
  else if(prop.getType() == dataType->getDataTypeInt())
  {
    variant.setValue(te::layout::Property::GetValueAs<int>(prop));
  }
  else if(prop.getType() == dataType->getDataTypeString())
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
  else if (prop.getType() == dataType->getDataTypeFont())
  {
    if (widget->objectName().compare("cmbPlanarTextSize") == 0
      || widget->objectName().compare("cmbGeoTextSize") == 0
      || widget->objectName().compare("cmbCornerGeoTextSize") == 0)
    {
      int pointSize = te::layout::Property::GetValueAs<Font>(prop).getPointSize();
      variant.setValue(pointSize);
    }
    else if (widget->objectName().compare("cmbPlanarFont") == 0
      || widget->objectName().compare("cmbGeoFont") == 0
      || widget->objectName().compare("cmbCornerGeoFont") == 0)
    {
      std::string txt = te::layout::Property::GetValueAs<Font>(prop).getFamily();
      QString qText = ItemUtils::convert2QString(txt);

      variant.setValue(qText);
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
  if ( index == -1 ) 
  { 
    index = combo->findText(value);
    if(index != -1)
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

