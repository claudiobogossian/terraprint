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
  \file SVGItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "SVGItem.h"
#include "../../core/AbstractScene.h"
#include "terralib/color/RGBAColor.h"
#include "terralib/qt/widgets/Utils.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/common/STLUtils.h"
#include "../../item/PointModel.h"
#include "../../core/enum/EnumPointType.h"
#include "../../item/SVGModel.h"
#include "SVGController.h"

// STL
#include <cmath>

// Qt
#include <QColor>
#include <QPen>
#include <QPolygonF>
#include <QPainterPath>
#include <QtSvg/QSvgRenderer>
#include "AbstractItem.h"

te::layout::SVGItem::SVGItem(AbstractItemController* controller, bool invertedMatrix)
: AbstractItem<QGraphicsItem>(controller, invertedMatrix)
{
    
}

te::layout::SVGItem::~SVGItem()
{

}

void te::layout::SVGItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  painter->save();
  SVGController *svgController = dynamic_cast<SVGController*>(m_controller);

  //QRectF adjustBoundRect = svgController->calculateSVGRect();

  QRectF boundRect = boundingRect();
  QTransform transform;
  transform.translate(0., boundRect.y() + boundRect.height());
  transform.scale(1., -1.);
  transform.translate(0., -boundRect.y());

  painter->setTransform(transform, true);

  Property pFileName = svgController->getProperty("file_name");

  if (pFileName.getValue().toString() == ""){
   
    painter->drawPolygon(boundRect);
    painter->restore();
    return;
  }
    
  const QString fileName = ItemUtils::convert2QString(pFileName.getValue().toString());

  QSvgRenderer renderer(fileName);

  renderer.render(painter, boundRect);
  painter->restore();

}

