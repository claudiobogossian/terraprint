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
  \file MapCompositionModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapCompositionModel.h"

te::layout::MapCompositionModel::MapCompositionModel()
  : ItemGroupModel()
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //updating properties
  {
    Property property(0);
    property.setName("resizable");
    property.setValue(true, dataType->getDataTypeBool());
    m_properties.updateProperty(property);
  }

  {
    Property property(0);
    property.setName("editable");
    property.setValue(true, dataType->getDataTypeBool());
    m_properties.updateProperty(property);
  }

  reparentProperties(Enums::getInstance().getEnumObjectType()->getMapCompositionItem());
}

te::layout::MapCompositionModel::~MapCompositionModel()
{

}

