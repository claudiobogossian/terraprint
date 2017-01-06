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
  \file ItemGroup.cpp
   
  \brief 

  \ingroup layout
*/

#include "PaperItem.h"

#include "../../item/PaperModel.h"

// TerraLib
#include <terralib/color/RGBAColor.h>


te::layout::PaperItem::PaperItem()
  : AbstractItem(nullptr)
{  
  this->setFlags(QGraphicsItem::ItemSendsGeometryChanges);
  this->setAcceptHoverEvents(false);
}

te::layout::PaperItem::~PaperItem()
{

}

te::layout::AbstractItemModel* te::layout::PaperItem::createModel() const
{
  return new PaperModel();
}

void te::layout::PaperItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  const Property& pPaperColor = this->getProperty("paper_color");
  const Property& pShadowColor = this->getProperty("shadow_color");
  const Property& pFrameColor = this->getProperty("frame_color");
  const Property& pPaperWidth = this->getProperty("paper_width");
  const Property& pPaperHeight = this->getProperty("paper_height");
  const Property& pShadowPadding = this->getProperty("shadow_padding");

  const te::color::RGBAColor& paperColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pPaperColor);
  const te::color::RGBAColor& shadowColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pShadowColor);
  const te::color::RGBAColor& frameColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pFrameColor);
  double paperWidth = te::layout::Property::GetValueAs<double>(pPaperWidth);
  double paperHeight = te::layout::Property::GetValueAs<double>(pPaperHeight);
  double shadowPadding = te::layout::Property::GetValueAs<double>(pShadowPadding);

  QColor qPaperColor(paperColor.getRed(), paperColor.getGreen(), paperColor.getBlue(), paperColor.getAlpha());
  QColor qShadowColor(shadowColor.getRed(), shadowColor.getGreen(), shadowColor.getBlue(), shadowColor.getAlpha());
  QColor qFrameColor(frameColor.getRed(), frameColor.getGreen(), frameColor.getBlue(), frameColor.getAlpha());

  QRectF boundRect;
  boundRect = boundingRect();

  painter->save();

  QBrush bsh(qShadowColor);
  painter->setBrush(bsh);
  QPen pen(qFrameColor);
  painter->setPen(pen);

  QRectF boxShadow(boundRect.x() + shadowPadding, boundRect.y() - shadowPadding, boundRect.width() - shadowPadding, boundRect.height() - shadowPadding);
  painter->drawRect(boxShadow);
  
  bsh.setColor(qPaperColor);
  painter->setBrush(bsh);

  QRectF boxPaper = QRectF(boundRect.x(), boundRect.y(), boundRect.width() - shadowPadding, boundRect.height() - shadowPadding);
  painter->drawRect(boxPaper);

  painter->restore();
}
