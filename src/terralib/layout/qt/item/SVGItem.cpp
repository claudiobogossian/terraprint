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
#include "../../item/SVGModel.h"
#include "SVGController.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/AbstractScene.h"
#include "terralib/color/RGBAColor.h"
#include "terralib/qt/widgets/Utils.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/common/STLUtils.h"
#include "../../item/PointModel.h"
#include "../../core/enum/EnumPointType.h"

// STL
#include <cmath>

// Qt
#include <QColor>
#include <QPen>
#include <QPolygonF>
#include <QPainterPath>
#include <QtSvg/QSvgRenderer>
#include "AbstractItem.h"

te::layout::SVGItem::SVGItem()
  : AbstractItem()
{
    
}

te::layout::SVGItem::~SVGItem()
{

}

te::layout::AbstractItemModel* te::layout::SVGItem::createModel() const
{
  return new SVGModel();
}

te::layout::AbstractItemController* te::layout::SVGItem::createController() const
{
  AbstractItemModel* model = createModel();
  return new SVGController(model, (AbstractItemView*)this);
}

void te::layout::SVGItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  painter->save();
  SVGController *svgController = dynamic_cast<SVGController*>(getController());

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  QRectF boundRect = getAdjustedBoundingRect(painter);
  QTransform transform;
  transform.translate(0., boundRect.y() + boundRect.height());
  transform.scale(1., -1.);
  transform.translate(0., -boundRect.y());

  painter->setTransform(transform, true);

  const Property& pFileName = svgController->getProperty("file_name");
  if (te::layout::Property::GetValueAs<std::string>(pFileName) == ""){
   
    painter->drawPolygon(boundRect);
    painter->restore();
    return;
  }

  const QString fileName = ItemUtils::convert2QString(te::layout::Property::GetValueAs<std::string>(pFileName));

  QSvgRenderer renderer(fileName);

  renderer.render(painter, boundRect);
  painter->restore();

}

