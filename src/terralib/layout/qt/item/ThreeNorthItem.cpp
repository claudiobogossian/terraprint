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
  \file ThreeNorthItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ThreeNorthItem.h"
#include "terralib/color/RGBAColor.h"
#include "../core/Scene.h"
#include "../core/ItemUtils.h"
// Qt
#include <QPointF>
#include "qglobal.h"
#include "qmatrix.h"
#include "boost/array.hpp"
#include "qtransform.h"


// TerraLib
#include "ThreeNorthItem.h"
#include "../../item/ThreeNorthModel.h"
#include "terralib/common/StringUtils.h"

te::layout::ThreeNorthItem::ThreeNorthItem(AbstractItemController* controller, bool invertedMatrix) :
AbstractItem<QGraphicsItem>(controller, invertedMatrix)
{  

}

te::layout::ThreeNorthItem::~ThreeNorthItem()
{

}


void te::layout::ThreeNorthItem::drawItem(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    drawTrueNorth(painter);
    drawArc(painter);

    double angleMN =0;
    double angleMC = 0;

    getAngles(angleMN, angleMC);


    if (m_controller->getProperty("magnetic_north").getValue().toBool() == true){
      drawMagneticNorth(painter, angleMN);
  }


    if (m_controller->getProperty("meridian_convergence").getValue().toBool() == true)
    {
      drawMeridianConvergence(painter, angleMC);
    }
}

void te::layout::ThreeNorthItem::getAngles(double &angleMN, double &angleMC){

    double angle_magnetic = m_controller->getProperty("angle_magnetic_north").getValue().toDouble();
    double angle_meridian = m_controller->getProperty("angle_meridian_convergence").getValue().toDouble();

    if (m_controller->getProperty("meridian_convergence").getValue().toBool() == true && m_controller->getProperty("magnetic_north").getValue().toBool() == true){
      if (angle_meridian == 0 && angle_magnetic == 0)
      {
        return;
      }
      else{

        if (angle_meridian < 0 && angle_magnetic > 0){
          if (angle_meridian < angle_magnetic){
            angleMN = 10;
            angleMC = -10;
          }
          else{
            angleMN = -10;
            angleMC = 10;
          }
        }
        else if (angle_meridian > 0 && angle_magnetic>0){

          if (angle_meridian < angle_magnetic){
            angleMN = 10;
            angleMC = -10;
          }
          else{
            angleMN = -10;
            angleMC = 10;
          }
        }
        else if (angle_meridian < 0 && angle_magnetic > 0){
          if (angle_meridian < angle_magnetic){
            angleMN = -20;
            angleMC = -10;
          }
          else{
            angleMN = -10;
            angleMC = -20;
          }
        }
        else if (angle_meridian > 0 && angle_magnetic>0){
          if (angle_meridian > angle_magnetic){
            angleMN = 10;
            angleMC = 20;
          }
          else{
            angleMN = 20;
            angleMC = 10;
          }
        }

        else if (angle_meridian > 0 && angle_magnetic < 0){
          if (angle_meridian > angle_magnetic){
            angleMN = -10;
            angleMC = 10;
          }
          else{
            angleMN = 10;
            angleMC = -10;
          }
        }
        else if (angle_meridian < 0 && angle_magnetic < 0){
          if (angle_meridian < angle_magnetic){
            angleMN = -10;
            angleMC = 10;
          }
          else{
            angleMN = 10;
            angleMC = -10;
          }
        }
      }

    }
    
    //Convergencia

    else if (m_controller->getProperty("meridian_convergence").getValue().toBool() == true && m_controller->getProperty("magnetic_north").getValue().toBool() == false){
      angle_magnetic = 0;
      if (angle_meridian == 0) {
        return;
      }

      else{

        if (angle_meridian < 0 && angle_magnetic == 0){
          angleMC = -10;
        }
        else if (angle_meridian > 0 && angle_magnetic == 0){
          angleMC = 10;
        }

      }
    }
}

