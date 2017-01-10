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
  \file PaperModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PaperModel.h"
#include "../core/enum/Enums.h"

te::layout::PaperModel::PaperModel() 
  : AbstractItemModel()
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  te::color::RGBAColor paperColor(255, 255, 255, 255);
  te::color::RGBAColor shadowColor(0, 0, 0, 255);
  te::color::RGBAColor frameColor(255, 255, 255, 0);
  double shadowPadding = 10.;


  double paperWidth = 0.;
  double paperHeight = 0.;
  LayoutAbstractPaperType paperType = A4;
  LayoutOrientationType paperOrientation = Portrait;

  te::gm::Envelope box(0., - shadowPadding, paperWidth + shadowPadding, paperHeight);

  //adding properties
  {
    Property property(0);
    property.setName("paper_color");
    property.setLabel(TR_LAYOUT("Paper Color"));
    property.setValue(paperColor, dataType->getDataTypeColor());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("shadow_color");
    property.setLabel(TR_LAYOUT("Shadow Color"));
    property.setValue(shadowColor, dataType->getDataTypeColor());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("shadow_padding");
    property.setLabel(TR_LAYOUT("Shadow padding"));
    property.setValue(shadowPadding, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("paper_width");
    property.setLabel(TR_LAYOUT("Paper width"));
    property.setValue(paperWidth, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("paper_height");
    property.setLabel(TR_LAYOUT("Paper height"));
    property.setValue(paperHeight, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("paper_type");
    property.setLabel(TR_LAYOUT("Paper type"));
    property.setValue<int>((int)paperType, dataType->getDataTypeInt());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("paper_orientation");
    property.setLabel(TR_LAYOUT("Orientation"));
    property.setValue<int>((int)paperOrientation, dataType->getDataTypeInt());
    m_properties.addProperty(property);
  }

  //updating properties
  {
    Property property(0);
    property.setName("frame_color");
    property.setValue(frameColor, dataType->getDataTypeColor());
    m_properties.completelyUpdateProperty(property);
  }

  {
    Property property(0);
    property.setName("printable");
    property.setValue<bool>(false, dataType->getDataTypeBool());
    m_properties.completelyUpdateProperty(property);
  }

  {
    std::string name = "Paper";
    Property property(0);
    property.setName("name");
    property.setValue(name, dataType->getDataTypeString());
    m_properties.completelyUpdateProperty(property);
  }

  {
    Property property(0);
    property.setName("resizable");
    property.setValue<bool>(false, dataType->getDataTypeBool());
    m_properties.completelyUpdateProperty(property);
  }

  invisibleAllProperties();

  //Visible Properties
  {
    Property property(0);
    std::string value = TR_LAYOUT("Setup");
    property.setName("page_setup");
    property.setLabel(TR_LAYOUT("Page Setup"));
    property.setValue(value, dataType->getDataTypePageSetup());
    property.setMenu(true);
    m_properties.addProperty(property);
  }
  
  reparentProperties(Enums::getInstance().getEnumObjectType()->getPaperItem());
}

te::layout::PaperModel::~PaperModel()
{

}

void te::layout::PaperModel::invisibleAllProperties()
{
  const std::vector<Property>& vecProperties = m_properties.getProperties();
  for (unsigned int i = 0; i < vecProperties.size(); ++i)
  {
    Property property = vecProperties[i];
    property.setVisible(false);
    m_properties.completelyUpdateProperty(property);
  }
}

