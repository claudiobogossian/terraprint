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
  \file TextGridModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextGridModel.h"
#include "../core/enum/Enums.h"

te::layout::TextGridModel::TextGridModel()
  : TextModel()
{
  std::size_t numRows = 1;
  std::size_t numColumns = 1;

  std::vector< std::vector<std::string> > textMatrix;
  textMatrix.resize(numRows);
  textMatrix[0].resize(numColumns);

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  {
    Property property;
    property.setName("text_matrix");
    property.setLabel(TR_LAYOUT("Text Matrix"));
    property.setValue(textMatrix, dataType->getDataTypeStringMatrix());
    property.setVisible(false);

    m_properties.addProperty(property);
  }

  {
    Property property;
    property.setName("num_rows");
    property.setLabel(TR_LAYOUT("Number of Rows"));
    property.setValue(numRows, dataType->getDataTypeInt());

    m_properties.addProperty(property);
  }

  {
    Property property;
    property.setName("num_columns");
    property.setLabel(TR_LAYOUT("Number of Columns"));
    property.setValue(numColumns, dataType->getDataTypeInt());

    m_properties.addProperty(property);
  }

  reparentProperties(Enums::getInstance().getEnumObjectType()->getTextGridItem());
}

te::layout::TextGridModel::~TextGridModel()
{
}
