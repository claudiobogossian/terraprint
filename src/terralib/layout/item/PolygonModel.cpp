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
  \file PolygonModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PolygonModel.h"
#include "../core/enum/Enums.h"
#include "../core/property/Properties.h"
#include "../core/enum/EnumDataType.h"
#include "../core/property/Property.h"
#include "terralib/color/RGBAColor.h"

te::layout::PolygonModel::PolygonModel()
{
  te::color::RGBAColor fillColor = te::color::RGBAColor(255, 255, 255, 255);
  te::color::RGBAColor contourColor = te::color::RGBAColor(0, 0, 0, 255);
  te::gm::GeometryShrPtr lineString(new te::gm::LineString(0, te::gm::LineStringType));

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //adding properties
  {
    Property property(0);
    property.setName("fill_color");
    property.setLabel(TR_LAYOUT("Fill Color"));
    property.setValue(fillColor, dataType->getDataTypeColor());
    property.setMenu(true);
    this->m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("contour_color");
    property.setLabel(TR_LAYOUT("Contour Color"));
    property.setValue(contourColor, dataType->getDataTypeColor());
    property.setMenu(true);
    this->m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("geometry");
    property.setLabel(TR_LAYOUT("Geometry"));
    property.setVisible(false);
    property.setValue<te::gm::GeometryShrPtr>(lineString, dataType->getDataTypeGeometry());
    this->m_properties.updateProperty(property);
  }

  this->m_properties.removeProperty("line_style_type");
  this->m_properties.removeProperty("color");
  
  reparentProperties(Enums::getInstance().getEnumObjectType()->getPolygonItem());
}

te::layout::PolygonModel::~PolygonModel()
{

}