void te::layout::ThreeNorthItem::drawTrueNorth(QPainter * painter)
{
  const Property& lineWidth = m_controller->getProperty("line_width");
  double lnew = lineWidth.getValue().toDouble();

  painter->save();
  QColor cpen(0,0,0);
  QPen pn(cpen, lnew, Qt::SolidLine);
  painter->setPen(pn);

  QString qStrUnit("NQ");
  ItemUtils utils(scene());
  double fontNQ = boundingRect().width() / 7.;
  double test = -boundingRect().height() / 38.;
  QPainterPath textPath = utils.textToVector(qStrUnit, QFont("Arial", fontNQ),QPointF(- boundingRect().height() / 38., boundingRect().center().y() + boundingRect().height() / 2.3), 0);

  QPainterPath rect_path;

  QPointF p5 = QPointF(0, boundingRect().center().y() + boundingRect().height() / 2.4);
  QPointF p6 = QPointF(0, boundingRect().center().y() - boundingRect().height() / 2.);

  QPolygonF north;
  north << p5 << p6;

  rect_path.addPolygon(north);
  rect_path.addPath(textPath);

  QTransform transf;
  transf.rotate(0);
  rect_path = transf.map(rect_path);
  rect_path.translate(boundingRect().width() / 2., 0.);

  painter->setPen(pn);
  setBrush(painter);
  painter->drawPath(rect_path);
  painter->restore();
  
}

void te::layout::ThreeNorthItem::drawMagneticNorth(QPainter * painter, double angleMN)
{
  const Property& lineWidth = m_controller->getProperty("line_width");
  double lnew = lineWidth.getValue().toDouble();

  painter->save();
  QColor cpen(0, 0, 0);
  QPen pn(cpen, lnew, Qt::SolidLine);
  painter->setPen(pn);
  double w = boundingRect().width() / 35.;
  double h = boundingRect().height() / 55.;
  double x1 = -(h / 2);
  double x2 = (h / 2);
  double y = boundingRect().center().y() + boundingRect().height() / 2.9;
  double ch1 = boundingRect().center().y() + boundingRect().height() / 2.4;
  double ch2 = boundingRect().center().y() + boundingRect().height() / 2.7;

  QPointF p1 = QPointF(0, ch1);
  QPointF p2 = QPointF(x1 - w, y);
  QPointF p3 = QPointF(0, ch2);
  QPointF p4 = QPointF(x2 + w, y);

  QPolygonF arrow;
  arrow << p1 << p2 << p3;// << p4;

  QPointF p5 = QPointF(0, boundingRect().center().y() + boundingRect().height() / 2.4);
  QPointF p6 = QPointF(0, boundingRect().center().y() - boundingRect().height() / 2.);

  QPolygonF north;
  north << p5 << p6;

  QString qStrUnit("NM");
  ItemUtils utils(scene());
  double fontNM = boundingRect().width() / 7.;
  QPainterPath textPath = utils.textToVector(qStrUnit, QFont("Arial", fontNM),QPointF(-boundingRect().height() / 38., boundingRect().center().y() + boundingRect().height() / 2.3), 0);

  QPainterPath rect_path;
  rect_path.addPolygon(north);
  rect_path.addPolygon(arrow);
  rect_path.addPath(textPath);

  QTransform transf;
  transf.rotate(angleMN);
  rect_path = transf.map(rect_path);
  rect_path.translate(boundingRect().width() / 2., 0.);
  painter->setPen(pn);
  setBrush(painter);
  painter->drawPath(rect_path);
  painter->restore();
}

