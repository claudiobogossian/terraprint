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
  \file SVCModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "SVGModel.h"
#include "../core/enum/Enums.h"

te::layout::SVGModel::SVGModel()
  : AbstractItemModel()
{
  std::string fileName("");
  std::string fileDir("");

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  {
    Property property(0);
    property.setName("file_name");
    property.setLabel(TR_LAYOUT("File Name"));
    property.setValue(fileName, dataType->getDataTypeString());
    property.setEditable(false);
    property.setMenu(true);
    m_properties.addProperty(property);
  }


  {
    Property property(0);
    property.setName("file_dir");
    property.setValue(fileDir, dataType->getDataTypePath());
    property.setMenu(false);
    property.setVisible(false);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("show_frame");
    property.setValue<bool>(false, dataType->getDataTypeBool());
    this->m_properties.updateProperty(property);
  }
  {
    Property property(0);
    property.setName("keep_aspect");
    property.setLabel(TR_LAYOUT("Keep Aspect"));
    property.setValue<bool>(true, dataType->getDataTypeBool());
    property.setVisible(true);
    m_properties.completelyUpdateProperty(property);
  }

  {
    Property property(0);
    property.setName("SVG_Library");
    property.setLabel(TR_LAYOUT("SVG Library"));
    std::string name = "";
    property.setValue(name, dataType->getDataTypeSVGView());
    property.setVisible(true);
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  this->reparentProperties(Enums::getInstance().getEnumObjectType()->getSVGItem());
}

te::layout::SVGModel::~SVGModel()
{
  
}
