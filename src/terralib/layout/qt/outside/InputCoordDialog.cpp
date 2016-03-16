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



#include "InputCoordDialog.h" 
#include "LayoutDoubleValidator.h"
#include "ui_InputCoordWidgetForm.h"
#include "../core/ItemUtils.h"

//boost
#include <boost/lexical_cast.hpp>


te::layout::InputCoordDialog::InputCoordDialog(std::string& coord, int bottom, int top, QWidget* parent) :
QDialog(parent), m_ui(new Ui::InputCoordWidgetForm), inputValue(coord), degreeBottom(bottom), degreeTop(top)
{
  m_ui->setupUi(this);
  init();
}

te::layout::InputCoordDialog::~InputCoordDialog(){

}

void te::layout::InputCoordDialog::init()
{

  m_ui->lneDegree->setValidator(new QIntValidator(degreeBottom, degreeTop, m_ui->lneDegree));
  
  m_ui->lneMinute->setValidator(new QIntValidator(0, 59, m_ui->lneMinute));
  LayoutDoubleValidator* validatorDouble = new LayoutDoubleValidator(0.0, 59.999, 3, m_ui->lneSecond);

  validatorDouble->setNotation(QDoubleValidator::StandardNotation);
  m_ui->lneSecond->setValidator(validatorDouble);

  double doubleBottom = (double) degreeBottom;
  if (doubleBottom < 0.0)
  {
    doubleBottom = doubleBottom - 0.999999;
  }

  double doubleTop = degreeTop + 0.999999;


  m_ui->lneDecimalDegree->setValidator(new QDoubleValidator(doubleBottom, doubleTop, 12, m_ui->lneDecimalDegree));

  if (m_ui->rdoDMS->isChecked())
  {
    m_ui->lneDecimalDegree->setReadOnly(true);

    m_ui->lneDegree->setReadOnly(false);
    m_ui->lneMinute->setReadOnly(false);
    m_ui->lneSecond->setReadOnly(false);
  }
  else
  {
    m_ui->lneDecimalDegree->setReadOnly(false);

    m_ui->lneDegree->setReadOnly(true);
    m_ui->lneMinute->setReadOnly(true);
    m_ui->lneSecond->setReadOnly(true);

  }

  QString convertedValue = ItemUtils::convert2QString(inputValue);

  if (convertedValue.toDouble() > 360.0)
  {
    m_ui->lneDecimalDegree->setText("0.00");
  }
  else
  {
    QString formatting = QString::number(ItemUtils::convert2QString(inputValue).toDouble(), 'f', 12);
    m_ui->lneDecimalDegree->setText(formatting);
  }

  QString dms = ItemUtils::DD2DMS(ItemUtils::convert2QString(inputValue));

  QChar degreeSymbol('°');
  QChar minuteSymbol('\'');
  QChar secondsSymbol('\'');


  int a = dms.indexOf(degreeSymbol);
  int b = dms.indexOf(minuteSymbol);
  int c = dms.indexOf(secondsSymbol, b + 1);
  double deg = dms.mid(0, a - 0).replace(QString(" "), QString("")).toDouble();
  double mins = dms.mid(a + 1, b - a - 1).replace(QString(" "), QString("")).toDouble();
  double secs = dms.mid(b + 1, c - b - 1).replace(QString(" "), QString("")).toDouble();
    

  if (deg > 360){
    deg = 0;
  }
  m_ui->lneDegree->setText(ItemUtils::convert2QString(boost::lexical_cast<std::string>(deg)));
  degree = (int)deg;
  m_ui->lneMinute->setText(ItemUtils::convert2QString(boost::lexical_cast<std::string>(mins)));
  min = (int)mins;
  
  QString formatting = QString::number(secs, 'f', 3);

  if (formatting.toDouble() >= 60.0 ){
    formatting = "59.999";
  }

  m_ui->lneSecond->setText(formatting);
  sec = secs;


}

