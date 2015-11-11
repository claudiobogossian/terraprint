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

te::layout::ThreeNorthItem::ThreeNorthItem(AbstractItemController* controller, bool invertedMatrix) :
AbstractItem<QGraphicsItem>(controller, invertedMatrix)
{  

}

te::layout::ThreeNorthItem::~ThreeNorthItem()
{

}


void te::layout::ThreeNorthItem::drawItem(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	drawTrueNorthStar(painter);
	drawAngles(painter);
	//descobre a angulacao de cada linha (convergencia e declinaca)
	double angleA =0;
	double angleB = 0;

	getAngles(angleA, angleB);


	if (m_controller->getProperty("magnetic_north").getValue().toBool() == true){
		drawMagneticNorthArrow(painter, angleA);
	}


	if (m_controller->getProperty("magnetic_convergence").getValue().toBool() == true)
	{
		drawGridNorthSquare(painter, angleB);
	}

	return;

}

void te::layout::ThreeNorthItem::getAngles(double &angleA, double &angleB){

	double angle_magnetic = m_controller->getProperty("angle_magnetic_north").getValue().toDouble();
	double angle_grid = m_controller->getProperty("angle_magnetic_convergence").getValue().toDouble();

	if (angle_grid == 0 && angle_magnetic == 0) {
		return;
	}
	else{

		if (angle_grid<0 && angle_magnetic>0){
			if (angle_grid < angle_magnetic){
				angleA = 15;
				angleB = -15;
			}
			else{
				angleA = -15;
				angleB = 15;
			}
		}
		else if (angle_grid > 0 && angle_magnetic < 0){
			if (angle_grid < angle_magnetic){
				angleA = 15;
				angleB = -15;
			}
			else{
				angleA = -15;
				angleB = 15;
			}
		}
		else if (angle_grid < 0 && angle_magnetic < 0){
			if (angle_grid < angle_magnetic){
				angleA = -30;
				angleB = -15;
			}
			else{
				angleA = -15;
				angleB = -30;
			}
		}
		else if (angle_grid > 0 && angle_magnetic > 0){
			if (angle_grid > angle_magnetic){
				angleA = 15;
				angleB = 30;
			}
			else{
				angleA = 30;
				angleB = 15;
				}
		}
		return;
	}
}

void te::layout::ThreeNorthItem::drawTrueNorthStar(QPainter * painter)
{
  painter->save();
  QColor cpen(0,0,0);
  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);
 
  double dpi = 96.;
  Scene* myScene = dynamic_cast<Scene*>(this->scene());
  if (myScene != 0)
  {
	  dpi = myScene->getContext().getDpiY();
  }

  QString qStrUnit("NQ");
  ItemUtils utils(scene());
  QPainterPath textPath = utils.textToVector(qStrUnit, QFont("Arial", 17), dpi, QPointF(-2.1, boundingRect().center().y() + boundingRect().height() / 2.2), 0);

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
void te::layout::ThreeNorthItem::drawMagneticNorthArrow(QPainter * painter, double angleA)
{
	painter->save();
	QColor cpen(0, 0, 0);
	QPen pn(cpen, 0, Qt::SolidLine);
	painter->setPen(pn);
	double w = boundingRect().width() / 25.;
	double h = boundingRect().height() / 25.;
	double x1 = -(h / 2);
	double x2 = (h / 2);
	double y = boundingRect().center().y() + boundingRect().height() / 2.8;
	double ch1 = boundingRect().center().y() + boundingRect().height() / 2.4;
	double ch2 = boundingRect().center().y() + boundingRect().height() / 2.6;

	QPointF p1 = QPointF(0, ch1);
	QPointF p2 = QPointF(x1 - w, y);
	QPointF p3 = QPointF(0, ch2);
	QPointF p4 = QPointF(x2 + w, y);

	QPolygonF arrow;
	arrow << p1 << p2 << p3 << p4;

	QPointF p5 = QPointF(0, boundingRect().center().y() + boundingRect().height() / 2.4);
	QPointF p6 = QPointF(0, boundingRect().center().y() - boundingRect().height() / 2.);

	QPolygonF north;
	north << p5 << p6;

	double dpi = 96.;
	Scene* myScene = dynamic_cast<Scene*>(this->scene());
	if (myScene != 0)
	{
		dpi = myScene->getContext().getDpiY();
	}

	QString qStrUnit("NM");
	ItemUtils utils(scene());
	QPainterPath textPath = utils.textToVector(qStrUnit, QFont("Arial", 17), dpi, QPointF(-2.1, boundingRect().center().y() + boundingRect().height() / 2.2), 0);

	QPainterPath rect_path;
	rect_path.addPolygon(north);
	rect_path.addPolygon(arrow);
	rect_path.addPath(textPath);

	QTransform transf;
	transf.rotate(angleA);
	rect_path = transf.map(rect_path);
	rect_path.translate(boundingRect().width() / 2., 0.);
	painter->setPen(pn);
	setBrush(painter);
	painter->drawPath(rect_path);
	painter->restore();
}

