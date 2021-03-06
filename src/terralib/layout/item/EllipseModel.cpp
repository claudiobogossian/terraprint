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
  \file EllipseModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EllipseModel.h"
#include "../core/enum/Enums.h"
#include "../core/property/Properties.h"
#include "terralib/color/RGBAColor.h"
#include "terralib/geometry/Envelope.h"
#include "../core/Utils.h"

#include <terralib/core/translator/Translator.h>

te::layout::EllipseModel::EllipseModel()
  : AbstractItemModel()
{
  te::color::RGBAColor fillColor(0, 0, 0, 255);
  te::color::RGBAColor contourColor(0, 0, 0, 255);
  double width = 22.;
  double height = 20.;
  double lineWidth = Utils::getLineWidthMinimumValue();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //adding properties
  {
    Property property(0);
    property.setName("line_width");
    property.setLabel(TE_TR("Line Width"));
    property.setVisible(false);
    property.setValue(lineWidth, dataType->getDataTypeDouble());
    this->m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("fill_color");
    property.setLabel(TE_TR("Fill Color"));
    property.setValue(fillColor, dataType->getDataTypeColor());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("contour_color");
    property.setLabel(TE_TR("Contour Color"));
    property.setValue(contourColor, dataType->getDataTypeColor());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

//updating properties
  {
    Property property(0);
    property.setName("width");
    property.setValue(width, dataType->getDataTypeDouble());
    this->m_properties.updateProperty(property);
  }

  {
    Property property(0);
    property.setName("height");
    property.setValue(height, dataType->getDataTypeDouble());
    this->m_properties.updateProperty(property);
  }

  reparentProperties(Enums::getInstance().getEnumObjectType()->getEllipseItem());
}

te::layout::EllipseModel::~EllipseModel()
{

}
