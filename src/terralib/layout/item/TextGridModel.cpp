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
  /*
  m_type = Enums::getInstance().getEnumObjectType()->getTextGridItem();

  m_spacing = 1;
  m_padding = 1;
  m_columnNumber = 5;
  m_rowNumber = 4;

  m_box = te::gm::Envelope(0., 0., 190., 170.);

  m_tableColor.setColor(0,0,0,255);
  m_borderGridColor.setColor(0,0,0,255);
  m_headerHorizontalColor.setColor(192,192,192,255);
  m_headerVerticalColor.setColor(192,192,192,255);
  */

  std::size_t numRows = 1;
  std::size_t numColns = 1;

  std::vector< std::vector<std::string> > textMatrix;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  {
    Property pMatrix;
    pMatrix.setName("text_matrix");
    pMatrix.setLabel(TR_LAYOUT("Text Matrix"));
    pMatrix.setValue(textMatrix, dataType->getDataTypeStringMatrix());
    pMatrix.setVisible(false);

    m_properties.addProperty(pMatrix);
  }

  reparentProperties(Enums::getInstance().getEnumObjectType()->getTextGridItem());
}

te::layout::TextGridModel::~TextGridModel()
{
}