void te::layout::ThreeNorthItem::drawGridNorthSquare(QPainter * painter, double angleB)
{
	painter->save();
	QColor cpen(0, 0, 0);
	QPen pn(cpen, 0, Qt::SolidLine);
	painter->setPen(pn);

	QPolygonF star;
	double halfW = boundingRect().width() / 40.;
	double w = boundingRect().width() / 20.;
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

	double dpi = 96.;
	Scene* myScene = dynamic_cast<Scene*>(this->scene());
	if (myScene != 0)
	{
		dpi = myScene->getContext().getDpiY();
	}

	QMatrix matrix;
	matrix.rotate(46.5);
	star = matrix.map(star);
	star.translate(QPointF(0, boundingRect().center().y() + boundingRect().height() / 2.4));

	QString qStrUnit("NG");
	ItemUtils utils(scene());
	QPainterPath textPath = utils.textToVector(qStrUnit, QFont("Arial", 17), dpi, QPointF(-1.8, boundingRect().center().y() + boundingRect().height() / 2.), 0);

	QPainterPath rect_path;

	QPointF p5 = QPointF(0, boundingRect().center().y() + boundingRect().height() / 2.4);
	QPointF p6 = QPointF(0, boundingRect().center().y() - boundingRect().height() / 2.);

	QPolygonF north;
	north << p5 << p6;

	//QPointF p10 = QPointF(boundingRect().width() / 2 - boundingRect().width() / 30, boundingRect().height() / 2);

	rect_path.addPolygon(north);
	rect_path.addPolygon(star);
	rect_path.addPath(textPath);

	double angle_grid = m_controller->getProperty("angle_magnetic_convergence").getValue().toDouble();

	QTransform transf;
	transf.rotate(angleB);
	rect_path = transf.map(rect_path);
	rect_path.translate(boundingRect().width() / 2., 0.);

	painter->setPen(pn);
	setBrush(painter);
	painter->drawPath(rect_path);
	//painter->drawPath(textPath);
	painter->restore();
}

void te::layout::ThreeNorthItem::drawAngles(QPainter * painter)
{
	painter->save();
	QColor cpen(0, 0, 0);
	QPen pn(cpen, 0, Qt::SolidLine);
	painter->setPen(pn);


	//QPointF p9 = QPointF(boundingRect().width() / 2 + boundingRect().width() / 30, boundingRect().height() / 2);
	//QPointF p10 = QPointF(boundingRect().width() / 2 - boundingRect().width() / 30, boundingRect().height() / 2);


	//QPolygonF north4;
	//north4 << p9 << p10;
	//painter->drawPolygon(north4);
	painter->setPen(pn);
	setBrush(painter);
	painter->restore();
}

void te::layout::ThreeNorthItem::setBrush(QPainter* painter)
{
  const Property& colorProperty = m_controller->getProperty("color");
  const te::color::RGBAColor& color = colorProperty.getValue().toColor();
  QColor brushColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
  painter->setBrush(QBrush(brushColor));
}

