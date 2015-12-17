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
  \file GridMapModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridMapModel.h"

#include "../core/property/GridSettingsConfigProperties.h"
#include "../core/property/SharedProperties.h"
#include "../core/pattern/mvc/AbstractItemView.h"

te::layout::GridMapModel::GridMapModel() 
  : AbstractItemModel()
{
  std::string name;
  std::string mapName;

  //Reference Settings
  double scale = 0.;

  // Grid 
  bool visible = true;
  double lneHrzGap = 1000.; // 0.
  double lneVrtGap = 1000.; // 0.
  double m_initialGridPointX = 0.; 
  double m_initialGridPointY = 0.;

  // Line
  te::color::RGBAColor lineColor(0, 0, 0, 255);
  double lineWidth = 0.;

  // Text: Basic Configuration
  int pointTextSize = 6;
  std::string fontFamily("Arial");
  te::color::RGBAColor textColor(0, 0, 0, 255);

  // Text: Advanced configuration
  Font font;
  bool visibleAllTexts = true;
  bool superscriptText = false;
  double lneVrtDisplacement = 1.;
  double lneHrzDisplacement = 1.;
  bool bottomText = true;
  bool leftText = true;
  bool rightText = true;
  bool topText = true;
  bool bottomRotateText = false;
  bool leftRotateText = false;
  bool rightRotateText = false;
  bool topRotateText = false;

  double crossOffSet = 2.;

  std::string itemName = "";

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  GridSettingsConfigProperties settingsConfig;
  SharedProperties sharedProps;

  m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getGridMapItem());

  // Grid settings
  Property prop_gridsettings(0);
  prop_gridsettings.setName("GridSettings");
  prop_gridsettings.setLabel(TR_LAYOUT("Grid Settings"));
  prop_gridsettings.setMenu(true);
  prop_gridsettings.setPublic(true);
  prop_gridsettings.setValue(name, dataType->getDataTypeGridSettings());
  
  // Grid: sub properties
  {
    Property property(0);
    property.setName(settingsConfig.getVisible());
    property.setLabel(TR_LAYOUT("Visible"));
    property.setComposeWidget(true);
    property.setValue(visible, dataType->getDataTypeBool());  
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getLneHrzGap());
    property.setLabel(TR_LAYOUT("Horizontal Gap"));
    property.setComposeWidget(true);
    property.setValue(lneHrzGap, dataType->getDataTypeDouble());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getLneVrtGap());
    property.setLabel(TR_LAYOUT("Vertical Gap"));
    property.setComposeWidget(true);
    property.setValue(lneVrtGap, dataType->getDataTypeDouble());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getInitialGridPointX());
    property.setLabel(TR_LAYOUT("Initial Point X"));
    property.setComposeWidget(true);
    property.setValue(m_initialGridPointX, dataType->getDataTypeDouble());  
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getInitialGridPointY());
    property.setLabel(TR_LAYOUT("Initial Point Y"));
    property.setComposeWidget(true);
    property.setValue(m_initialGridPointY, dataType->getDataTypeDouble());  
    prop_gridsettings.addSubProperty(property);
  }

  //Just one is visible
  {
    EnumGridStyleType gridStyleType;
    EnumType* currentGridStyle = gridStyleType.getStyleContinuous();

    Property property(0);
    property.setName(settingsConfig.getStyle());
    property.setLabel(TR_LAYOUT("Style"));
    property.setComposeWidget(true);
    property.setValue(currentGridStyle->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentGridStyle->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for (int i = 0; i < gridStyleType.size(); ++i)
    {
      EnumType* enumType = gridStyleType.getEnum(i);

      if (enumType == gridStyleType.getStyleNone() || enumType == currentGridStyle)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    prop_gridsettings.addSubProperty(property);
  }

  // Line
  {
    EnumLineStyleType lineStyleType;
    EnumType* currentLineStyle = lineStyleType.getStyleSolid();

    Property property(0);
    property.setName(settingsConfig.getLineStyle());
    property.setLabel(TR_LAYOUT("Line Style"));
    property.setComposeWidget(true);
    property.setValue(currentLineStyle->getLabel(), dataType->getDataTypeStringList());

    Variant v;
    v.setValue(currentLineStyle->getLabel(), dataType->getDataTypeString());
    property.addOption(v);
    property.setOptionChoice(v);

    for (int i = 0; i < lineStyleType.size(); ++i)
    {
      EnumType* enumType = lineStyleType.getEnum(i);

      if (enumType == lineStyleType.getStyleNone() || enumType == currentLineStyle)
        continue;

      Variant v;
      v.setValue(enumType->getLabel(), dataType->getDataTypeString());
      property.addOption(v);
    }

    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getLineColor());
    property.setLabel(TR_LAYOUT("Line Color"));
    property.setComposeWidget(true);
    property.setValue(lineColor, dataType->getDataTypeColor());
    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getLineWidth());
    property.setLabel(TR_LAYOUT("Line Width"));
    property.setComposeWidget(true);
    property.setValue(lineWidth, dataType->getDataTypeDouble());
    prop_gridsettings.addSubProperty(property);
  }


  {
    Property property(0);
    property.setName(settingsConfig.getFont());
    property.setLabel(TR_LAYOUT("Font"));
    property.setValue(font, dataType->getDataTypeFont());
    prop_gridsettings.addSubProperty(property);
  }


  {
    Property property(0);
    property.setName(settingsConfig.getTextColor());
    property.setLabel(TR_LAYOUT("Text Color"));
    property.setComposeWidget(true);
    property.setValue(textColor, dataType->getDataTypeColor());
    prop_gridsettings.addSubProperty(property);
  }

  // Text: Advanced configuration
  {
    Property property(0);
    property.setName(settingsConfig.getVisibleAllTexts());
    property.setLabel(TR_LAYOUT("All Texts Visible"));
    property.setComposeWidget(true);
    property.setValue(visibleAllTexts, dataType->getDataTypeBool());
    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getSuperscriptText());
    property.setLabel(TR_LAYOUT("Superscript"));
    property.setComposeWidget(true);
    property.setVisible(false); //need review
    property.setValue(superscriptText, dataType->getDataTypeBool());
    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getLneVrtDisplacement());
    property.setLabel(TR_LAYOUT("Vertical Displacement"));
    property.setComposeWidget(true);
    property.setValue(lneVrtDisplacement, dataType->getDataTypeDouble());
    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getLneHrzDisplacement());
    property.setLabel(TR_LAYOUT("Horizontal Displacement"));
    property.setComposeWidget(true);
    property.setValue(lneHrzDisplacement, dataType->getDataTypeDouble());
    prop_gridsettings.addSubProperty(property);
  }

  {
    Property property(0);
    property.setName(settingsConfig.getBottomText());
    property.setLabel(TR_LAYOUT("Bottom Text Visible"));
    property.setComposeWidget(true);
    property.setValue(bottomText, dataType->getDataTypeBool());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getLeftText());
    property.setLabel(TR_LAYOUT("Left Text Visible"));
    property.setComposeWidget(true);
    property.setValue(leftText, dataType->getDataTypeBool());  
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getRightText());
    property.setLabel(TR_LAYOUT("Right Text Visible"));
    property.setComposeWidget(true);
    property.setValue(rightText, dataType->getDataTypeBool());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getTopText());
    property.setLabel(TR_LAYOUT("Top Text Visible"));
    property.setComposeWidget(true);
    property.setValue(topText, dataType->getDataTypeBool());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getBottomRotateText());
    property.setLabel(TR_LAYOUT("Bottom Text Rotate"));
    property.setComposeWidget(true);
    property.setValue(bottomRotateText, dataType->getDataTypeBool());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getLeftRotateText());
    property.setLabel(TR_LAYOUT("Left Text Rotate"));
    property.setComposeWidget(true);
    property.setValue(leftRotateText, dataType->getDataTypeBool());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getRightRotateText());
    property.setLabel(TR_LAYOUT("Right Text Rotate"));
    property.setComposeWidget(true);
    property.setValue(rightRotateText, dataType->getDataTypeBool());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getTopRotateText());
    property.setLabel(TR_LAYOUT("Top Text Rotate"));
    property.setComposeWidget(true);
    property.setValue(topRotateText, dataType->getDataTypeBool());
    prop_gridsettings.addSubProperty(property);
  }
  {
    Property property(0);
    property.setName(settingsConfig.getCrossOffset());
    property.setLabel(TR_LAYOUT("Cross Offset"));
    property.setComposeWidget(true);
    property.setValue(crossOffSet, dataType->getDataTypeDouble());
    prop_gridsettings.addSubProperty(property);
  }


  m_properties.addProperty(prop_gridsettings); // add gridsettings property

  //updating properties
  {
    Property property(0);
    property.setName("connect_item_position");
    property.setValue(true, dataType->getDataTypeBool());
    m_properties.updateProperty(property);
  }

  // Observer pattern relationship. Associate: != 0 / Dissociate : == 0.
  {
    Property property(0);
    property.setName(sharedProps.getItemObserver());
    property.setLabel(TR_LAYOUT("Connection with"));
    property.setComposeWidget(true);
    property.setValue(itemName, dataType->getDataTypeItemObserver());
    m_properties.addProperty(property);
  }
}

te::layout::GridMapModel::~GridMapModel()
{

}
