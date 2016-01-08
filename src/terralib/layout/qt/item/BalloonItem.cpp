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
  \file RectangleItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BalloonItem.h"

// STL
#include <algorithm>    // std::max and std::min
#include <cmath>

te::layout::BalloonItem::BalloonItem(AbstractItemController* controller, bool invertedMatrix)
  : AbstractItem<QGraphicsItem>(controller, invertedMatrix)
{      
  
}

te::layout::BalloonItem::~BalloonItem()
{

}

void te::layout::BalloonItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  const Property& property = m_controller->getProperty("border");

  if (property.isNull() == false){
    drawBalloon1(painter);
  }


}

void te::layout::BalloonItem::drawBalloon1(QPainter * painter){

  painter->save();
  QColor cpen = setBrush(painter);
  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);

  QPen pen(cpen, 0, Qt::SolidLine);
  pen.setWidth(1);

  QPointF p1 = QPointF(boundingRect().width() / 4., boundingRect().height() - (boundingRect().height() / 4.));
  QPointF p2 = QPointF((boundingRect().width() / 2.) + (boundingRect().width() / 4.), boundingRect().height() - (boundingRect().height() / 4.));

  QPointF p3 = QPointF(boundingRect().width() / 4., boundingRect().height() - (boundingRect().height() / 2.));
  QPointF p4 = QPointF((p2.rx() - (p2.rx()*0.2)), p3.ry());
  QPointF p5 = QPointF((p2.rx() - (p2.rx()*0.2)), p3.ry() - (p3.ry()*0.3));
  QPointF p6 = QPointF((p2.rx() - (p2.rx()*0.1)), p3.ry());
  QPointF p7 = QPointF(p2.rx(), p3.ry());


  painter->setPen(pen);

  painter->drawLine(p1, p2);
  painter->drawLine(p1, p3);
  painter->drawLine(p3, p4);
  painter->drawLine(p4, p5);
  painter->drawLine(p5, p6);
  painter->drawLine(p6, p7);
  painter->drawLine(p7, p2);

  painter->restore();

}

QColor te::layout::BalloonItem::setBrush(QPainter* painter)
{
  const Property& colorProperty = m_controller->getProperty("color");
  const te::color::RGBAColor& color = colorProperty.getValue().toColor();
  QColor brushColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
  painter->setBrush(QBrush(brushColor));
  return brushColor;
}