void te::layout::ThreeNorthItem::drawMeridianConvergence(QPainter * painter, double angleMC)
{
  const Property& lineWidth = m_controller->getProperty("line_width");
  double lnew = lineWidth.getValue().toDouble();

  painter->save();
  QColor cpen(0, 0, 0);
  QPen pn(cpen, lnew, Qt::SolidLine);
  painter->setPen(pn);

  QPolygonF star;
  double halfW = boundingRect().width() / 54.;
  double w = boundingRect().width() / 27.;
  qreal const c = halfW;
  qreal const d = w;
  bool inner = true;

  QPointF pUnion;

  for (qreal i = 0; i < 2 * 3.14; i += 3.14 / 5.0, inner = !inner) {
    qreal const f = inner ? c : d;
    star << QPointF(f * std::cos(i), f * std::sin(i));
    if (i == 0)
    {
      pUnion = QPointF(f * std::cos(i), f * std::sin(i));
    }
  }

  star << pUnion;

  QMatrix matrix;
  matrix.rotate(50.);
  star = matrix.map(star);
  star.translate(QPointF(0, boundingRect().center().y() + boundingRect().height() / 2.6));
  QString qStrUnit("NG");
  ItemUtils utils(scene());
  double fontNG = boundingRect().width() / 7.;
  QPainterPath textPath = utils.textToVector(qStrUnit, QFont("Arial", fontNG), QPointF(-boundingRect().height() / 38., boundingRect().center().y() + boundingRect().height() / 2.3), 0);

  QPainterPath rect_path;

  QPointF p5 = QPointF(0, boundingRect().center().y() + boundingRect().height() / 2.4);
  QPointF p6 = QPointF(0, boundingRect().center().y() - boundingRect().height() / 2.);

  QPolygonF north;
  north << p5 << p6;

  rect_path.addPolygon(north);
  rect_path.addPolygon(star);
  rect_path.addPath(textPath);
  QTransform transf;
  transf.rotate(angleMC);
  rect_path = transf.map(rect_path);
  rect_path.translate(boundingRect().width() / 2., 0.);

  painter->setPen(pn);
  setBrush(painter);
  painter->drawPath(rect_path);
  painter->restore();
}

