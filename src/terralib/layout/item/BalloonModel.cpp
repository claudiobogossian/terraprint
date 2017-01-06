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
  \file RectangleModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BalloonModel.h"
//#include "../core/ContextItem.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/color/RGBAColor.h"
#include "terralib/maptools/Canvas.h"
#include "../core/enum/Enums.h"
#include "../core/enum/EnumBalloonType.h"
#include "../core/enum/EnumBalloonDirectionType.h"
#include "TextModel.h"
#include "../core/enum/EnumAlignmentType.h"

te::layout::BalloonModel::BalloonModel()
: TextModel()
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  double margin = 5.;

  te::color::RGBAColor fillColor(255, 242, 188, 255);
  te::color::RGBAColor contourColor(0, 0, 0, 255);
  double contourThickness = 0.5;
  std::string text = "";

  {
    EnumBalloonType balloonType;
    EnumType* currentType = balloonType.getRectangleBalloon();

    Property property(0);
    property.setName("balloon_type");
    property.setLabel(TR_LAYOUT("Balloon Type"));
    property.setValue(currentType->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentType->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for (int i = 0; i < balloonType.size(); ++i)
    {
      EnumType* enumType = balloonType.getEnum(i);

      if (enumType == balloonType.getNoneType() || enumType == currentType)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    this->m_properties.addProperty(property);
  }

  {
    EnumAlignmentType enumAlignmentType;
    EnumType* currentAlignmentType = enumAlignmentType.getAlignmentCenterType();

    Property property(0);
    property.setName("alignment");
    property.setLabel(TR_LAYOUT("Alignment"));
    property.setValue(currentAlignmentType->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentAlignmentType->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for (int i = 0; i < enumAlignmentType.size(); ++i)
    {
      EnumType* enumType = enumAlignmentType.getEnum(i);

      if (enumType == enumAlignmentType.getNoneType() || enumType == currentAlignmentType)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    this->m_properties.updateProperty(property);
  }


  {
    EnumBalloonDirectionType balloonType;
    EnumType* currentType = balloonType.getRight();

    Property property(0);
    property.setName("balloon_direction");
    property.setLabel(TR_LAYOUT("Balloon Direction"));
    property.setValue(currentType->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentType->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for (int i = 0; i < balloonType.size(); ++i)
    {
      EnumType* enumType = balloonType.getEnum(i);

      if (enumType == balloonType.getNoneType() || enumType == currentType)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    this->m_properties.addProperty(property);
  }


  {
    Property property(0);
    property.setName("margin_size");
    property.setLabel(TR_LAYOUT("Margin Size"));
    property.setValue(margin, dataType->getDataTypeDouble());
    this->m_properties.addProperty(property);
  }
  
  {
    Property property(0);
    property.setName("resizable");
    property.setValue<bool>(true, dataType->getDataTypeBool());
    this->m_properties.updateProperty(property);
  }
  {
    Property property(0);
    property.setName("fill_color");
    property.setLabel(TR_LAYOUT("Fill Color"));
    property.setValue(fillColor, dataType->getDataTypeColor());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("contour_color");
    property.setLabel(TR_LAYOUT("Contour Color"));
    property.setValue(contourColor, dataType->getDataTypeColor());
    property.setMenu(true);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("contour_thickness");
    property.setLabel(TR_LAYOUT("Contour Thickness"));
    property.setValue(contourThickness, dataType->getDataTypeDouble());
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("text");
    property.setValue<std::string>(text, dataType->getDataTypeString());
    this->m_properties.updateProperty(property);
  }

  reparentProperties(Enums::getInstance().getEnumObjectType()->getBalloonItem());
}

te::layout::BalloonModel::~BalloonModel()
{

}
