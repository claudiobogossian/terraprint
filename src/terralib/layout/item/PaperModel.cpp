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

void te::layout::PaperModel::setProperty(const Property& property)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pCurrentPaperWidth = m_properties.getProperty("paper_width");
  const Property& pCurrentPaperHeight = m_properties.getProperty("paper_height");

  double currentPaperWidth = te::layout::Property::GetValueAs<double>(pCurrentPaperWidth);
  double currentPaperHeight = te::layout::Property::GetValueAs<double>(pCurrentPaperHeight);

  if(property.getName() == "paper_width")
  {
    double newPaperWidth = te::layout::Property::GetValueAs<double>(property);

    Properties properties = handleNewPaperSize(newPaperWidth, currentPaperHeight);
    properties.addProperty(property);

    AbstractItemModel::setProperties(properties);
    return;
  }
  else if(property.getName() == "paper_height")
  {
    double newPaperHeight = te::layout::Property::GetValueAs<double>(property);

    Properties properties = handleNewPaperSize(currentPaperWidth, newPaperHeight);
    properties.addProperty(property);

    AbstractItemModel::setProperties(properties);
    return;
  }

  AbstractItemModel::setProperty(property);
}

void te::layout::PaperModel::setProperties(const Properties& properties)
{
  const Property& pCurrentPaperWidth = m_properties.getProperty("paper_width");
  const Property& pCurrentPaperHeight = m_properties.getProperty("paper_height");

  double currentPaperWidth = te::layout::Property::GetValueAs<double>(pCurrentPaperWidth);
  double currentPaperHeight = te::layout::Property::GetValueAs<double>(pCurrentPaperHeight);

  if(properties.contains("paper_width") || properties.contains("paper_height"))
  {
    double newPaperWidth = currentPaperWidth;
    double newPaperHeight = currentPaperHeight;

    if(properties.contains("paper_width"))
    {
      const Property& pNewPaperWidth = properties.getProperty("paper_width");
      newPaperWidth = te::layout::Property::GetValueAs<double>(pNewPaperWidth);
    }
    if(properties.contains("paper_height"))
    {
      const Property& pNewPaperHeight = properties.getProperty("paper_height");
      newPaperHeight = te::layout::Property::GetValueAs<double>(pNewPaperHeight);
    }

    Properties fullProperties = handleNewPaperSize(newPaperWidth, newPaperHeight);
    for(size_t i = 0; i < properties.getProperties().size(); ++i)
    {
      fullProperties.addProperty(properties.getProperties()[i]);
    }
    
    AbstractItemModel::setProperties(fullProperties);
    return;
  }

  AbstractItemModel::setProperties(properties);
}

te::layout::Properties te::layout::PaperModel::handleNewPaperSize(double paperWidth, double paperHeight)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pShadowPadding = m_properties.getProperty("shadow_padding");
  double shadowPadding = te::layout::Property::GetValueAs<double>(pShadowPadding);

  double width = paperWidth + shadowPadding;
  double height = paperHeight + shadowPadding;

  Properties properties;
  {
    Property property(0);
    property.setName("width");
    property.setValue(width, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("height");
    property.setValue(height, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }

  return properties;
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

