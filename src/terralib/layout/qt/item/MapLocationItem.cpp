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
  \file MapLocation.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapLocationItem.h"

#include "MapLocationController.h"
#include "../../item/MapLocationModel.h"

#include <terralib/geometry/Utils.h>
#include <terralib/qt/widgets/canvas/Canvas.h>
#include <terralib/qt/widgets/canvas/MapDisplay.h>



te::layout::MapLocationItem::MapLocationItem(te::layout::ItemInputProxy* itemInputProxy)
  : MapItem(itemInputProxy)
{

}

te::layout::MapLocationItem::~MapLocationItem()
{

}

te::layout::AbstractItemModel* te::layout::MapLocationItem::createModel() const
{
  return new MapLocationModel();
}

te::layout::AbstractItemController* te::layout::MapLocationItem::createController() const
{
  AbstractItemModel* model = createModel();
  return new MapLocationController(model, (AbstractItemView*)this);
}

void te::layout::MapLocationItem::drawLayers(te::qt::widgets::Canvas* canvas, const te::gm::Envelope& envelope)
{
  te::layout::MapItem::drawLayers(canvas, envelope);

  const Property& pSrid = this->getProperty("srid");
  const Property& pReferenceSrid = this->getProperty("reference_srid");
  const Property& pReferenceBox = this->getProperty("reference_box");
  const Property& pFillColor = this->getProperty("reference_box_fill_color");
  const Property& pContourColor = this->getProperty("reference_box_contour_color");

  int srid = te::layout::Property::GetValueAs<int>(pSrid);
  int referenceSrid = te::layout::Property::GetValueAs<int>(pReferenceSrid);
  te::gm::Envelope referenceBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pReferenceBox);
  const te::color::RGBAColor& fillColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pFillColor);
  const te::color::RGBAColor& contourColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pContourColor);

  if (referenceSrid <= 0 || referenceBox.isValid() == false)
  {
    return;
  }

  if (srid <= 0 || envelope.isValid() == false)
  {
    return;
  }

  if (srid != referenceSrid)
  {
    referenceBox.transform(referenceSrid, srid);
  }

  te::gm::Geometry* referenceGeometry = te::gm::GetGeomFromEnvelope(&referenceBox, srid);

  canvas->setPolygonFillColor(fillColor);
  canvas->setPolygonContourColor(contourColor);
  canvas->setPolygonContourDashStyle(te::map::DashLine);
  canvas->setPolygonContourWidth(2);
  canvas->draw(referenceGeometry);

  delete referenceGeometry;
}