void te::layout::ThreeNorthItem::drawArc(QPainter * painter)
{
  const Property& lineWidth = m_controller->getProperty("line_width");
  double lnew = lineWidth.getValue().toDouble();

  painter->save();
  QColor cpen(0, 0, 0);
  QPen pn(cpen, lnew, Qt::SolidLine);
  painter->setPen(pn);

  // Conversion factor: degrees to radians
  double TeCDR = 0.01745329251994329576;

  //Angles Magnetic North and Meridian Convergence
  double angleMN = 0;
  double angleMC = 0;

  //Get angles
  getAngles(angleMN, angleMC);

  //Calculation for the drawing of the arc
  double refX = boundingRect().width() / 2.;
  double refY = 0.;
  double distance = boundingRect().height() / 2.;
  double x = boundingRect().width() / 2;
  double y = boundingRect().height() / 2 + boundingRect().height() / 8;
  double cosineMagneticNorth = std::cos(TeCDR * (90. + angleMN)) * distance;
  double sinMagneticNorth = std::sin(TeCDR * (90. + angleMN)) * distance;
  double x1MagneticNorth = refX + cosineMagneticNorth;
  double y1MagneticNorth = refY + sinMagneticNorth;
  double cosineMeridianConvergence = std::cos(TeCDR * (90. + angleMC)) * distance;
  double sinMeridianConvergence = std::sin(TeCDR * (90. + angleMC)) * distance;
  double x1MeridianConvergence = refX + cosineMeridianConvergence;
  double y1MeridianConvergence = refY + sinMeridianConvergence;

  //Arc of Declination and Convergence

  QLineF lne1(x1MagneticNorth, y1MagneticNorth, x1MeridianConvergence, y1MeridianConvergence);

  //Set text 

  double angleMerC = m_controller->getProperty("angle_meridian_convergence").getValue().toDouble();
  double angleMagN = m_controller->getProperty("angle_magnetic_north").getValue().toDouble();
  stringstream ssM;
  ssM << angleMerC;
  string strangleMerC = ssM.str();
  QString qangleM = QString::fromStdString(strangleMerC);
  string font = "Arial";
  double fontd = boundingRect().width() / 7.;
  ItemUtils utils(scene());

  stringstream ssN;
  ssN << angleMagN;
  string strangleMagN = ssN.str();
  QString qangleN = QString::fromStdString(strangleMagN);
  

  // Get angles of Model Properties

  double angle_magnetic = m_controller->getProperty("angle_magnetic_north").getValue().toDouble();
  double angle_meridian = m_controller->getProperty("angle_meridian_convergence").getValue().toDouble();

  //Convert to degree, minute and second

  Utils util = ((Scene*) this->scene())->getUtils(); 
    
  std::string dmsAngleMer = util.convertDecimalToDegree(angleMerC, true, true, true, 3);
  std::string dmsAngleMag = util.convertDecimalToDegree(angleMagN, true, true, true, 3);

  QString dmsAngleMerC = utils.convert2QString(dmsAngleMer);
  QString dmsAngleMagN = utils.convert2QString(dmsAngleMag);

  //Paths Magnetic North and Meridian Convergence
  QPainterPath anglePathMagN;
  QPainterPath anglePathMerC;

  double dpi = 96.;
  Scene* myScene = dynamic_cast<Scene*>(this->scene());
  if (myScene != 0)
  {
    dpi = myScene->getContext().getDpiY();
  }

  if (m_controller->getProperty("meridian_convergence").getValue().toBool() == true && m_controller->getProperty("magnetic_north").getValue().toBool() == true){

    double distance = boundingRect().height() / 2 + boundingRect().height() / 9.4;
    double cosineMeridianConver = std::cos(TeCDR * (90. + angleMC)) * distance;
    double sinMeridianConver = std::sin(TeCDR * (90. + angleMC)) * distance;
    double x1MeridianConvergence = refX + cosineMeridianConver;
    double y1MeridianConvergence = refY + sinMeridianConver;
    QLineF lne2(x, y, x1MeridianConvergence, y1MeridianConvergence);
    

    if (angle_meridian == 0 && angle_magnetic == 0)
    {
      painter->restore();
      return;
    }
    else{

      double maxWidth = 0.;
      double font = boundingRect().width() / 7.;

      //Set Text
      anglePathMerC = utils.textToVector(dmsAngleMerC, QFont("Arial", font));
      anglePathMagN = utils.textToVector(dmsAngleMagN, QFont("Arial", font));

      QRectF mcRect = anglePathMerC.boundingRect();
      QRectF mnRect = anglePathMagN.boundingRect();

      if (angle_meridian == 0 && angle_magnetic > 0){

        x1MeridianConvergence = refX + cosineMeridianConver;
        y1MeridianConvergence = refY + sinMeridianConver;
        x1MagneticNorth = refX + cosineMagneticNorth;
        y1MagneticNorth = refY + sinMagneticNorth;

        anglePathMerC.translate(x1MeridianConvergence + boundingRect().width() / 80., y1MeridianConvergence - (mcRect.height() / 2.));
        anglePathMagN.translate(x1MagneticNorth - mnRect.width() - boundingRect().width() / 80., y1MagneticNorth - (mnRect.height() / 2.));
      }

      else if (angle_magnetic == 0 && angle_meridian > 0){

        x1MeridianConvergence = refX + cosineMeridianConver;
        y1MeridianConvergence = refY + sinMeridianConver;
        x1MagneticNorth = refX + cosineMagneticNorth;
        y1MagneticNorth = refY + sinMagneticNorth;

        anglePathMerC.translate(x1MeridianConvergence + boundingRect().width() / 80., y1MeridianConvergence - (mcRect.height() / 2.));
        anglePathMagN.translate(x1MagneticNorth - mnRect.width() - boundingRect().width() / 80., y1MagneticNorth - (mnRect.height() / 2.));
      }

      else if (angle_meridian < 0 && angle_magnetic > 0){

        x1MeridianConvergence = refX + cosineMeridianConver;
        y1MeridianConvergence = refY + sinMeridianConver;
        x1MagneticNorth = refX + cosineMagneticNorth;
        y1MagneticNorth = refY + sinMagneticNorth;

        anglePathMerC.translate(x1MeridianConvergence + boundingRect().width() / 80., y1MeridianConvergence - (mcRect.height() / 2.));
        anglePathMagN.translate(x1MagneticNorth - mnRect.width() - boundingRect().width() / 80., y1MagneticNorth - (mnRect.height() / 2.));


        // Largura do box

        double leftX = x1MagneticNorth - mnRect.width();
        double rightX = x1MeridianConvergence + mcRect.width();

        double midX = boundingRect().width() / 2.;

        double leftWidth = midX - leftX;
        double rightWidth = rightX - midX;

        maxWidth = leftWidth;
        if (maxWidth < rightWidth)
        {
          maxWidth = rightWidth;
        }

        maxWidth = maxWidth * 2.;

      }

      else if (angle_meridian > 0 && angle_magnetic < 0){

        x1MeridianConvergence = refX + cosineMeridianConver;
        y1MeridianConvergence = refY + sinMeridianConver;
        x1MagneticNorth = refX + cosineMagneticNorth;
        y1MagneticNorth = refY + sinMagneticNorth;

        anglePathMerC.translate(x1MeridianConvergence - mcRect.width() - boundingRect().width() / 80., y1MeridianConvergence - (mcRect.height() / 2.));
        anglePathMagN.translate(x1MagneticNorth + boundingRect().width() / 80., y1MagneticNorth - (mnRect.height() / 2.));


      }

      else if (angle_meridian < 0 && angle_magnetic < 0){

        x1MeridianConvergence = refX + cosineMeridianConver;
        y1MeridianConvergence = refY + sinMeridianConver;
        x1MagneticNorth = refX + cosineMagneticNorth;
        y1MagneticNorth = refY + sinMagneticNorth;

        anglePathMerC.translate(x1MeridianConvergence + boundingRect().width() / 80., y1MeridianConvergence - (mcRect.height() / 2.));
        anglePathMagN.translate(x1MagneticNorth - mnRect.width() - boundingRect().width() / 80., y1MagneticNorth - (mnRect.height() / 2.));

      }

      else if (angle_meridian > 0 && angle_magnetic > 0){

        if (angle_meridian == angle_magnetic){

          x1MeridianConvergence = refX + cosineMagneticNorth;
          y1MeridianConvergence = refY + sinMagneticNorth;
          x1MagneticNorth = refX + cosineMeridianConver;
          y1MagneticNorth = refY + sinMeridianConver;

          anglePathMerC.translate(x1MeridianConvergence + boundingRect().width() / 80., y1MeridianConvergence - (mcRect.height() / 2.));
          anglePathMagN.translate(x1MagneticNorth - mnRect.width(), y1MagneticNorth - (mcRect.height() / 2.));

        }

        else if (angle_meridian > angle_magnetic){

          x1MeridianConvergence = refX + cosineMagneticNorth;
          y1MeridianConvergence = refY + sinMagneticNorth;
          x1MagneticNorth = refX + cosineMeridianConver;
          y1MagneticNorth = refY + sinMeridianConver;

          anglePathMerC.translate(x1MeridianConvergence + boundingRect().width() / 80., y1MeridianConvergence - (mcRect.height() / 2.));
          anglePathMagN.translate(x1MagneticNorth - mnRect.width(), y1MagneticNorth - (mcRect.height() / 2.));

        }

        else if (angle_meridian < angle_magnetic){

          x1MeridianConvergence = refX + cosineMagneticNorth;
          y1MeridianConvergence = refY + sinMagneticNorth;
          x1MagneticNorth = refX + cosineMeridianConver;
          y1MagneticNorth = refY + sinMeridianConver;

          anglePathMerC.translate(x1MeridianConvergence - mcRect.width() - boundingRect().width() / 80., y1MeridianConvergence - (mcRect.height() / 2.));
          anglePathMagN.translate(x1MagneticNorth + boundingRect().width() / 80., y1MagneticNorth - (mcRect.height() / 2.));

        }

      }


    }
    painter->setPen(pn);
    setBrush(painter);
    painter->drawLine(lne1);
    painter->drawLine(lne2);
    painter->drawPath(anglePathMerC);
    painter->drawPath(anglePathMagN);
  }

  else if (m_controller->getProperty("meridian_convergence").getValue().toBool() == true && m_controller->getProperty("magnetic_north").getValue().toBool() == false){

    angle_magnetic = 0;
    double distance = boundingRect().height() / 2 + boundingRect().height() / 9.4;
    double cosineMeridianConver = std::cos(TeCDR * (90. + angleMC)) * distance;
    double sinMeridianConver = std::sin(TeCDR * (90. + angleMC)) * distance;
    double x1MeridianConvergence = refX + cosineMeridianConver;
    double y1MeridianConvergence = refY + sinMeridianConver;
    QLineF lne2(x, y, x1MeridianConvergence, y1MeridianConvergence);
      

    if (angle_meridian == 0 && angle_magnetic == 0) {
      painter->restore();
      return;
    }
    else{

      double maxWidth = 0.;
      double font = boundingRect().width() / 7.;

      QRectF mcRect = anglePathMerC.boundingRect();
      QRectF mnRect = anglePathMagN.boundingRect();

      anglePathMerC = utils.textToVector(dmsAngleMerC, QFont("Arial", font));

      //Convergencia Magnetica

      if (angle_meridian == 0) {
        painter->restore();
        return;
      }
      else{
        double maxWidth = 0.;
        double font = boundingRect().width() / 7.;
        anglePathMerC = utils.textToVector(dmsAngleMerC, QFont("Arial", font));

        QRectF mcRect = anglePathMerC.boundingRect();

        if (angle_meridian > 0 && angle_magnetic == 0){

          x1MagneticNorth = refX + cosineMeridianConver;
          y1MagneticNorth = refY + sinMeridianConver;

          anglePathMerC.translate(x1MagneticNorth - mcRect.width() - boundingRect().width() / 80., y1MagneticNorth - (mcRect.height() / 2.));

        }

        else if (angle_meridian < 0 && angle_magnetic == 0){

          x1MeridianConvergence = refX + cosineMeridianConver;
          y1MeridianConvergence = refY + sinMeridianConver;

          anglePathMerC.translate(x1MeridianConvergence + boundingRect().width() / 80., y1MeridianConvergence - (mcRect.height() / 2.));

        }
      }
    }
    painter->setPen(pn);
    setBrush(painter);
    painter->drawLine(lne2);
    painter->drawPath(anglePathMerC);
  }
  painter->restore();
}

