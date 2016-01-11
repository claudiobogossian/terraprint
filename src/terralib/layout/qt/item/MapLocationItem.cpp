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

#include "terralib/qt/widgets/canvas/Canvas.h"
#include "terralib/qt/widgets/canvas/MapDisplay.h"
#include "terralib/geometry/Utils.h"

te::layout::MapLocationItem::MapLocationItem(AbstractItemController* controller) 
  : MapItem(controller)
{

}

te::layout::MapLocationItem::~MapLocationItem()
{

}

void te::layout::MapLocationItem::drawMapOnDevice(QPaintDevice* device)
{
  te::layout::MapItem::drawMapOnDevice(device);

  const Property& pSrid = m_controller->getProperty("reference_srid");
  const Property& pBox = m_controller->getProperty("reference_box");
  const Property& pFillColor = m_controller->getProperty("reference_box_fill_color");
  const Property& pContourColor = m_controller->getProperty("reference_box_contour_color");

  int referenceSrid = pSrid.getValue().toInt();
  te::gm::Envelope referenceBox = pBox.getValue().toEnvelope();
  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  if (referenceSrid <= 0 || referenceBox.isValid() == false)
  {
    return;
  }

  te::gm::Envelope envelope = m_mapDisplay->getExtent();
  int srid = m_mapDisplay->getSRID();

  if (srid <= 0 || envelope.isValid() == false)
  {
    return;
  }

  te::qt::widgets::Canvas canvas(device);
  canvas.setWindow(envelope.m_llx, envelope.m_lly, envelope.m_urx, envelope.m_ury);

  if (srid != referenceSrid)
  {
    referenceBox.transform(referenceSrid, srid);
  }

  te::gm::Geometry* referenceGeometry = te::gm::GetGeomFromEnvelope(&referenceBox, srid);

  canvas.setPolygonFillColor(fillColor);
  canvas.setPolygonContourColor(contourColor);
  canvas.setPolygonContourDashStyle(te::map::DashLine);
  canvas.setPolygonContourWidth(2);
  canvas.draw(referenceGeometry);

  delete referenceGeometry;
}
