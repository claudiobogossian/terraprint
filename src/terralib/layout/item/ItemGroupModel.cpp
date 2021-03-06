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

#include "../core/enum/EnumDataType.h"
#include "../core/enum/Enums.h"

#include <terralib/core/translator/Translator.h>

te::layout::ItemGroupModel::ItemGroupModel()
  : AbstractItemModel()
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //updating properties
  {
    Property property(0);
    property.setName("resizable");
    property.setLabel(TE_TR("Resizable"));
    property.setValue<bool>(true, dataType->getDataTypeBool());
    m_properties.updateProperty(property);
  }

  reparentProperties(Enums::getInstance().getEnumObjectType()->getItemGroup());
}

te::layout::ItemGroupModel::~ItemGroupModel()
{

}