void te::layout::ThreeNorthItem::setBrush(QPainter* painter)
{
  const Property& colorProperty = m_controller->getProperty("color");
  const te::color::RGBAColor& color = colorProperty.getValue().toColor();
  QColor brushColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
  painter->setBrush(QBrush(brushColor));
}

QString te::layout::ThreeNorthItem::DMS2DD(const QString dms)
{
    int pos = 0;
    if (dms.startsWith('+') || dms.startsWith('-'))
    {
      pos = 1;
    }
    int a = dms.indexOf('°');
    int b = dms.indexOf('\'');
    int c = dms.indexOf('\'', b + 1);
    double deg = dms.mid(pos, a - pos).replace(QString(" "), QString("")).toDouble();
    double min = dms.mid(a + 1, b - a - 1).replace(QString(" "), QString("")).toDouble();
    double sec = dms.mid(b + 1, c - b - 1).replace(QString(" "), QString("")).toDouble();
    double coord = deg + (min / 60.0) + (sec / 3600.0);
    if (dms.mid(0, a).toDouble() < 0)
    {
      coord *= -1;
    }

    std::string output = te::common::Convert2String(coord, 4);
    return ItemUtils::convert2QString(output);
}

QString te::layout::ThreeNorthItem::DD2DMS(QString dd)
{
    int degree;
    int minute;
    double second;
    double ll;

    double value = dd.replace(QString(" "), QString("")).toDouble();

    degree = (int)value;
    ll = value - degree;
    minute = (int)(ll * 60.);
    ll = (ll * 60.) - minute;
    second = ll * 60.;

    QString output;
    if (degree < 0)
    {
      output = QString::fromLatin1(("-" + te::common::Convert2String(abs(degree)) + "°" + te::common::Convert2String(abs(minute)) + "'" + te::common::Convert2String(fabs(second), 2) + "''").c_str());
   }
    else
    {
      output = QString::fromLatin1((te::common::Convert2String(abs(degree)) + "°" + te::common::Convert2String(abs(minute)) + "'" + te::common::Convert2String(fabs(second), 2) + "''").c_str());
    }
    return output;
}