void te::layout::InputCoordDialog::on_lneDegree_editingFinished()
{
  if (m_ui->lneDegree->text().isEmpty())
  {
    m_ui->lneDegree->setText("0");
  }
  else
  {
    degree = m_ui->lneDegree->text().toInt();
  }

  std::string degreeSymbol = "°";
  std::string minuteSymbol = "\'";
  std::string secondsSymbol = "\'";

  QString dms = ItemUtils::DMS2DD(ItemUtils::convert2QString(boost::lexical_cast<std::string>(degree)+degreeSymbol + boost::lexical_cast<std::string>(min)+minuteSymbol + boost::lexical_cast<std::string>(sec)+secondsSymbol));
  m_ui->lneDecimalDegree->setText(dms);

}

void te::layout::InputCoordDialog::on_lneMinute_editingFinished()
{
  if (m_ui->lneMinute->text().isEmpty())
  {
    m_ui->lneMinute->setText("0");
  }
  else
  {
    min = m_ui->lneMinute->text().toInt();
  }

  std::string degreeSymbol = "°";
  std::string minuteSymbol = "\'";
  std::string secondsSymbol = "\'";

  QString dms = ItemUtils::DMS2DD(ItemUtils::convert2QString(boost::lexical_cast<std::string>(degree)+degreeSymbol + boost::lexical_cast<std::string>(min)+minuteSymbol + boost::lexical_cast<std::string>(sec)+secondsSymbol)); 
  m_ui->lneDecimalDegree->setText(dms);
  
}

void te::layout::InputCoordDialog::on_lneSecond_editingFinished()
{
  if (m_ui->lneSecond->text().isEmpty())
  {
    m_ui->lneSecond->setText("0");
  }
  else
  {
    sec = m_ui->lneSecond->text().toDouble();
  }

  std::string degreeSymbol = "°";
  std::string minuteSymbol = "\'" ;
  std::string secondsSymbol = "\'";

  QString dms = ItemUtils::DMS2DD(ItemUtils::convert2QString(boost::lexical_cast<std::string>(degree)+degreeSymbol + boost::lexical_cast<std::string>(min)+minuteSymbol + boost::lexical_cast<std::string>(sec)+secondsSymbol));
  m_ui->lneDecimalDegree->setText(dms);

}

void te::layout::InputCoordDialog::on_lneDecimalDegree_editingFinished()
{
  if (m_ui->lneDecimalDegree->text().isEmpty())
  {
    m_ui->lneDecimalDegree->setText("0");
  }
  else
  {
    decimalD = m_ui->lneDecimalDegree->text().toDouble();
  }

}

void te::layout::InputCoordDialog::on_pbOK_clicked()
{
  decimalD = m_ui->lneDecimalDegree->text().toDouble();
  QString formatting = QString::number(sec, 'f', 3);
  outPutValueDMS = boost::lexical_cast<std::string>(degree)+"°" + boost::lexical_cast<std::string>(min)+"'" + formatting.toLatin1().toStdString() + "''";
  outPutValueDD = boost::lexical_cast<std::string>(decimalD);

  this->accept();
}

std::string te::layout::InputCoordDialog::getCoordvalueDMS(){
  return outPutValueDMS;
}

std::string te::layout::InputCoordDialog::getCoordvalueDD(){
  return outPutValueDD;
}


void te::layout::InputCoordDialog::on_rdoDD_clicked()
{


  m_ui->lneDecimalDegree->setReadOnly(false);

  m_ui->lneDegree->setReadOnly(true);
  m_ui->lneMinute->setReadOnly(true);
  m_ui->lneSecond->setReadOnly(true);

}

void te::layout::InputCoordDialog::on_rdoDMS_clicked()
{

  m_ui->lneDecimalDegree->setReadOnly(true);

  m_ui->lneDegree->setReadOnly(false);
  m_ui->lneMinute->setReadOnly(false);
  m_ui->lneSecond->setReadOnly(false);

}

void te::layout::InputCoordDialog::on_pbCancel_clicked(){
  this->reject();
}