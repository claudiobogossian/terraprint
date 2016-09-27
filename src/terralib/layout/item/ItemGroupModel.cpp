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
  \file ItemGroupModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemGroupModel.h"

te::layout::ItemGroupModel::ItemGroupModel()
  : AbstractItemModel()
{
  m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getItemGroup());

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  double thickness = 1.0;

  {
    Property property(0);
    property.setName("show_frame");
    property.setValue(true, dataType->getDataTypeBool());
    this->m_properties.updateProperty(property);
  }

  {
    Property property = m_properties.getProperty("frame_thickness");
    property.setVisible(true);
    property.setEditable(true);
    this->m_properties.completelyUpdateProperty(property);
  }

  //updating properties
  {
    Property property(0);
    property.setName("resizable");
    property.setLabel(TR_LAYOUT("Resizable"));
    property.setValue(true, dataType->getDataTypeBool());
    m_properties.updateProperty(property);
  }
}

te::layout::ItemGroupModel::~ItemGroupModel()
{

}


